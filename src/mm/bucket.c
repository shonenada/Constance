#include <system.h>
#include <const.h>
#include <asm.h>
#include <console.h>
#include <segment.h>
#include <mm.h>

struct bucket_desc free_bk_desc_chain = {0,};

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

int bkslot(uint size) {
    int i, b;
    if (size <= 0) {
        return -1;
    }
    b = 16;
    for (i=0;b<=4096;b=b<<1,i++) {
        if (size <= b) {
            return i;
        }
    }
    return -1;
}

struct bucket_desc* bkalloc() {
    struct bucket_desc* bk, *bnext;
    uint page;

    if (free_bk_desc_chain.next == NULL) {
        bnext = &free_bk_desc_chain;
        page = palloc();
        cli();
        for(bk = (struct bucket_desc*)page;(uint)bk<page + PAGE_SIZE; bk++) {
            bnext->next = bk;
            bk->next = NULL;
            bnext = bk;
        }
        sti();
    }

    bk = free_bk_desc_chain.next;
    free_bk_desc_chain.next = bk->next;
    return bk;
}

int bkfree(struct bucket_desc *bk) {
    cli();
    bk->next = free_bk_desc_chain.next;
    free_bk_desc_chain.next = bk;
    sti();
    return 0;
}

int bkinit(struct bucket_desc *bk, int size) {
    int i;
    uint page;
    struct bucket_desc *bkp, *bd;

    page = palloc();
    bk->page = page;
    bk->bk_size = size;
    bk->free_ptr = (void*) page;
    bkp = bk;
    for (i=0;i<PAGE_SIZE/size;i++) {
        bd = (struct bucket_desc*)(page + i * size);
        bd->next = NULL;
        bkp->next = bd;
        bkp = bd;
    }
    return 0;
}
