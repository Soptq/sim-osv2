//
// Created by Soptq on 2020/7/23.
//

#ifndef SIM_OS_ARRAY_H
#define SIM_OS_ARRAY_H

#include "../cpu/type.h"

typedef void* type_t;
typedef int8_t (*lesssthan_predicate_t)(type_t, type_t);

typedef struct {
    type_t *array;
    uint32_t size;
    uint32_t max_size;
    lesssthan_predicate_t less_than;
} ordered_array_t;

int8_t standard_lessthan_predicate(type_t a, type_t b);

ordered_array_t create_ordered_array(uint32_t max_size, lesssthan_predicate_t less_than);
ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lesssthan_predicate_t less_than);

void destory_ordered_array(ordered_array_t *array);

void insert_ordered_array(type_t item, ordered_array_t *array);

type_t lookup_ordered_array(uint32_t i, ordered_array_t *array);

void remove_ordered_array(uint32_t i, ordered_array_t *array);

#endif //SIM_OS_ORDERED_ARRAY_H
