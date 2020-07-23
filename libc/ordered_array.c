//
// Created by Soptq on 2020/7/23.
//

#include "ordered_array.h"
#include "mem.h"
#include "../libc/assert.h"

/**
 * if the first argument is less than the second, return 1
 * else return 0
 */
int8_t standard_lessthan_predicate(type_t a, type_t b) {
    return (a < b);
}


/**
 * Create an ordered array by kmalloc ing some space;
 * This needs kmalloc function to be working.
 */
ordered_array_t create_ordered_array(uint32_t max_size, lesssthan_predicate_t less_than) {
    ordered_array_t ordered_array;
    ordered_array.array = (void *)kmalloc(max_size * sizeof(type_t));
    memset(ordered_array.array, 0, max_size * sizeof(type_t));

    ordered_array.size = 0;
    ordered_array.max_size = max_size;
    ordered_array.less_than = less_than;

    return ordered_array;
}

/**
 * Create an ordered array at given address;
 * This is useful when kmalloc is unusable.
 */
ordered_array_t place_ordered_array(void *addr, uint32_t max_size, lesssthan_predicate_t less_than){
    ordered_array_t ordered_array;
    ordered_array.array = (type_t*) addr;
    memset(ordered_array.array, 0, max_size * sizeof(type_t));

    ordered_array.size = 0;
    ordered_array.max_size = max_size;
    ordered_array.less_than = less_than;

    return ordered_array;
}

void destory_ordered_array(ordered_array_t *array) {
//    kfree(array->array);
}

void insert_ordered_array(type_t item, ordered_array_t *array) {
    assert(array->less_than != NULL, "Ordered array insertion failed! Function less_than not configured!");
    uint32_t iterator = 0;
    while ((iterator < array -> size) &&
        (array -> less_than(array -> array[iterator], item))) {
        item ++;
    }
    /* item is for the first time smaller than array -> array[iterator] */
    /* we will insert the item here */
    if (iterator == array -> size) { /* the last one of the array */
        array -> array[array -> size++] = item;
    } else {
        type_t tmp = array -> array[iterator];
        array -> array[iterator] = item;
        while (iterator < array -> size) {  /* push 1 to the last */
            iterator++;
            type_t tmp2 = array -> array[iterator];
            array -> array[iterator] = tmp;
            tmp = tmp2;
        }
        array -> size ++;
    }
}

type_t lookup_ordered_array(uint32_t i, ordered_array_t *array) {
    assert(i < array -> size, "Ordered array lookup failed! lookup index is out of the bound!");
    return array -> array[i];
}

void remove_ordered_array(uint32_t i, ordered_array_t *array) {
    while (i < array -> size) {
        array -> array[i] = array -> array[i + 1];
        i++;
    }
    array -> size--;
}


