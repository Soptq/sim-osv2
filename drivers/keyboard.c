//
// Created by Soptq on 2020/7/22.
//

#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"
#include "../libc/string.h"

int8_t shift_pressed = 0;
int8_t capslock_pressed = 0;
int8_t escape_pressed = 0;

static int8_t key_buffer[256];
int32_t command_row = -1;
int32_t command_col = -1;
int32_t command_col_start = -1;


void escape_arrow_controller(int8_t direction_x, int8_t direction_y) {
    if (escape_pressed) {
        escape_pressed = 0;
        if (direction_x > 0 && get_cursor_col() + direction_x > MAX_COLS - 1)
            return;
        if (direction_x < 0 && get_cursor_col() + direction_x < 0)
            return;
        if (direction_y > 0 && get_cursor_row() + direction_y > MAX_ROWS - 1)
            return;
        if (direction_y < 0 && get_cursor_row() + direction_y < 0)
            return;

        set_cursor_pos(get_cursor_col() + direction_x, get_cursor_row() + direction_y);
    }
}

static void keyboard_callback(registers_t *t) {
    uint8_t scancode = port_byte_in(0x60);

    /* 'keuyp' event corresponds to the 'keydown' + 0x80
     * it may still be a scancode we haven't implemented yet, or
     * maybe a control/escape sequence */
    if (scancode <= 0x7f) {
        switch (scancode) {
            case KEYDOWN_ERROR:
                break;
            case KEYDOWN_ESC:
                break;
            case KEYDOWN_BACKSPACE:
                if (command_row == get_cursor_row())
                    if (command_col == get_cursor_col()) {
                        backspace((char *) key_buffer);
                        command_col--;
                        kbackspace();
                    } else if (command_col > get_cursor_col() &&
                        get_cursor_col() >= command_col_start) {
                        int32_t index = get_cursor_col() - command_col_start - 1;
                        if (index >= 0) {
                            c_pop((char*) key_buffer, index);
                            command_col--;
                            kprint_at(" ", command_col, command_row);
                            kprint_at((char *)key_buffer, command_col_start, command_row);
                            set_cursor_pos(index + command_col_start, command_row);
                        }
                    } else {
                        set_cursor_pos(command_col, command_row);
                    }
                else
                    kbackspace();
                break;
            case KEYDOWN_TAB:
                break;
            case KEYDOWN_ENTER:
                set_cursor_pos(0, command_row);
                kprintln("");
                user_input((char *)key_buffer); /* kernel-controlled function */
                key_buffer[0] = '\0';
                break;
            case KEYDOWN_LCTRL:
                break;
            case KEYDOWN_LSHIFT:
                shift_pressed = 1;
                break;
            case KEYDOWN_RSHIFT:
                shift_pressed = 1;
                break;
            case KEYDOWN_LALT:
                break;
            case KEYDOWN_SPACE:
                if (command_row == get_cursor_row())
                    if (command_col == get_cursor_col()) {
                        append((char *) key_buffer, ' ');
                        command_col++;
                        kprint(" ");
                    } else if (command_col > get_cursor_col() &&
                        get_cursor_col() > command_col_start) {
                        insert((char *)key_buffer, ' ', get_cursor_col());
                        command_col++;
                        kprint_at((char *)key_buffer, command_col_start, command_row);
                    } else {
                        set_cursor_pos(command_col, command_row);
                    }
                break;
            case KEYDOWN_CAPSLOCK:
                capslock_pressed ^= 1;
                break;
            case KEYDOWN_BLANK:
                break;
            case KEYDOWN_FN:
                break;
            case KEYDOWN_F1:
                break;
            case KEYDOWN_F2:
                break;
            case KEYDOWN_F3:
                break;
            case KEYDOWN_F4:
                break;
            case KEYDOWN_F5:
                break;
            case KEYDOWN_F6:
                break;
            case KEYDOWN_F7:
                break;
            case KEYDOWN_F8:
                break;
            case KEYDOWN_F9:
                break;
            case KEYDOWN_F10:
                break;
            case KEYDOWN_F11:
                break;
            case KEYDOWN_F12:
                break;
            case KEYDOWN_NUMLOCK:
                break;
            case KEYDOWN_SCROLLLOCK:
                break;
            case KEYDOWN_KEYPAD_STAR:
                break;
            case KEYDOWN_KEYPAD_0:
                break;
            case KEYDOWN_KEYPAD_1:
                break;
            case KEYDOWN_KEYPAD_2:
                /* down arrow */
                escape_arrow_controller(0, 1);
                break;
            case KEYDOWN_KEYPAD_3:
                break;
            case KEYDOWN_KEYPAD_4:
                /* left arrow */
                escape_arrow_controller(-1, 0);
                break;
            case KEYDOWN_KEYPAD_5:
                break;
            case KEYDOWN_KEYPAD_6:
                /* right arrow */
                escape_arrow_controller(1, 0);
                break;
            case KEYDOWN_KEYPAD_7:
                break;
            case KEYDOWN_KEYPAD_8:
                /* up arrow */
                escape_arrow_controller(0, -1);
                break;
            case KEYDOWN_KEYPAD_9:
                break;
            case KEYDOWN_KEYPAD_MINUS:
                break;
            case KEYDOWN_KEYPAD_PLUS:
                break;
            case KEYDOWN_KEYPAD_DEL:
                break;
            case KEYDOWN_KEYPAD_SYSRQ:
                break;
            default:
                if (0x02 <= scancode && scancode <= 0x35 ) {
                    int8_t input;
                    if (capslock_pressed || shift_pressed) {
                        input = SHIFT_KEYBOARD_CODES[scancode - 0x02];
                    } else {
                        input = KEYBOARD_CODES[scancode - 0x02];
                    }
                    char str[2] = {input, '\0'};
                    if (command_row == get_cursor_row())
                        if (command_col == get_cursor_col()) {
                            append((char *)key_buffer, input);
                            command_col++;
                            kprint(str);
                        } else if (command_col > get_cursor_col() &&
                                   get_cursor_col() >= command_col_start) {
                            int32_t tmp = get_cursor_col();
                            insert((char *)key_buffer, input, get_cursor_col() - command_col_start);
                            command_col++;
                            kprint_at((char *)key_buffer, command_col_start, command_row);
                            set_cursor_pos(tmp + 1, command_row);
                        } else {
                            set_cursor_pos(command_col, command_row);
                        }
                    break;
                }
                /* Some keycodes we don't define */
                break;
        }
    } else {
        if (scancode == KEYDOWN_ESCAPE) {
            escape_pressed = 1;
        }
        if (scancode == KEYUP_LSHIFT || scancode == KEYUP_RSHIFT) {
            shift_pressed = 0;
        }
    }

    UNUSED(t);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}