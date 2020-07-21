//
// Created by Soptq on 2020/7/21.
//

#ifndef SIM_OS_TYPES_H
#define SIM_OS_TYPES_H

typedef     unsigned    int     u32;
typedef                 int     s32;
typedef     unsigned    short   u16;
typedef                 short   s16;
typedef     unsigned    char    u8;
typedef                 char    s8;

#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)

#endif //SIM_OS_TYPES_H
