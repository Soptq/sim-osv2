//
// Created by Soptq on 2020/7/22.
//

#ifndef SIM_OS_MEM_H
#define SIM_OS_MEM_H

#include "../cpu/type.h"

void memory_copy(uint8_t *source, uint8_t *dest, uint32_t len);
void memory_set(uint8_t* dest, uint8_t val, uint32_t len);

uint32_t kmalloc(size_t size, int aligh, uint32_t *phys_addr);

#endif //SIM_BOOTLOADER_MEM_H
