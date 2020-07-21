//
// Created by Soptq on 2020/7/21.
//

#include "util.h"

void strrev(char *str);

void memory_copy(s8 *source, s8 *dest, s32 nbytes) {
    for (s32 i = 0; i < nbytes; i++) {
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
    s32 sign;
    if ((sign = n) < 0)
        n = -n;
    do {
        *(str++) = (s8)(n % 10 + (s32)'0');
    } while ((n /= 10) > 0);

    if (sign < 0) *(str++) = '-';
    strrev(str);
    *str = '\0';
}

void strrev(char *str) {
    if (!str) return;       /* if str is null pointer, return*/
    char *rev = str;
    while (*rev) ++rev;
    --rev;                  /* find the last char in str */

    for ( ; str < rev; ++str, --rev) {
        char h = *str, t = *rev;
        *str = t;
        *rev = h;
    }
}