//
// Created by Soptq on 2020/7/22.
//

#include "timer.h"
#include "../drivers/screen.h"
#include "isr.h"
#include "../libc/function.h"

uint32_t tick = 0;

static void timer_callback(registers_t *regs) {
    tick++;
    UNUSED(regs);
}


/**
 * init PIT (maybe 8253?)
 * 0x40:    Channel 0 data port (RW)
 * 0x41:    Channel 1 data port (RW)
 * 0x42:    Channel 2 data port (RW)
 * 0x43:    Mode/Command register (W)
 * Detail: https://wiki.osdev.org/PIT
 */
void init_timer(uint32_t freq) {
    /* Install the timer function */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Get the PIT (programmable interval timer) value */
    uint32_t divisor = 1193180 / freq;
    uint8_t low = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);

    port_byte_out(0x43, 0x36);  /* 0x00110110: Channel 0, low-high byte input, mode 3, 16-bit mode*/
    port_byte_out(0x40, low);
    port_byte_out(0x40, high);
}
