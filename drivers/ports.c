//
// Created by Soptq on 2020/7/21.
//

/**
 * Read a byte from the specified port
 */

#include "ports.h"

u8 port_byte_in (u16 port) {
    u8 result;
    /* inline assembly
     * syntax:
     * __asm__("assembly" : "=r" (val1) : "r" (val2))
     * execute provided assembly, making val1 be the output variable and val2 be the input variable
     * =r / r is utilized to specify the register. E.g. a stands for eax, ax, al.
     * */

    /* here we mean:
     * read I/O from dx to al, result will be referred as al and port will be referred as dx.
     * */
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out (u16 port, u8 data) {
    /* here we mean:
     * output data in al to I/O dx, where as data is stored in a and port is stored in dx.
     * */
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

u16 port_word_in (u16 port) {
    u16 result;      /* short takes 2 bytes */
    __asm__("in %%dx, %%ax" : "=a" (result): "d" (port));
    return result;
}

void port_word_out (u16 port, u16 data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
