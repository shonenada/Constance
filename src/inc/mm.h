#ifndef __PAGE_H
#define __PAGE_H

#include <const.h>

#define PG_P 0b1
#define PG_RW 0b10
#define PG_U 0b100

#define PAGE_SIZE (4*1024)     // 4kb each page

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

struct bk_dir bucket_dir[] = {
    {16, (struct bk_dir*) 0},
    {32, (struct bk_dir*) 0},
    {64, (struct bk_dir*) 0},
    {128, (struct bk_dir*) 0},
    {256, (struct bk_dir*) 0},
    {512, (struct bk_dir*) 0},
    {1024, (struct bk_dir*) 0},
    {2048, (struct bk_dir*) 0},
    {4096, (struct bk_dir*) 0},
    {0, (struct bk_dir*) 0}};

struct bucket_desc *free_bk_desc_chain = (struct bucket_desc*) 0;

// set cr3 to be the base address of page directory
extern void flush_cr3();
// set cr0(PG) to the 1, enable paging
extern void page_enable();

void page_init();
void kmalloc();

#endif
