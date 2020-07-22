//
// Created by Soptq on 2020/7/21.
//

#ifndef SIM_OS_IDT_H
#define SIM_OS_IDT_H

#include "type.h"

#define KERNEL_CS 0x08

typedef struct {    /* 8 bytes */
    uint16_t low_offset; /* lower 16 bits handler address */
    uint16_t sel;        /* kernel segment selector, this is pointed to GDT. */
    uint8_t always0;
    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */
    uint8_t flags;
    uint16_t high_offset; /* higher 16 handler address */
} __attribute__((packed)) idt_gate_t;   /* __attribute__((packed)) : cancel the align of GCC */

typedef struct {
    uint16_t limit;      /* similar to GDT, length - 1 */
    uint32_t base;       /* IDT start address */
} __attribute__((packed)) idt_register_t;

#define IDT_ENTRIES 256
idt_gate_t idt[IDT_ENTRIES];
idt_register_t idt_reg;

void set_idt_gate(int32_t  n, uint32_t handler, uint16_t sel, uint8_t flags);
void set_idt();

#endif //SIM_OS_IDT_H
