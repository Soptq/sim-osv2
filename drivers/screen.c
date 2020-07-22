//
// Created by Soptq on 2020/7/21.
//

#include "screen.h"

void _kprint_at(s8 *message, s32 col, s32 row, s8 attr);
s32 print_char(s8 c, s32 col, s32 row, s8 attr);
s32 get_cursor_offset();
void set_cursor_offset(s32 offset);
void clear_screen();
s32 get_offset(s32 col, s32 row);
s32 get_offset_row(s32 offset);
s32 get_offset_col(s32 offset);


/**
 * ================================================================================
 *
 * Public API
 *
 * ================================================================================
 */


void kprint_at(s8 *message, s32 col, s32 row) {
    _kprint_at(message, col, row, WHITE_ON_BLACK);
}

void kprint(s8 *message) {
    kprint_at(message, -1, -1);
}

void kprintln(s8 *message) {
    kprint(message);
    kprint("\n");
}

void err_kprint(s8 *message) {
    _kprint_at(message, -1, -1, RED_ON_WHITE);
}

void err_kprintln(s8 *message) {
    err_kprint(message);
    kprint("\n");
}

void clear_screen() {
    s32 screen_size = MAX_COLS * MAX_ROWS;
    s8* vidptr = (s8*)VIDEO_ADDRESS;
    for (s32 i = 0; i < screen_size; ++i) {
        vidptr[i * 2] = ' ';
        vidptr[i * 2 + 1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}

void kbackspace() {
    s32 offset = get_cursor_offset() - 2;
    if (offset == 0) return;

    s32 col = get_offset_col(offset);
    s32 row = get_offset_row(offset);
    print_char(' ', col, row, WHITE_ON_BLACK);
    set_cursor_offset(offset);
}

/**
 * ================================================================================
 *
 * Private API
 *
 * ================================================================================
 */

/**
 * Innermost print function for our kernel, directly accesses the video memory
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */


void _kprint_at(s8 *message, s32 col, s32 row, s8 attr) {
    if (!attr) attr = WHITE_ON_BLACK;
    s32 offset;
    if (col < 0 || row < 0) {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    s32 i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, attr);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}


s32 print_char(s8 c, s32 col, s32 row, s8 attr) {
    u8* vidmem = (u8*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2 * MAX_COLS * MAX_ROWS - 2] = 'E';
        vidmem[2 * MAX_COLS * MAX_ROWS - 1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    s32 offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else
        offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row + 1);
    } else {
        vidmem[offset++] = c;
        vidmem[offset++] = attr;
    }

    /* check if the offset is over screen size and scroll */
    if (offset >= MAX_ROWS * MAX_COLS * 2) {
        for (s32 i = 1; i < MAX_ROWS; ++i) {
            /* scroll one row */
            memory_copy((s8 *) (get_offset(0, i) + VIDEO_ADDRESS),
                        (s8 *) (get_offset(0, i - 1) + VIDEO_ADDRESS),
                    MAX_COLS * 2);
        }

        /* blank the last line */
        s8* last_line = get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS;
        for (s32 i = 0; i < MAX_COLS * 2; ++i) last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }

    set_cursor_offset(offset);
    return offset;

}

s32 get_cursor_offset() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    s32 offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(s32 offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    /* Firstly we send the higher 8 bits of the offset */
    port_byte_out(REG_SCREEN_DATA, (u8)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (u8)(offset & 0xff));
}

s32 get_offset(s32 col, s32 row) {
    return 2 * (row * MAX_COLS + col);
}

s32 get_offset_row(s32 offset) {
    return offset / (2 * MAX_COLS);
}

s32 get_offset_col(s32 offset) {
    return (offset - (2 * MAX_COLS * get_offset_row(offset))) / 2;
}