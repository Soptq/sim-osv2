//
// Created by Soptq on 2020/7/23.
//

#include "assert.h"
#include "../drivers/screen.h"

void assert(int8_t bool_exp, char*msg){
    if (!bool_exp) {
        err_kprintln("");
        err_kprintln(msg);
        cpu_halt();
    }
}