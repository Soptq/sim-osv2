//
// Created by Soptq on 2020/7/21.
//

#include "kernel.h"
#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../cpu/page.h"
#include "../libc/string.h"
#include "../drivers/keyboard.h"

void kuser_prompt() {
    kprint("\n> ");
    command_row = get_cursor_row();
    command_col = get_cursor_col();
    command_col_start = command_col;
}

void kernel_main() {
    initialize_VGA();
    isr_install();
    irq_install();
    initialize_paging();
    kprintln("");

    kuser_prompt();
}

void user_input(char *input) {
    if (strcmp(input, "halt") == 0) {
        err_kprintln("");
        err_kprintln("Halting");
        cpu_halt();
    }

    if (strcmp(input, "clear") == 0) {
        clear_screen();
        kuser_prompt();
        return;
    }

    kprint("+=> ");
    kprint(input);
    kuser_prompt();
}


void cpu_halt() {
    asm volatile("hlt");
}

