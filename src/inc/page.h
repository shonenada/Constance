#ifndef __PAGE_H
#define __PAGE_H

#include <const.h>

extern void flush_cr3();
extern void page_enable();
void page_init();

#define PG_P 0b1
#define PG_RW 0b10
#define PG_U 0b100

#endif
