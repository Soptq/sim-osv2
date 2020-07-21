//
// Created by Soptq on 2020/7/21.
//

#ifndef SIM_OS_IDT_H
#define SIM_OS_IDT_H

#include "types.h"

#define KERNEL_CS 0x08

typedef struct {    /* 8 bytes */
    u16 low_offset; /* lower 16 bits handler address */
    u16 sel;        /* kernel segment selector, this is pointed to GDT. */
    u8 always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    u8 flags;
    u16 high_offset; /* higher 16 handler address */
} __attribute__((packed)) idt_gate_t;   /* __attribute__((packed)) : cancel the align of GCC */

typedef struct {
    u16 limit;      /* similar to GDT, length - 1 */
    u32 base;       /* IDT start address */
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(s32 n, u32 handler, u16 sel, u8 flags);
void set_idt();

#endif //SIM_OS_IDT_H
