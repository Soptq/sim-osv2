//
// Created by Soptq on 2020/7/21.
//

#include "idt.h"

void set_idt_gate(s32 n, u32 handler, u16 sel, u8 flags) {
    idt[n].low_offset = low_16(handler);
    idt[n].sel = sel;
    idt[n].always0 = 0;
    idt[n].flags = flags; /* 0x10001110*/
    idt[n].high_offset = high_16(handler);
}

void set_idt() {
    idt_reg.base = (u32) &idt;
    idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
    /* __volatile__: no optimization for GCC */
    __asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
}