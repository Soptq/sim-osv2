//
// Created by Soptq on 2020/7/22.
//

#include "mem.h"

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

uint32_t free_mem_addr = 0x1000000;

uint32_t _kmalloc(size_t size, int8_t aligh, uint32_t *phys_addr) {
    /* if we need to aligh the memory and it is not already aligned */
    if (aligh == 1 && (free_mem_addr & 0x00000FFF)) {
        free_mem_addr &= 0xFFFFF000;    /* align it with a 4k unit */
        free_mem_addr += 0x1000;        /* kernel offset */
    }

    if (phys_addr) *phys_addr = free_mem_addr;

    uint32_t ret = free_mem_addr;
    free_mem_addr += size;
    return ret;
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

