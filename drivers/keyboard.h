//
// Created by Soptq on 2020/7/22.
//

#ifndef SIM_OS_KEYBOARD_H
#define SIM_OS_KEYBOARD_H

/**
 * Details of keycode mapping: https://www.win.tue.nl/~aeb/linux/kbd/scancodes-1.html
 */

#define KEYDOWN_ERROR 0x00
#define KEYDOWN_ESC 0x01
#define KEYDOWN_BACKSPACE 0x0e
#define KEYDOWN_TAB 0x0f
#define KEYDOWN_ENTER 0x1c
#define KEYDOWN_LCTRL 0x1d
#define KEYDOWN_LSHIFT 0x2a
#define KEYDOWN_RSHIFT 0x36
#define KEYDOWN_KEYPAD_STAR 0x37
#define KEYDOWN_LALT 0x38
#define KEYDOWN_SPACE 0x39
#define KEYDOWN_CAPSLOCK 0x3a
#define KEYDOWN_F1 0x3b
#define KEYDOWN_F2 0x3c
#define KEYDOWN_F3 0x3d
#define KEYDOWN_F4 0x3e
#define KEYDOWN_F5 0x3f
#define KEYDOWN_F6 0x40
#define KEYDOWN_F7 0x41
#define KEYDOWN_F8 0x42
#define KEYDOWN_F9 0x43
#define KEYDOWN_F10 0x44
#define KEYDOWN_NUMLOCK 0x45
#define KEYDOWN_SCROLLLOCK 0x46
#define KEYDOWN_KEYPAD_7 0x47
#define KEYDOWN_KEYPAD_8 0x48
#define KEYDOWN_KEYPAD_9 0x49
#define KEYDOWN_KEYPAD_MINUS 0x4a
#define KEYDOWN_KEYPAD_4 0x4b
#define KEYDOWN_KEYPAD_5 0x4c
#define KEYDOWN_KEYPAD_6 0x4d
#define KEYDOWN_KEYPAD_PLUS 0x4e
#define KEYDOWN_KEYPAD_1 0x4f
#define KEYDOWN_KEYPAD_2 0x50
#define KEYDOWN_KEYPAD_3 0x51
#define KEYDOWN_KEYPAD_0 0x52
#define KEYDOWN_KEYPAD_DEL 0x53
#define KEYDOWN_KEYPAD_SYSRQ 0x54
#define KEYDOWN_FN 0x55
#define KEYDOWN_BLANK 0x56
#define KEYDOWN_F11 0x57
#define KEYDOWN_F12 0x58

#define KEYUP_LSHIFT 0xaa
#define KEYUP_RSHIFT 0xb6
#define KEYUP_CAPSLOCK 0xba

#define KEYDOWN_ESCAPE 0xe0

/**
 * Starting from 0x02 and ends at 0x35
 */
static const char* KEYBOARD_CODES = "1234567890-=  qwertyuiop[]  asdfghjkl;'` \\zxcvbnm,./";
static const char* SHIFT_KEYBOARD_CODES = "!@#$%^&*()_+  QWERTYUIOP{}  ASDFGHJKL:\"~ |ZXCVBNM<>?";

void init_keyboard();

#endif //SIM_OS_KEYBOARD_H
