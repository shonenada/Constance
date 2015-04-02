#ifndef __PAGE_H
#define __PAGE_H

struct page {
    uchar count;
    uchar flag;
    ushort idx;
    struct page *next;
};

#define PG_RSVD 1

#define PFE_P 0x1
#define PFE_W 0x2
#define PFE_U 0x4
#define PFE_RSVD 0x8
#define PFE_I 0x10

extern struct page pfreelist;
extern struct page pagemap[NPAGE];

struct page *palloc();
int pfree(struct page*);
struct page* pfind(uint pn);
struct pte* pmap(struct pde *pgd, void* vaddr, struct page *page, uchar flag);
void pm_init();
void mm_init();
void page_init();

#endif
