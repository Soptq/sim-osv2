//
// Created by Soptq on 2020/7/21.
//

#include "idt.h"

void set_idt_gate(int32_t  n, uint32_t handler, uint16_t sel, uint8_t flags) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = sel;
    idt[n].always0 = 0;
    idt[n].flags = flags; /* 0x10001110*/
    idt[n].high_offset = high_16(handler);
}

void set_idt() {
    idt_reg.base = (uint32_t) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    /* volatile: no optimization for GCC */
    asm volatile("lidtl (%0)" : : "r" (&idt_reg));
}