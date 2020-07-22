//
// Created by Soptq on 2020/7/21.
//

#ifndef SIM_OS_UTIL_H
#define SIM_OS_UTIL_H

#include "../cpu/types.h"

void memory_copy(u8 *source, u8 *dest, u32 len);
void int_to_ascii(s32 n, s8* str);
s32 strlen(const s8* s);
#endif //SIM_OS_UTIL_H
