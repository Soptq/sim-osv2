//
// Created by Soptq on 2020/7/22.
//

#ifndef SIM_OS_STRING_H
#define SIM_OS_STRING_H

#include "../cpu/type.h"

void int_to_ascii(int32_t  n, char* str);
void hex_to_ascii(int32_t  n, char* str);
void int_to_hexstr(uint32_t n, char* str);

void strrev(char* str);
int32_t  strlen(const char* s);
void append(char* s, char n);
void backspace(char* s);
int32_t  strcmp(char* s1, char* s2);

#endif //SIM_BOOTLOADER_STRING_H
