#ifndef __PAGE_H
#define __PAGE_H

#include <const.h>

// set cr3 to be the base address of page directory
extern void flush_cr3();
// set cr0(PG) to the 1, enable paging
extern void page_enable();

void page_init();
void kmalloc();

#define PG_P 0b1
#define PG_RW 0b10
#define PG_U 0b100

#define PAGE_SIZE (4*1024)     // 4kb each page

#endif
