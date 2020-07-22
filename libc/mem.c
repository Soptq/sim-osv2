//
// Created by Soptq on 2020/7/22.
//

#include "mem.h"

void memory_copy(u8 *source, u8 *dest, u32 len) {
    for (u32 i = 0; i < len; i++) {
        *(dest + i) = *(source + i);
    }
}

void memory_set(u8* dest, u8 val, u32 len) {
    u8* temp = (u8 *)dest;
    for( ; len != 0; len--) *temp++ = val;
}