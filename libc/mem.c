//
// Created by Soptq on 2020/7/22.
//

#include "mem.h"
#include "../libc/assert.h"
#include "../cpu/page.h"

heap_t *kheap = 0;
uint32_t free_mem_addr = FREE_MEM_ADDR;

void *memset(void *str, int c, size_t n) {
    char *p = str;
    if (p == NULL) return NULL;
    while (n > 0) {
        *p = (char)c;
        p++;
        n--;
    }
    return str;
}

void *memcpy(void *dest, const void* src, size_t n) {
    char *p1 = dest;
    const char *p2 = src;
    if ((p1 == NULL) || (p2 == NULL)) return NULL;
    while (n) {
        *(p1++) = *(p2++);
        n--;
    }
    return dest;
}

//void *memmove(void * restrict dest, const void *src, size_t n) {
//    char *p1 = dest;
//    const char p2 = src;
//
//}

uint32_t _kmalloc(size_t size, int8_t align, uint32_t *phys_addr) {
    if (kheap != 0) {
        void *addr = alloc(size, (uint8_t)align, kheap);
        if (phys_addr != 0) {
            page_t *page = get_page((uint32_t)addr, 0, kernel_directory);
            *phys_addr = page -> frame * 0x1000 + (uint32_t)addr & 0xFFF;
        }
        return (uint32_t)addr;
    } else {
        /* if we need to aligh the memory and it is not already aligned */
        if (align == 1 && (free_mem_addr & 0x00000FFF)) {
            free_mem_addr &= 0xFFFFF000;    /* align it with a 4k unit */
            free_mem_addr += 0x1000;        /* 4k */
        }

        if (phys_addr) *phys_addr = free_mem_addr;

        uint32_t ret = free_mem_addr;
        free_mem_addr += size;
        return ret;
    }
}

void kfree(void *p)
{
    free(p, kheap);
}

/* kmalloc alighed */
uint32_t kmalloc_a(size_t size) {
    return _kmalloc(size, 1, 0);
}

/* kmalloc physical address */
uint32_t kmalloc_p(size_t size, uint32_t *phys_addr) {
    return _kmalloc(size, 0, phys_addr);
}

uint32_t kmalloc_ap(size_t size, uint32_t *phys_addr) {
    return _kmalloc(size, 1, phys_addr);
}

uint32_t kmalloc(size_t size) {
    return _kmalloc(size, 0, 0);
}


/*
 * Here the input size is the size of header, footer and the requested *real* data space.
 */
static int32_t find_smallest_hole(uint32_t size, uint8_t page_align, heap_t *heap) {
    uint32_t iterator = 0;
    while (iterator < heap -> index.size) {
        /* returned type_t (void *) is pointed to the header of the block / hole */
        heap_header_t *header = (heap_header_t*)lookup_ordered_array(iterator, &(heap -> index));
        if (page_align > 0) {   /* page-aligned */
            /* page align the starting point of the block / content */
            /* It's important to note that when the user requests memory to be page-aligned,
             * he is requesting the memory that he has access to to be page-aligned. */
            uint32_t location = (uint32_t) header;
            int32_t offset = 0;
            /* note: the priority of != is bigger then &, so we will blanket it */
            if (((location + sizeof(heap_header_t)) & 0xFFFFF000) != 0) {
                offset = 0x1000 - (location + sizeof(heap_header_t)) % 0x1000;
            }
            int32_t hole_size = (int32_t)header -> size - offset;
            if (hole_size >= (int32_t)size)
                break;
        } else if (header -> size >= size)
            break;
        iterator++;
    }

    if (iterator == heap -> index.size) {
        return -1;
    }

    return iterator;
}

static int8_t header_t_less_then(void* a, void* b) {
    return (((heap_header_t*)a) -> size < ((heap_header_t*)b) -> size);
}

/**
 * This function will create a heap index at start address.
 */
heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly) {
    heap_t *heap = (heap_t*)kmalloc(sizeof(heap_t));

    assert(start % 0x1000 == 0, "Heap initialization failed! Start address is not aligned");
    assert(end % 0x1000 == 0, "Heap initialization failed! End address is not aligned");

    heap -> index = place_ordered_array((void *)start, HEAP_INDEX_SIZE, &header_t_less_then);

    /* shift the start address toward to resemble where we can start putting heap data */
    start += sizeof(type_t) * HEAP_INDEX_SIZE;

    if (start & 0x00000FFF) {   /* start is not aligned */
        start &= 0xFFFFF000;
        start += 0x1000;
    }

    heap -> start_address = start;
    heap -> end_address = end;
    heap -> max_address = max;
    heap -> supervisor = supervisor;
    heap -> readonly = readonly;

    /* at initial the whole heap is a hole */
    heap_header_t *hole = (heap_header_t *)start;
    hole -> size = end - start;
    hole -> magic = HEAP_MAGIC;
    hole -> is_hole = 1;
    insert_ordered_array((void *)hole, &heap -> index);

    return heap;
}

static void expand(uint32_t new_size, heap_t *heap) {
    assert((new_size > ((heap -> end_address) - (heap -> start_address))),
            "Heap expand failed! The requested new_size is smaller than the exists");

    if (new_size & 0x00000FFF) {
        new_size &= 0xFFFFF000;
        new_size += 0x1000;
    }

    assert((heap -> start_address) + new_size <= (heap -> max_address),
            "Heap expand failed! The requested new_size is out of max_address");

    uint32_t old_size = (heap -> end_address) - (heap -> start_address);
    uint32_t i = old_size;
    while (i < new_size) {  /* allocate new memory by utilizing paging */
        alloc_frame( get_page(heap -> start_address + i, 1, kernel_directory),
                     (heap -> supervisor) ? 1 : 0, (heap -> readonly) ? 0 : 1);
        i += 0x1000;
    }
    heap -> end_address = heap -> start_address + new_size;
}

static uint32_t contract(uint32_t new_size, heap_t *heap) {
    assert((new_size < (heap -> end_address) - (heap -> start_address)),
            "Heap contract failed! The requested new_size is larger than the exists");

    if (new_size & 0x00000FFF) {
        new_size &= 0xFFFFF000;
        new_size += 0x1000;
    }

    if (new_size < HEAP_MIN_SIZE) {
        new_size = HEAP_MIN_SIZE;
    }

    uint32_t old_size = (heap -> end_address) - (heap -> start_address);
    /* if the difference between new_size and old_size is within 0x1000,
     * then we cannot free_frame() it as it is smaller than a page frame */
    uint32_t i = old_size - 0x1000;
    while (new_size < i) {
        free_frame(get_page(heap -> start_address + i, 0, kernel_directory));
        i -= 0x1000;
    }
    heap -> end_address = heap -> start_address + new_size;
    return new_size;
}

void *alloc(uint32_t size, uint8_t page_align, heap_t *heap) {
    /* the implementation of our memory allocator always taken into
     * account of the header and the footer at both ends of blocks / holes */
    uint32_t new_size = size + sizeof(heap_header_t) + sizeof(heap_footer_t);

    int32_t iterator = find_smallest_hole(new_size, page_align, heap);

    if (iterator == -1) {
        /* there is no such a hole,
         * we need to allocate the heap */
        uint32_t old_length = (heap -> end_address) - (heap -> start_address);
        uint32_t old_end_addr = heap -> end_address;

        expand(old_length + new_size, heap);
        uint32_t new_length = (heap -> end_address) - (heap -> start_address);

        /* after expanding, we need to adjust our heap index */
        iterator = 0;
        uint32_t idx = -1, value = 0x0;

        /* get the endmost index in memory address, as the endmost index is
         * just before what allocated, so we can merge these two hole together */
        while (iterator < heap -> index.size) {
            uint32_t tmp = (uint32_t)lookup_ordered_array(iterator, &heap -> index);
            if (tmp > value) {
                value = tmp;
                idx = iterator;
            }
            iterator++;
        }


        if (idx == -1) {    /* such a index is not exists */
                            /* this will happen when the index size is 0 */
                            /* so we will make a index */
            heap_header_t *header = (heap_header_t*)old_end_addr;
            header -> magic = HEAP_MAGIC;
            header -> size = new_length - old_length;
            header -> is_hole = 1;
            heap_footer_t *footer = (heap_footer_t*)
                    (old_end_addr + header -> size - sizeof(heap_footer_t));
            footer -> magic = HEAP_MAGIC;
            footer -> header = header;
            insert_ordered_array((void *)header, &heap -> index);
        } else {    /* merge the endmost index with what we just allocated */
            heap_header_t *header = lookup_ordered_array(idx, &heap -> index);
            header -> size += new_length - old_length;
            heap_footer_t *footer = (heap_footer_t*)
                    ((uint32_t)header + header -> size - sizeof(heap_footer_t));
            footer -> magic = HEAP_MAGIC;
            footer -> header = header;
        }

        /* after expanding and adjusting,
         * we are sure that the next run of alloc() will return the desired result */
        return alloc(size, page_align, heap);
    }

    /* fetch the header of the hole */
    heap_header_t *orig_hole_header =
            (heap_header_t *)lookup_ordered_array(iterator, &heap -> index);
    uint32_t orig_hole_pos = (uint32_t)orig_hole_header;
    uint32_t orig_hole_size = orig_hole_header -> size;
    /* see if we can split the hole to 2 hole to improve RAM use ratio */
    if ((orig_hole_size - new_size) <
        (sizeof(heap_header_t) + sizeof(heap_footer_t))) {
        /* this hole cannot afford of another allocation,
         * so we cannot split it into 2 hole */
        size += orig_hole_size - new_size;
        /* we will act like the size requested is orig_hole_size */
        new_size = orig_hole_size;
        /* after this orig_hole_size - new_size should be 0
         * if it isn't, it implies that this hole can be split */
    }

    /* if the data need to be page-aligned,
     * we would do it now and make a new hole in front of our block */
    if (page_align && (orig_hole_pos & 0x00000FFF)) {
        /* remember that we need to align the actual data instead of head metadata */
        uint32_t new_location = orig_hole_pos
                - (orig_hole_pos & 0xFFF) + 0x1000 - sizeof(heap_header_t);
        /* this is actually the new hole, before our page-aligned hole */
        heap_header_t *hole_header = (heap_header_t*)orig_hole_pos;
        hole_header -> size = 0x1000 - (orig_hole_pos & 0xFFF) - sizeof(heap_header_t);
        hole_header -> magic = HEAP_MAGIC;
        hole_header -> is_hole = 1;
        heap_footer_t *hole_footer =
                (heap_footer_t*)((uint32_t)new_location - sizeof(heap_footer_t));
        hole_footer -> magic = HEAP_MAGIC;
        hole_footer -> header = hole_header;
        orig_hole_pos = new_location;
        orig_hole_size = orig_hole_size - (hole_header -> size);
    } else {
        /* the old hole is used as a block, so we delete it in index */
        remove_ordered_array(iterator, &heap -> index);
    }

    heap_header_t *block_header = (heap_header_t*)orig_hole_pos;
    block_header -> magic = HEAP_MAGIC;
    block_header -> is_hole = 0;
    block_header -> size = new_size;

    heap_footer_t *block_footer =
            (heap_footer_t*) (orig_hole_pos + sizeof(heap_header_t) + size);
    block_footer -> magic = HEAP_MAGIC;
    block_footer -> header = block_header;

    if (orig_hole_size - new_size > 0) {
        /* this is the split hole */
        heap_header_t *hole_header = (heap_header_t*)
                (orig_hole_pos + sizeof(heap_header_t) + size + sizeof(heap_footer_t));
        hole_header -> magic = HEAP_MAGIC;
        hole_header -> is_hole = 1;
        hole_header -> size = orig_hole_size - new_size;

        heap_footer_t *hole_footer = (heap_footer_t*)
                ((uint32_t)hole_header + hole_header -> size - sizeof(heap_footer_t));
        hole_footer -> magic = HEAP_MAGIC;
        hole_footer -> header = hole_header;

        insert_ordered_array((void *)hole_header, &heap -> index);
    }

    return (void *)((uint32_t)block_header + sizeof(heap_header_t));
}

void free(void *p, heap_t *heap) {
    if (p == 0) return;

    heap_header_t *header = (heap_header_t*)((uint32_t)p - sizeof(heap_header_t));
    heap_footer_t *footer = (heap_footer_t*)
            ((uint32_t)header + header -> size - sizeof(heap_footer_t));

    assert(header -> magic == HEAP_MAGIC,
            "Heap free() error! Header's magic number don't match! Memory is disrupted!");
    assert(footer -> magic == HEAP_MAGIC,
           "Heap free() error! Footer's magic number don't match! Memory is disrupted!");

    header -> is_hole = 1;
    uint8_t do_add = 1; /* do we want to add this hole to the heap index? */

    /* unify left */
    heap_footer_t *test_footer = (heap_footer_t*)((uint32_t)header - sizeof(heap_footer_t));
    if (test_footer->magic == HEAP_MAGIC && test_footer -> header -> is_hole == 1) {
        /* we can merge these two hole together */
        uint32_t cache_size = header -> size;
        header = test_footer -> header;
        footer -> header = header;
        header -> size += cache_size;
        do_add = 0;
    }

    /* unify right */
    heap_header_t *test_header = (heap_header_t*)((uint32_t)footer + sizeof(heap_footer_t));
    if ((test_header -> magic = HEAP_MAGIC) && (test_header -> is_hole == 1)) {
        header -> size += test_header -> size;
        test_footer = (heap_footer_t*)
                ((uint32_t)test_header + test_header -> size - sizeof(heap_footer_t));
        test_footer -> header = header;
        footer = test_footer;

        /* if the right hole is in index, we will remove it */
        uint32_t iterator = 0;
        while ((iterator < heap -> index.size) &&
            (lookup_ordered_array(iterator, &heap -> index) != (void *)test_header))
            iterator++;

        assert(iterator < heap -> index.size,
                "Heap free() error! Iterator our of the index bound!");

        remove_ordered_array(iterator, &heap -> index);
    }

    /* if the footer address is the end address, we will contract the heap */
    if ((uint32_t)footer + sizeof(heap_footer_t) == heap -> end_address) {
        uint32_t old_length = (heap -> end_address) - (heap -> start_address);
        uint32_t new_length = contract((uint32_t)header - heap -> start_address, heap);

        /* as the contract is aligned, we cannot be sure that the hole will be
         * reclaim to the OS */
        if (header -> size > (old_length - new_length)) {
            /* this hole still exists */
            header -> size = old_length - new_length;
            footer = (heap_footer_t*)
                    ((uint32_t)header + header -> size - sizeof(heap_footer_t));
            footer -> magic = HEAP_MAGIC;
            footer -> header = header;
        } else {
            uint32_t iterator = 0;
            while ((iterator < heap -> index.size) &&
                (lookup_ordered_array(iterator, &heap -> index) != (void *)header)) {
                iterator++;
            }
            if (iterator < heap -> index.size)
                remove_ordered_array(iterator, &heap -> index);
        }
    }

    if (do_add == 1)
        insert_ordered_array((void *)header, &heap -> index);
}
