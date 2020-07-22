//
// Created by Soptq on 2020/7/22.
//

#ifndef SIM_OS_MEM_H
#define SIM_OS_MEM_H

#include "../cpu/type.h"

void *memset(void *str, int c, size_t n);
void *memcpy(void *dest, const void* src, size_t n);

uint32_t _kmalloc(size_t size, int8_t aligh, uint32_t *phys_addr);
uint32_t kmalloc_a(size_t size);
uint32_t kmalloc_p(size_t size, uint32_t *phys_addr);
uint32_t kmalloc_ap(size_t size, uint32_t *phys_addr);
uint32_t kmalloc(size_t size);

extern uint32_t free_mem_addr;

#endif //SIM_OS_MEM_H
