//
// Created by Soptq on 2020/7/21.
//

/**
 * Read a byte from the specified port
 */

#include "ports.h"

uint8_t port_byte_in (uint16_t port) {
    uint8_t result;
    /* inline assembly
     * syntax:
     * asm("assembly" : "=r" (val1) : "r" (val2))
     * execute provided assembly, making val1 be the output variable and val2 be the input variable
     * =r / r is utilized to specify the register. E.g. a stands for eax, ax, al.
     * */

    /* here we mean:
     * read I/O from dx to al, result will be referred as al and port will be referred as dx.
     * */
    asm("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_byte_out (uint16_t port, uint8_t data) {
    /* here we mean:
     * output data in al to I/O dx, where as data is stored in a and port is stored in dx.
     * */
    asm("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_word_in (uint16_t port) {
    uint16_t result;      /* short takes 2 bytes */
    asm("in %%dx, %%ax" : "=a" (result): "d" (port));
    return result;
}

void port_word_out (uint16_t port, uint16_t data) {
    asm("out %%ax, %%dx" : : "a" (data), "d" (port));
}
