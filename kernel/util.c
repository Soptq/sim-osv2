//
// Created by Soptq on 2020/7/21.
//

#include "util.h"

void strrev(char *str);
s32 strlen(const s8* s);

void memory_copy(u8 *source, u8 *dest, u32 len) {
    for (s32 i = 0; i < len; i++) {
        *(dest + i) = *(source + i);
    }
}

/**
 * convert integer to string reversely
 * 123 => "321"; -123 => "321-"
 *
 * @param n : the integer;
 * @param str : the string container;
 */
void int_to_ascii(s32 n, s8* str) {
    s32 sign, index = 0;
    if ((sign = n) < 0)
        n = -n;
    do {
        *(str + index++) = (s8)(n % 10 + (s32)'0');
    } while ((n /= 10) > 0);

    if (sign < 0) {
        *(str + index++) = '-';
    }
    *(str + index) = '\0';

    strrev(str);
}

void strrev(s8 *str) {
    if (!str) return;       /* if str is null pointer, return*/
    s8 *rev = str;
    while (*rev) ++rev;
    --rev;                  /* find the last char in str */

    for ( ; str < rev; ++str, --rev) {
        s8 h = *str, t = *rev;
        *str = t;
        *rev = h;
    }
}

s32 strlen(const s8* s) {
    s32 i = 0;
    while (*(s + i++) != '\0');
    return i;
}