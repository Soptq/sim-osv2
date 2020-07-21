//
// Created by Soptq on 2020/7/21.
//

#include "../drivers/ports.h"

void main() {
    /* Firstly we ask VGA control register (0x3d4) for cursor position */
    /* 14 for high byte and 15 for low byte of cursor position */
    port_byte_out(0x3d4, 14);
    /* After asking, the answer is stored in VGGA data register (0x3d5) */
    int position = port_byte_in(0x3d5);
    position <<= 8;

    port_byte_out(0x3d4, 15);
    position += port_byte_in(0x3d5);

    /* each character takes 2 bytes */
    int offset_from_vga = position * 2;

    char* vga = 0xb8000;
    vga[offset_from_vga++] = 'X';
    vga[offset_from_vga++] = 0x0f;
}

