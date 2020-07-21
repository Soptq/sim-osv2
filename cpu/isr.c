//
// Created by Soptq on 2020/7/21.
//

#include "isr.h"
#include "idt.h"
#include "../drivers/screen.h"
#include "../kernel/util.h"

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
