//
// Created by Soptq on 2020/7/21.
//

#ifndef SIM_OS_TYPES_H
#define SIM_OS_TYPES_H

#include "stdint.h"
#include "stddef.h"

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

#endif //SIM_OS_TYPES_H
