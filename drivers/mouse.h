//
// Created by Soptq on 2020/7/24.
//

#ifndef SIM_OS_MOUSE_H
#define SIM_OS_MOUSE_H

#include "../cpu/type.h"

#define MOUSE_SENSITIVITY 2

typedef struct {
    int32_t x;
    int32_t y;
} mouse_position_t;

void install_mouse();

#endif //SIM_OS_MOUSE_H
