//
// Created by Soptq on 2020/7/22.
//

#include "page.h"
#include "../kernel/kernel.h"
#include "../drivers/screen.h"
#include "../libc/assert.h"

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

/* kernel's page directory */
page_directory_t  *kernel_directory = 0;

/* current page directory */
page_directory_t *current_directory = 0;


/* the bitset of the frames */
uint32_t *frames;
uint32_t nframes;

/* set a bit in the frames bitset */
static void set_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr >> 12;  /* page unit is 4k (12 bits) */
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);
}

/* reset a bit in the frames bitset */
static void clear_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr >> 12;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    frames[idx] &= ~(0x1 << off);
}

/* test if a bit is set */
static uint32_t test_frame(uint32_t frame_addr) {
    uint32_t frame = frame_addr >> 12;
    uint32_t idx = INDEX_FROM_BIT(frame);
    uint32_t off = OFFSET_FROM_BIT(frame);
    return (frames[idx] & (0x1 << off));
}

/* find the first free frame */
static uint32_t first_frame() {
    for (size_t i = 0; i < INDEX_FROM_BIT(nframes); ++i) {
        if (frames[i] == 0xFFFFFFFF) continue;
        for (size_t j = 0; j < 32; ++j) {
            if (!(frames[i] & (0x1 << j))) return i * 4 * 8 + j;
        }
    }
    return 0x0;
}


void alloc_frame(page_t *page, int is_kernel, int is_writeable) {
    if (page -> frame != 0) {
        return;     /* allocated frame */
    }

    uint32_t idx = first_frame();

    assert(idx != (uint32_t)-1, "0x00011");
    set_frame(idx << 12);
    page -> present = 1;
    page -> rw = (is_writeable) ? 1 : 0;
    page -> user = (is_kernel) ? 0 : 1;
    page -> frame = idx;
}

void free_frame(page_t *page) {
    if (page -> frame == 0) {
        return;     /* not allocated frame */
    }

    uint32_t frame = page -> frame;
    clear_frame(frame);
    page -> frame = 0x0;
}

void initialize_paging() {
    uint32_t mem_end_page = 0x1000000;  /* assume our machine's memory is 16MB big */

    nframes = mem_end_page >> 12;   /* compute how many frames we need for out memory*/
    frames = (uint32_t*) kmalloc(INDEX_FROM_BIT(nframes));  /* malloc memory for them */
    memset(frames, 0, INDEX_FROM_BIT(nframes)); /* initialize them to 0, all free */

    /* make a page directory */
    kernel_directory = (page_directory_t*) kmalloc_a(sizeof(page_directory_t));
    memset(kernel_directory, 0, sizeof(page_directory_t));
    current_directory = kernel_directory;

    int i = 0;
    for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000)
        get_page(i, 1, kernel_directory);

    i = 0;
    while (i < free_mem_addr + 0x1000) {
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;
    }

    for (i = KHEAP_START; i < KHEAP_START + KHEAP_INITIAL_SIZE; i += 0x1000) {
        alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
    }

    register_interrupt_handler(14, page_fault_callback);

    switch_page_directory(kernel_directory, 1);

    kheap = create_heap(KHEAP_START, KHEAP_START + KHEAP_INITIAL_SIZE, 0x8ff000, 0, 0);
}

void enable_page() {
    uint32_t cr0;
    asm volatile ("mov %%cr0, %0": "=r"(cr0));  /* get cr0 */
    cr0 |= 0x80000000;  /* enable paging */
    asm volatile ("mov %0, %%cr0": : "r"(cr0));
}

void switch_page_directory(page_directory_t *dir, uint8_t force_enable_paging) {
    current_directory = dir;
    asm volatile ("mov %0, %%cr3": : "r"(&(current_directory -> tables_physical)));
    if (force_enable_paging) {
        enable_page();
    }
}

/**
 * Get the page address of given address.
 */
page_t *get_page(uint32_t address, int make, page_directory_t* dir) {
    address >>= 12;  /* convert the address to frame */
    uint32_t table_idx = address / 1024;    /* 1024 tables, each contains 1024 pages */
    if (dir -> tables[table_idx]) { /* this table is assigned, return page address */
        return &(dir -> tables[table_idx] -> pages[address % 1024]);
    } else {    /* no table is found */
        if (make) {     /* make a table */
            uint32_t tmp;
            /* store the malloced address to tables and tmp*/
            dir -> tables[table_idx] =
                    (page_table_t*) kmalloc_ap(sizeof(page_table_t), &tmp);
            /* init the table with 0, size 4k */
            memset(dir -> tables[table_idx], 0, 0x1000);
            dir -> tables_physical[table_idx] = tmp | 0x7;  /* 0x0111, USER, RW, PRESENT*/
            return &(dir -> tables[table_idx] ->pages[address % 1024]);
        } else {
            return 0;
        }
    }
}

void page_fault_callback(registers_t *r) {
    /* a page fault has occurred */
    /* the faulting address is stored in the CR2 register. */
    uint32_t faulting_address;
    asm("mov %%cr2, %0" : "=r" (faulting_address));

    /* parse error code */
    int present = !(r -> err_code & 0x1);   /* page not present */
    int rw = r -> err_code & 0x2;           /* write operation? */
    int user = r -> err_code & 0x4;         /* user-mode? */
    int reserved = r -> err_code & 0x8;     /* overwritten reserved bits? */
    int id = r -> err_code & 0x10;          /* instruction fetch? */

    if (present) {err_kprint("P");}
    if (rw) {err_kprint("R");}
    if (user) {err_kprint("U");}
    if (reserved) {err_kprint("R");}
    if (id) {err_kprint("I");}
    kprint(" At ");
    err_kprintln_hex(faulting_address);
    cpu_halt();
}
