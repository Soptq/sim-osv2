//
// Created by Soptq on 2020/7/22.
//

#ifndef SIM_OS_MEM_H
#define SIM_OS_MEM_H

#include "../cpu/types.h"

void memory_copy(u8 *source, u8 *dest, u32 len);
void memory_set(u8* dest, u8 val, u32 len);

#endif //SIM_BOOTLOADER_MEM_H
