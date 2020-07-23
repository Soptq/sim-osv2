//
// Created by Soptq on 2020/7/21.
//

#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "ports.h"
#include "timer.h"
#include "../drivers/keyboard.h"
#include "../libc/string.h"

isr_t interrupt_handlers[256];

void isr_install() {
    set_idt_gate(0, (uint32_t)isr0, KERNEL_CS, 0x8E);
    set_idt_gate(1, (uint32_t)isr1, KERNEL_CS, 0x8E);
    set_idt_gate(2, (uint32_t)isr2, KERNEL_CS, 0x8E);
    set_idt_gate(3, (uint32_t)isr3, KERNEL_CS, 0x8E);
    set_idt_gate(4, (uint32_t)isr4, KERNEL_CS, 0x8E);
    set_idt_gate(5, (uint32_t)isr5, KERNEL_CS, 0x8E);
    set_idt_gate(6, (uint32_t)isr6, KERNEL_CS, 0x8E);
    set_idt_gate(7, (uint32_t)isr7, KERNEL_CS, 0x8E);
    set_idt_gate(8, (uint32_t)isr8, KERNEL_CS, 0x8E);
    set_idt_gate(9, (uint32_t)isr9, KERNEL_CS, 0x8E);
    set_idt_gate(10, (uint32_t)isr10, KERNEL_CS, 0x8E);
    set_idt_gate(11, (uint32_t)isr11, KERNEL_CS, 0x8E);
    set_idt_gate(12, (uint32_t)isr12, KERNEL_CS, 0x8E);
    set_idt_gate(13, (uint32_t)isr13, KERNEL_CS, 0x8E);
    set_idt_gate(14, (uint32_t)isr14, KERNEL_CS, 0x8E);
    set_idt_gate(15, (uint32_t)isr15, KERNEL_CS, 0x8E);
    set_idt_gate(16, (uint32_t)isr16, KERNEL_CS, 0x8E);
    set_idt_gate(17, (uint32_t)isr17, KERNEL_CS, 0x8E);
    set_idt_gate(18, (uint32_t)isr18, KERNEL_CS, 0x8E);
    set_idt_gate(19, (uint32_t)isr19, KERNEL_CS, 0x8E);
    set_idt_gate(20, (uint32_t)isr20, KERNEL_CS, 0x8E);
    set_idt_gate(21, (uint32_t)isr21, KERNEL_CS, 0x8E);
    set_idt_gate(22, (uint32_t)isr22, KERNEL_CS, 0x8E);
    set_idt_gate(23, (uint32_t)isr23, KERNEL_CS, 0x8E);
    set_idt_gate(24, (uint32_t)isr24, KERNEL_CS, 0x8E);
    set_idt_gate(25, (uint32_t)isr25, KERNEL_CS, 0x8E);
    set_idt_gate(26, (uint32_t)isr26, KERNEL_CS, 0x8E);
    set_idt_gate(27, (uint32_t)isr27, KERNEL_CS, 0x8E);
    set_idt_gate(28, (uint32_t)isr28, KERNEL_CS, 0x8E);
    set_idt_gate(29, (uint32_t)isr29, KERNEL_CS, 0x8E);
    set_idt_gate(30, (uint32_t)isr30, KERNEL_CS, 0x8E);
    set_idt_gate(31, (uint32_t)isr31, KERNEL_CS, 0x8E);

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
    set_idt_gate(32, (uint32_t)irq0, KERNEL_CS, 0x8E);
    set_idt_gate(33, (uint32_t)irq1, KERNEL_CS, 0x8E);
    set_idt_gate(34, (uint32_t)irq2, KERNEL_CS, 0x8E);
    set_idt_gate(35, (uint32_t)irq3, KERNEL_CS, 0x8E);
    set_idt_gate(36, (uint32_t)irq4, KERNEL_CS, 0x8E);
    set_idt_gate(37, (uint32_t)irq5, KERNEL_CS, 0x8E);
    set_idt_gate(38, (uint32_t)irq6, KERNEL_CS, 0x8E);
    set_idt_gate(39, (uint32_t)irq7, KERNEL_CS, 0x8E);

    set_idt_gate(40, (uint32_t)irq8, KERNEL_CS, 0x8E);
    set_idt_gate(41, (uint32_t)irq9, KERNEL_CS, 0x8E);
    set_idt_gate(42, (uint32_t)irq10, KERNEL_CS, 0x8E);
    set_idt_gate(43, (uint32_t)irq11, KERNEL_CS, 0x8E);
    set_idt_gate(44, (uint32_t)irq12, KERNEL_CS, 0x8E);
    set_idt_gate(45, (uint32_t)irq13, KERNEL_CS, 0x8E);
    set_idt_gate(46, (uint32_t)irq14, KERNEL_CS, 0x8E);
    set_idt_gate(47, (uint32_t)irq15, KERNEL_CS, 0x8E);

    /**
     * SYSTEM DEFINE INTERRUPT
     */


    set_idt(); // Load with ASM
}


/* To print the message which defines every exception */
/* CPU-dedicated interrupts are shown below.*/
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


void isr_handler(registers_t *r) {
    char int_no[3];  /* 0-255 */
    char error_code[11];
    int_to_ascii(r -> int_no, int_no);
    int_to_ascii(r -> err_code, error_code);
    err_kprint("Received Interrupt!");
    kprint(" ISR NUM ");
    err_kprint(int_no);
    kprint(" Error Code ");
    err_kprintln(error_code);
    kprint("Detail: ");
    err_kprintln(exception_messages[r -> int_no]);
    kprintln("");

    if (interrupt_handlers[r -> int_no] != 0) {
        isr_t handler = interrupt_handlers[r -> int_no];
        handler(r);
    }
}

void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t *r) {
    if (r -> int_no >= 40) {
        /* this is triggered by the slave PIC */
        port_byte_out(0xA0, 0x20);  /* End-of-interrupt command code */
    }
    port_byte_out(0x20, 0x20);

    if (interrupt_handlers[r -> int_no] != 0) {
        isr_t handler = interrupt_handlers[r -> int_no];
        handler(r);
    }
}

void irq_install() {
    asm volatile ("sti");
    init_timer(50);
    init_keyboard();
}


