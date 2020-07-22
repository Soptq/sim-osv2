//
// Created by Soptq on 2020/7/22.
//

#include "keyboard.h"
#include "ports.h"
#include "../cpu/isr.h"
#include "screen.h"

u8 keyboard_out(u8 scancode);

u8 shift_pressed = 0;
u8 capslock_pressed = 0;
u8 keystr[2];

static void keyboard_callback(registers_t t) {
    u8 scancode = port_byte_in(0x60);
    u8 keychar = keyboard_out(scancode);
    if (keychar == '\0') return;
    keystr[0] = keychar;
    keystr[1] = '\0';

    kprint(keystr);
}

void init_keyboard() {
    register_interrupt_handler(IRQ1, keyboard_callback);
}

/* 'keuyp' event corresponds to the 'keydown' + 0x80
 * it may still be a scancode we haven't implemented yet, or
 * maybe a control/escape sequence */

u8 keyboard_out(u8 scancode) {
    switch (scancode) {
        case KEYDOWN_ERROR:
            break;
        case KEYDOWN_ESC:
            break;
        case KEYDOWN_BACKSPACE:
            kbackspace();
            break;
        case KEYDOWN_TAB:
            break;
        case KEYDOWN_ENTER:
            kprintln("");
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
            return ' ';
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
            /* 'keuyp' event corresponds to the 'keydown' + 0x80
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (0x02 <= scancode && scancode <= 0x35 ) {
                if (capslock_pressed || shift_pressed) {
                    return SHIFT_KEYBOARD_TWOFACECHAR[scancode - 0x02];
                } else {
                    return KEYBOARD_TWOFACECHAR[scancode - 0x02];
                }
            } else if (scancode <= 0x7f) {
                /* Some keycodes we don't define */
            } else if (scancode <= 0xff) {
                if (scancode == KEYUP_LSHIFT || scancode == KEYUP_RSHIFT) {
                    shift_pressed = 0;
                }
            } else {
                /* some other keycodes */
            }
            break;
    }
    return '\0';
}
