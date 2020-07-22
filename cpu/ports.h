//
// Created by Soptq on 2020/7/21.
//

#ifndef SIM_OS_PORTS_H
#define SIM_OS_PORTS_H

#include "types.h"

u8 port_byte_in (u16 port);
void port_byte_out (u16, u8 data);
u16 port_word_in (u16 port);
void port_word_out (u16 port, u16 data);
#endif //SIM_OS_PORTS_H
