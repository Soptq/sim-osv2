//
// Created by Soptq on 2020/7/21.
//

void memory_copy(char *source, char *dest, int nbytes) {
    for (int i = 0; i < nbytes; i++) {
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
void int_to_ascii(int n, char* str) {
    int sign;
    if ((sign = n) < 0)
        n = -n;
    do {
        *(str++) = (char)(n % 10 + (int)'0');
    } while ((n /= 10) > 0);

    if (sign < 0) *(str++) = '-';
    *str = '\0';
}