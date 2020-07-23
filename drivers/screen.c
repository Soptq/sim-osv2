//
// Created by Soptq on 2020/7/21.
//

#include "screen.h"
#include "../libc/mem.h"
#include "../cpu/ports.h"
#include "../libc/string.h"

void _kprint_at(char *message, int32_t  col, int32_t  row, int8_t attr);
int32_t  print_char(char c, int32_t  col, int32_t  row, int8_t attr);
int32_t  get_cursor_offset();
void set_cursor_offset(int32_t  offset);
int32_t  get_offset(int32_t  col, int32_t  row);
int32_t  get_offset_row(int32_t  offset);
int32_t  get_offset_col(int32_t  offset);


/**
 * ================================================================================
 *
 * Public API
 *
 * ================================================================================
 */


void kprint_at(char *message, int32_t  col, int32_t  row) {
    _kprint_at(message, col, row, WHITE_ON_BLACK);
}

void kprint(char *message) {
    kprint_at(message, -1, -1);
}

void kprintln(char *message) {
    kprint(message);
    kprint("\n");
}

void err_kprint(char *message) {
    _kprint_at(message, -1, -1, RED_ON_WHITE);
}

void err_kprintln(char *message) {
    err_kprint(message);
    kprint("\n");
}

void clear_screen() {
    int32_t  screen_size = MAX_COLS * MAX_ROWS;
    int8_t* vidptr = (int8_t*)VIDEO_ADDRESS;
    for (int32_t  i = 0; i < screen_size; ++i) {
        vidptr[i * 2] = ' ';
        vidptr[i * 2 + 1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}


void kprint_hex(uint32_t hex) {
    char tmp[11];
    int_to_hexstr(hex, tmp);
    kprint(tmp);
}

void kprintln_hex(uint32_t hex) {
    char tmp[11];
    int_to_hexstr(hex, tmp);
    kprintln(tmp);
}


void err_kprint_hex(uint32_t hex) {
    char tmp[11];
    int_to_hexstr(hex, tmp);
    err_kprint(tmp);
}

void err_kprintln_hex(uint32_t hex) {
    char tmp[11];
    int_to_hexstr(hex, tmp);
    err_kprintln(tmp);
}


void kbackspace() {
    int32_t  offset = get_cursor_offset() - 2;
    int32_t  col = get_offset_col(offset);
    int32_t  row = get_offset_row(offset);
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


void _kprint_at(char *message, int32_t  col, int32_t  row, int8_t attr) {
    if (!attr) attr = WHITE_ON_BLACK;
    int32_t  offset;
    if (col < 0 || row < 0) {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    int32_t  i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, attr);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}


int32_t print_char(char c, int32_t  col, int32_t  row, int8_t attr) {
    uint8_t* vidmem = (uint8_t*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2 * MAX_COLS * MAX_ROWS - 2] = 'E';
        vidmem[2 * MAX_COLS * MAX_ROWS - 1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int32_t  offset;
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
        for (int32_t  i = 1; i < MAX_ROWS; ++i) {
            /* scroll one row */
            memcpy((void *) (get_offset(0, i - 1) + VIDEO_ADDRESS),
                    (const void *) (get_offset(0, i) + VIDEO_ADDRESS),
                    MAX_COLS * 2);
        }

        /* blank the last line */
        int8_t* last_line = (int8_t *) (get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS);
        for (int32_t  i = 0; i < MAX_COLS * 2; ++i) last_line[i] = 0;

        offset -= 2 * MAX_COLS;
    }

    set_cursor_offset(offset);
    return offset;
}

int32_t  get_cursor_offset() {
    port_byte_out(REG_SCREEN_CTRL, 14);
    int32_t  offset = port_byte_in(REG_SCREEN_DATA) << 8;
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2;
}

void set_cursor_offset(int32_t  offset) {
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    /* Firstly we send the higher 8 bits of the offset */
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (uint8_t)(offset & 0xff));
}

int32_t  get_offset(int32_t  col, int32_t  row) {
    return 2 * (row * MAX_COLS + col);
}

int32_t  get_offset_row(int32_t  offset) {
    return offset / (2 * MAX_COLS);
}

int32_t  get_offset_col(int32_t  offset) {
    return (offset - (2 * MAX_COLS * get_offset_row(offset))) / 2;
}