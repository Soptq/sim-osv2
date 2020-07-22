//
// Created by Soptq on 2020/7/22.
//

#include "keyboard.h"
#include "../cpu/ports.h"
#include "../cpu/isr.h"
#include "screen.h"
#include "../libc/function.h"
#include "../kernel/kernel.h"

/**
 * Starting from 0x02 and ends at 0x35
 */
const s8* KEYBOARD_CODES = "1234567890-=  qwertyuiop[]  asdfghjkl;'` \\zxcvbnm,./";
const s8* SHIFT_KEYBOARD_CODES = "!@#$%^&*()_+  QWERTYUIOP{}  ASDFGHJKL:\"~ |ZXCVBNM<>?";

s8 shift_pressed = 0;
s8 capslock_pressed = 0;

static s8 key_buffer[256];

static void keyboard_callback(registers_t t) {
    u8 scancode = port_byte_in(0x60);

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
                backspace(key_buffer);
                kbackspace();
                break;
            case KEYDOWN_TAB:
                break;
            case KEYDOWN_ENTER:
                kprintln("");
                user_input(key_buffer); /* kernel-controlled function */
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
                append(key_buffer, ' ');
                kprint(" ");
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
                break;
            case KEYDOWN_KEYPAD_3:
                break;
            case KEYDOWN_KEYPAD_4:
                break;
            case KEYDOWN_KEYPAD_5:
                break;
            case KEYDOWN_KEYPAD_6:
                break;
            case KEYDOWN_KEYPAD_7:
                break;
            case KEYDOWN_KEYPAD_8:
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
                    s8 input;
                    if (capslock_pressed || shift_pressed) {
                        input = SHIFT_KEYBOARD_CODES[scancode - 0x02];
                    } else {
                        input = KEYBOARD_CODES[scancode - 0x02];
                    }
                    s8 str[2] = {input, '\0'};
                    append(key_buffer, input);
                    kprint(str);
                }
                /* Some keycodes we don't define */
                break;
        }
    } else {
        if (scancode == KEYUP_LSHIFT || scancode == KEYUP_RSHIFT) {
            shift_pressed = 0;
        }
    }

    UNUSED(t);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}