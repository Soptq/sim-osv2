//
// Created by Soptq on 2020/7/21.
//

#ifndef SIM_OS_PORTS_H
#define SIM_OS_PORTS_H

#include "type.h"

uint8_t port_byte_in (uint16_t port);
void port_byte_out (uint16_t, uint8_t data);
uint16_t port_word_in (uint16_t port);
void port_word_out (uint16_t port, uint16_t data);
#endif //SIM_OS_PORTS_H
