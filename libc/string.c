//
// Created by Soptq on 2020/7/22.
//

#include "string.h"

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
    return --i;
}

void append(s8* s, s8 n) {
    s32 len = strlen(s);
    *(s + len++) = n;
    *(s + len) = '\0';
}

void backspace(s8* s) {
    s32 len = strlen(s);
    s[len - 1] = '\0';
}

s32 strcmp(s8* s1, s8* s2) {
    s32 i;
    for (i = 0; *(s1 + i) == *(s2 + i); ++i) {
        if (s1[i] == '\0') return 0;
    }
    return *(s1 + i) - *(s2 + i);
}