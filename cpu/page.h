//
// Created by Soptq on 2020/7/22.
//

#ifndef SIM_OS_PAGE_H
#define SIM_OS_PAGE_H

#include "type.h"
#include "isr.h"

typedef struct page {
    uint32_t present    : 1;    /* page present */
    uint32_t rw         : 1;    /* read-write */
    uint32_t user       : 1;    /* supervisor level */
    uint32_t accessed   : 1;    /* been accessed since last refresh */
    uint32_t dirty      : 1;    /* been written since last refresh */
    uint32_t unused     : 7;    /* unused and reserved */
    uint32_t frame      : 20;   /* frame address */
} page_t;

/* Intel tier system, one 4kb page table after another, linked together */
typedef struct page_table {
    page_t pages[1024];
} page_table_t;

typedef struct page_directory {
    page_table_t *tables[1024];     /* total of 1024 page table */
    uint32_t tables_physical[1024];  /* physical address of above tables */
    uint32_t physical_addr;         /* physical address of tables_physical */
} page_directory_t;

void initialize_paging();
void switch_page_directory(page_directory_t *new, uint8_t force_enable_paging);
page_t *get_page(uint32_t address, int make, page_directory_t *dir);
void page_fault_callback(registers_t *regs);

#endif //SIM_BOOTLOADER_PAGE_H
