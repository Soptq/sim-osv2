//
// Created by Soptq on 2020/7/22.
//

#include "mem.h"



void memory_copy(uint8_t *source, uint8_t *dest, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(uint8_t* dest, uint8_t val, uint32_t len) {
    uint8_t* temp = (uint8_t *)dest;
    for( ; len != 0; len--) *temp++ = val;
}

uint32_t free_mem_addr = 0x10000;


uint32_t kmalloc(size_t size, int aligh, uint32_t *phys_addr) {
    /* if we need to aligh the memory and it is not already aligned */
    if (aligh == 1 && (free_mem_addr & 0xFFFFF000)) {
        free_mem_addr &= 0xFFFFF000;    /* align it with a 4k unit */
        free_mem_addr += 0x1000;        /* kernel offset */
    }

    if (phys_addr) *phys_addr = free_mem_addr;

    uint32_t ret = free_mem_addr;
    free_mem_addr += size;
    return ret;
}

