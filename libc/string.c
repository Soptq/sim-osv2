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
void int_to_ascii(int32_t  n, int8_t* str) {
    int32_t  sign, index = 0;
    if ((sign = n) < 0)
        n = -n;
    do {
        *(str + index++) = (int8_t)(n % 10 + (int32_t )'0');
    } while ((n /= 10) > 0);

    if (sign < 0) {
        *(str + index++) = '-';
    }
    *(str + index) = '\0';

    strrev(str);
}

void hex_to_ascii(int32_t  n, int8_t* str) {
    append(str, '0');
    append(str, 'x');
    int8_t zeros = 0;

    int32_t  tmp;
    for (int32_t  i = 28; i > 0; i -= 4) {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && zeros == 0) continue;
        zeros = 1;
        if (tmp > 0xA) append(str, tmp - 0xA + 'a');
        else append(str, tmp + '0');
    }
}

void strrev(int8_t *str) {
    if (!str) return;       /* if str is null pointer, return*/
    int8_t *rev = str;
    while (*rev) ++rev;
    --rev;                  /* find the last char in str */

    for ( ; str < rev; ++str, --rev) {
        int8_t h = *str, t = *rev;
        *str = t;
        *rev = h;
    }
}

int32_t  strlen(const int8_t* s) {
    int32_t  i = 0;
    while (*(s + i++) != '\0');
    return --i;
}

void append(int8_t* s, int8_t n) {
    int32_t  len = strlen(s);
    *(s + len++) = n;
    *(s + len) = '\0';
}

void backspace(int8_t* s) {
    int32_t  len = strlen(s);
    s[len - 1] = '\0';
}

int32_t  strcmp(int8_t* s1, int8_t* s2) {
    int32_t  i;
    for (i = 0; *(s1 + i) == *(s2 + i); ++i) {
        if (s1[i] == '\0') return 0;
    }
    return *(s1 + i) - *(s2 + i);
}