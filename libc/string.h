//
// Created by Soptq on 2020/7/22.
//

#ifndef SIM_OS_STRING_H
#define SIM_OS_STRING_H

#include "../cpu/types.h"

void int_to_ascii(s32 n, s8* str);
void strrev(s8 *str);
s32 strlen(const s8* s);
void append(s8* s, s8 n);
void backspace(s8* s);
s32 strcmp(s8* s1, s8* s2);

#endif //SIM_BOOTLOADER_STRING_H
