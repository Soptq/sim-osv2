//
// Created by Soptq on 2020/7/21.
//

#ifndef SIM_OS_SCREEN_H
#define SIM_OS_SCREEN_H

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

#include "../cpu/type.h"

void clear_screen();
void kprint_at(char *message, int32_t col, int32_t row);
void kprint(char *message);
void kprintln(char *message);
void err_kprint(char *message);
void err_kprintln(char *message);
void kbackspace();

#endif //SIM_OS_SCREEN_H
