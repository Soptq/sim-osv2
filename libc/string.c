//
// Created by Soptq on 2020/7/22.
//

#include "string.h"


void str_clear(char* str) {
    str[0] = '\0';
}

/**
 * convert integer to string reversely
 * 123 => "321"; -123 => "321-"
 *
 * @param n : the integer;
 * @param str : the string container;
 */
void int_to_ascii(int32_t  n, char* str) {
    str_clear(str);
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

void hex_to_ascii(int32_t  n, char* str) {
    str_clear(str);
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


char c_int_to_hex(int index) {
    if (0 <= index && index < 10) {
        return (char)(index + '0');
    } else if (index < 16) {
        return (char)(index - 10 + 'a');
    } else {
        return '?';
    }
}




void int_to_hexstr(uint32_t n, char* str) {
    str_clear(str);
    if (n == 0) {
        append(str, '0');
    }
    while (n != 0) {
        append(str, c_int_to_hex(n % 16));
        n /= 16;
    }
    append(str, 'x');
    append(str, '0');
    strrev(str);
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

int32_t strlen(const char* s) {
    int32_t  i = 0;
    while (*(s + i++) != '\0');
    return --i;
}


char* strcpy(char* destination, const char* source) {
    if (destination == NULL)
        return NULL;

    char *ptr = destination;
    while (*source != '\0') {
        *destination = *source;
        destination++;
        source++;
    }
    *destination = '\0';
    return ptr;
}


void append(char* s, char n) {
    int32_t  len = strlen(s);
    *(s + len++) = n;
    *(s + len) = '\0';
}

void insert(char* s, char n, int32_t i) {
    int32_t index = i;
    int32_t len = strlen(s);
    char tmp = s[index], tmp2;
    s[index] = n;
    while (index < len) {
        tmp2 = s[index + 1];
        s[index + 1] = tmp;
        tmp = tmp2;
        index += 1;
    }
    s[index + 1] = 0;
}

void backspace(char* s) {
    int32_t  len = strlen(s);
    s[len - 1] = '\0';
}


void c_pop(char* s, int32_t i) {
    if (i < 0) {
        return;
    }
    int32_t index = i;
    int32_t len = strlen(s);
    len--;
    char tmp = s[len], tmp2;
    s[len] = s[len + 1];
    while (index < len) {
        tmp2 = s[len - 1];
        s[len - 1] = tmp;
        tmp = tmp2;
        len--;
    }
}

int32_t strcmp(char* s1, char* s2) {
    int32_t  i;
    for (i = 0; *(s1 + i) == *(s2 + i); ++i) {
        if (s1[i] == '\0') return 0;
    }
    return *(s1 + i) - *(s2 + i);
}