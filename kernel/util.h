//
// Created by Soptq on 2020/7/21.
//

#ifndef SIM_OS_UTIL_H
#define SIM_OS_UTIL_H

#include "../cpu/types.h"

void memory_copy(s8 *source, s8 *dest, s32 nbytes);
void int_to_ascii(s32 n, s8* str);
#endif //SIM_OS_UTIL_H
