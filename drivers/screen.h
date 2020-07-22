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

#include "../cpu/types.h"
#include "ports.h"
#include "../kernel/util.h"

void clear_screen();
void kprint_at(s8 *message, s32 col, s32 row);
void kprint(s8 *message);
void kprintln(s8 *message);
void err_kprint(s8 *message);
void err_kprintln(s8 *message);
void kbackspace();

#endif //SIM_OS_SCREEN_H
