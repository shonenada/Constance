#ifndef __PAGE_H
#define __PAGE_H

#include <const.h>

#define PG_P 0b1
#define PG_RW 0b10
#define PG_U 0b100

#define LO_MEM 0x100000
#define HI_MEM 0x1000000
#define PAGE_SIZE 0x1000    // 4kb
#define NR_FRAME ((HI_MEM-LO_MEM)/PAGE_SIZE)

// bucket allocation
struct bucket_desc {
    void *page;
    void *free_ptr;
    ushort cnt;
    ushort bk_size;
    struct bucket_desc *next;
};

struct bk_dir {
    int size;
    struct bucket_desc *chain;
};

struct bucket_desc *free_bk_desc_chain;

// set cr3 to be the base address of page directory
extern void flush_cr3();
// set cr0(PG) to the 1, enable paging
extern void page_enable();

void page_init();
void kmalloc();
uint palloc();
uint pfree(uint addr);

#endif
