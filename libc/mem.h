//
// Created by Soptq on 2020/7/22.
//

#ifndef SIM_OS_MEM_H
#define SIM_OS_MEM_H

#include "../cpu/type.h"
#include "../libc/ordered_array.h"


#define FREE_MEM_ADDR           0x3000000

#define KHEAP_START             0x100000
#define KHEAP_INITIAL_SIZE      0x100000
#define HEAP_INDEX_SIZE         0x2000
#define HEAP_MAGIC              0x50942FAB
#define HEAP_MIN_SIZE           0x10000

typedef struct {
    uint32_t magic;     /* magic number, used for error checking */
    uint8_t is_hole;    /* indicate if it is avaiable */
    uint32_t size;      /* size of the block */
} heap_header_t;

typedef struct {
    uint32_t magic;     /* validation */
    heap_header_t *header;  /* point to the head of this heap */
} heap_footer_t;

typedef struct {
    ordered_array_t index;
    uint32_t start_address; /* the start of our allocated space */
    uint32_t end_address;   /* the end of our allocated space, may be expanded up to max addres */
    uint32_t max_address;   /* the maximum address the heap can be expanded to */
    uint8_t supervisor;     /* supervisor-only? */
    uint8_t readonly;       /* read-only? */
} heap_t;

void *memset(void *str, int c, size_t n);
void *memcpy(void *dest, const void* src, size_t n);

/**
 * below 5 memory allocator is used for kernel to initialize basic infrastructure
 * so these allocated memory currently won't be freed.
 * Maybe in the future I will make them free-able
 */

extern uint32_t free_mem_addr;
extern heap_t *kheap;

uint32_t _kmalloc(size_t size, int8_t align, uint32_t *phys_addr);
uint32_t kmalloc_a(size_t size);
uint32_t kmalloc_p(size_t size, uint32_t *phys_addr);
uint32_t kmalloc_ap(size_t size, uint32_t *phys_addr);
uint32_t kmalloc(size_t size);
void kfree(void *p);

/**
 * Kernel heap;
 */

heap_t *create_heap(uint32_t start, uint32_t end, uint32_t max, uint8_t supervisor, uint8_t readonly);
void *alloc(uint32_t size, uint8_t page_align, heap_t *heap);
void free(void *p, heap_t *heap);

#endif //SIM_OS_MEM_H
