//
// Created by Soptq on 2020/7/21.
//

#include "kernel.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../cpu/page.h"
#include "../libc/string.h"

void kernel_main() {
    isr_install();
    kprintln("ISR loaded");
    irq_install();
    kprintln("IRQ loaded");
    initialize_paging();
    kprintln("Page initialized");
    kprintln("System booted");
    kprintln("");

    kprint("Type something, it will go through the kernel\n"
           "Type END to halt the CPU\n"
           "> ");
}

void user_input(char *input) {
    if (strcmp(input, "END") == 0) {
        err_kprintln("");
        err_kprintln("Stopping the CPU. Bye!");
        cpu_halt();
    }

    kprint("You said: ");
    kprint(input);
    kprint("\n> ");
}

void cpu_halt() {
    asm volatile("hlt");
}

