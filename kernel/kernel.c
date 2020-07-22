//
// Created by Soptq on 2020/7/21.
//

#include "../cpu/isr.h"
#include "../cpu/timer.h"
#include "../drivers/keyboard.h"

void main() {
    isr_install();    /* Test the interrupts */
    asm volatile("sti");
//    init_timer(50);
    init_keyboard();
}

