#ifndef __PAGE_H
#define __PAGE_H

struct page {
    uchar count;
    uchar flag;
    ushort idx;
    struct page *next;
};

#define PG_RSVD 1;

extern struct page pfreelist;
extern struct page pagemap[NPAGE];

struct page *palloc();
void pfree(struct page*);
struct page *pfind(uint pn);
void pmap(struct pde *pde, void* vaddr);
void pm_init();
void mm_init();
void page_init();

#endif
