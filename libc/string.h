//
// Created by Soptq on 2020/7/22.
//

#ifndef SIM_OS_STRING_H
#define SIM_OS_STRING_H

#include "../cpu/type.h"

void int_to_ascii(int32_t  n, int8_t* str);
void hex_to_ascii(int32_t  n, int8_t* str);


void strrev(int8_t *str);
int32_t  strlen(const int8_t* s);
void append(int8_t* s, int8_t n);
void backspace(int8_t* s);
int32_t  strcmp(int8_t* s1, int8_t* s2);

#endif //SIM_BOOTLOADER_STRING_H
