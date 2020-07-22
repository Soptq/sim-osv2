//
// Created by Soptq on 2020/7/21.
//

#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../kernel/util.h"
#include "../drivers/ports.h"

isr_t interrupt_handlers[256];

void isr_install() {
    set_idt_gate(0, (u32)isr0, KERNEL_CS, 0x8E);
    set_idt_gate(1, (u32)isr1, KERNEL_CS, 0x8E);
    set_idt_gate(2, (u32)isr2, KERNEL_CS, 0x8E);
    set_idt_gate(3, (u32)isr3, KERNEL_CS, 0x8E);
    set_idt_gate(4, (u32)isr4, KERNEL_CS, 0x8E);
    set_idt_gate(5, (u32)isr5, KERNEL_CS, 0x8E);
    set_idt_gate(6, (u32)isr6, KERNEL_CS, 0x8E);
    set_idt_gate(7, (u32)isr7, KERNEL_CS, 0x8E);
    set_idt_gate(8, (u32)isr8, KERNEL_CS, 0x8E);
    set_idt_gate(9, (u32)isr9, KERNEL_CS, 0x8E);
    set_idt_gate(10, (u32)isr10, KERNEL_CS, 0x8E);
    set_idt_gate(11, (u32)isr11, KERNEL_CS, 0x8E);
    set_idt_gate(12, (u32)isr12, KERNEL_CS, 0x8E);
    set_idt_gate(13, (u32)isr13, KERNEL_CS, 0x8E);
    set_idt_gate(14, (u32)isr14, KERNEL_CS, 0x8E);
    set_idt_gate(15, (u32)isr15, KERNEL_CS, 0x8E);
    set_idt_gate(16, (u32)isr16, KERNEL_CS, 0x8E);
    set_idt_gate(17, (u32)isr17, KERNEL_CS, 0x8E);
    set_idt_gate(18, (u32)isr18, KERNEL_CS, 0x8E);
    set_idt_gate(19, (u32)isr19, KERNEL_CS, 0x8E);
    set_idt_gate(20, (u32)isr20, KERNEL_CS, 0x8E);
    set_idt_gate(21, (u32)isr21, KERNEL_CS, 0x8E);
    set_idt_gate(22, (u32)isr22, KERNEL_CS, 0x8E);
    set_idt_gate(23, (u32)isr23, KERNEL_CS, 0x8E);
    set_idt_gate(24, (u32)isr24, KERNEL_CS, 0x8E);
    set_idt_gate(25, (u32)isr25, KERNEL_CS, 0x8E);
    set_idt_gate(26, (u32)isr26, KERNEL_CS, 0x8E);
    set_idt_gate(27, (u32)isr27, KERNEL_CS, 0x8E);
    set_idt_gate(28, (u32)isr28, KERNEL_CS, 0x8E);
    set_idt_gate(29, (u32)isr29, KERNEL_CS, 0x8E);
    set_idt_gate(30, (u32)isr30, KERNEL_CS, 0x8E);
    set_idt_gate(31, (u32)isr31, KERNEL_CS, 0x8E);

    /**
     * Remap PIC
     * Master PIC: command: 0x20, data: 0x21
     * Slave PIC: command: 0xA0, data: 0xA1
     * Detail: https://wiki.osdev.org/PIC
     */
    /* ICW 1 */
    port_byte_out(0x20, 0x11);  /* 0x11: the initialize coommand */
    port_byte_out(0xA0, 0x11);
    /* ICW 2: vector offset */
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);
    /* ICW 3: wire of master/slaves */
    port_byte_out(0x21, 0x04);  /* 0x00000100 */
    port_byte_out(0xA1, 0x02);  /* 0x00000010 */
    /* ICW 4: additional information about the environment */
    port_byte_out(0x21, 0x01);  /* 0x01: 8086/88 mode */
    port_byte_out(0xA1, 0x01);

    /* MASK */
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);

    /* install irq */
    set_idt_gate(32, (u32)irq0, KERNEL_CS, 0x8E);
    set_idt_gate(33, (u32)irq1, KERNEL_CS, 0x8E);
    set_idt_gate(34, (u32)irq2, KERNEL_CS, 0x8E);
    set_idt_gate(35, (u32)irq3, KERNEL_CS, 0x8E);
    set_idt_gate(36, (u32)irq4, KERNEL_CS, 0x8E);
    set_idt_gate(37, (u32)irq5, KERNEL_CS, 0x8E);
    set_idt_gate(38, (u32)irq6, KERNEL_CS, 0x8E);
    set_idt_gate(39, (u32)irq7, KERNEL_CS, 0x8E);

    set_idt_gate(40, (u32)irq8, KERNEL_CS, 0x8E);
    set_idt_gate(41, (u32)irq9, KERNEL_CS, 0x8E);
    set_idt_gate(42, (u32)irq10, KERNEL_CS, 0x8E);
    set_idt_gate(43, (u32)irq11, KERNEL_CS, 0x8E);
    set_idt_gate(44, (u32)irq12, KERNEL_CS, 0x8E);
    set_idt_gate(45, (u32)irq13, KERNEL_CS, 0x8E);
    set_idt_gate(46, (u32)irq14, KERNEL_CS, 0x8E);
    set_idt_gate(47, (u32)irq15, KERNEL_CS, 0x8E);

    set_idt(); // Load with ASM
}


/* To print the message which defines every exception */
char *exception_messages[] = {
        "Division By Zero",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Into Detected Overflow",
        "Out of Bounds",
        "Invalid Opcode",
        "No Coprocessor",

        "Double Fault",
        "Coprocessor Segment Overrun",
        "Bad TSS",
        "Segment Not Present",
        "Stack Fault",
        "General Protection Fault",
        "Page Fault",
        "Unknown Interrupt",

        "Coprocessor Fault",
        "Alignment Check",
        "Machine Check",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",

        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved"
};


void isr_handler(registers_t r) {
    char int_no[3];  /* 0-255 */
    char error_code[11];
    int_to_ascii(r.int_no, int_no);
    int_to_ascii(r.err_code, error_code);
    kprint("Received Interrupt! ISR NUM ");
    err_kprint(int_no);
    kprint(" Error Code ");
    err_kprintln(error_code);
    kprint("Detail: ");
    err_kprintln(exception_messages[r.int_no]);
    kprintln("");
}

void register_interrupt_handler(u8 n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
    if (r.int_no >= 40) {
        /* this is triggered by the slave PIC */
        port_byte_out(0xA0, 0x20);  /* End-of-interrupt command code */
    }
    port_byte_out(0x20, 0x20);

    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}


