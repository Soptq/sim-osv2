//
// Created by Soptq on 2020/7/21.
//

#ifndef SIM_OS_PORTS_H
#define SIM_OS_PORTS_H
unsigned char port_byte_in (unsigned short port);
void port_byte_out (unsigned short port, unsigned char data);
unsigned short port_word_in (unsigned short port);
void port_word_out (unsigned short port, unsigned short data);
#endif //SIM_OS_PORTS_H
