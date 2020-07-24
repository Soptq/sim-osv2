//
// Created by Soptq on 2020/7/24.
//

#include "mouse.h"
#include "../cpu/ports.h"
#include "../libc/function.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"

mouse_position_t mouse_pos;

static uint8_t buffer[3] = {0};
static uint8_t offset = 0;

static void mouse_callback(registers_t *t) {
    UNUSED(t);
    uint8_t status = port_byte_in(0x64);
    if (!(status & 0x20))
        return;
    buffer[offset] = port_byte_in(0x60);
    offset = (offset + 1) % 3;

    if (offset == 0) {  /* get three bytes */
        if (buffer[1] != 0 || buffer[2] != 0){
            int32_t int_x, int_y;
            uint8_t bm, br, bl;
            /* Bit 4 of buffer[0] is X's sign bit
               Bit 5 of buffer[0] is Y's sign bit */
            int_x = buffer[1] - (0x100 & (buffer[0] << 4));
            int_y = buffer[2] - (0x100 & (buffer[0] << 3));
            bm = (buffer[0] >> 2) & 0x1;
            br = (buffer[0] >> 1) & 0x1;
            bl = buffer[0] & 0x1;



            mouse_pos.x += int_x;
            mouse_pos.y -= int_y;

            if (mouse_pos.x < 0)
                mouse_pos.x = 0;
            if ((mouse_pos.x >> MOUSE_SENSITIVITY) > (MAX_COLS - 1))
                mouse_pos.x = ((MAX_COLS - 1) << MOUSE_SENSITIVITY);
            if (mouse_pos.y < 0)
                mouse_pos.y = 0;
            if ((mouse_pos.y >> MOUSE_SENSITIVITY) > (MAX_ROWS - 1))
                mouse_pos.y = ((MAX_ROWS - 1) << MOUSE_SENSITIVITY);

            vga_mouse_callback(mouse_pos.x >> MOUSE_SENSITIVITY,
                    mouse_pos.y >> MOUSE_SENSITIVITY, bm, br, bl);

        }
    }
}

uint8_t mouse_read(){
    return port_byte_in(0x60);
}


void mouse_write(uint8_t a_write){
    port_byte_out(0x64, 0xD4); /* Tell the mouse we are sending a command */
    port_byte_out(0x60, a_write); /* Finally write */
}

void mouse_set_sample_rate(uint8_t sample_rate) {
    mouse_write(0xF3);
    mouse_read();
    mouse_write(sample_rate);
    mouse_read();
}

void install_mouse() {
    port_byte_out(0x64, 0xA8);  /* enable second device */
    port_byte_out(0x64, 0x20);  /* read Controller Configuration Byte */
    uint8_t status_byte = port_byte_in(0x60) | 0x2;
    port_byte_out(0x64, 0x60);  /* write Controller Configuration Byte */
    port_byte_out(0x60, status_byte);
    mouse_write(0xF6);
    mouse_read();

    /* expansion mode */
//    mouse_set_sample_rate(200);
//    mouse_set_sample_rate(100);
//    mouse_set_sample_rate(80);
//    mouse_set_sample_rate(200);
//    mouse_set_sample_rate(200);
//    mouse_set_sample_rate(80);

    mouse_write(0xF4);
    mouse_read();


    mouse_pos.x = 0;
    mouse_pos.y = 0;
    register_interrupt_handler(IRQ12, mouse_callback);
}