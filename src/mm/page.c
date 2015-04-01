#include <system.h>
#include <const.h>
#include <asm.h>
#include <segment.h>
#include <mm.h>
#include <page.h>

struct page pgfreelist;
struct page freepage[NPAGE];
extern char __kend__;

void pm_init() {
    uint idx;
    struct page *pg, *pf;

    for (idx=640*1024/PAGE_SIZE; idx<PPN(&__kend__)+1;idx++) {
        freepage[idx].idx = idx;
        freepage[idx].flag = PG_RSVD;
    }

    pf = &pgfreelist;
    for (idx=0;idx<NPAGE;idx++) {
        if (freepage[idx].flag & PG_RSVD) {
            continue;
        }
        pg = &freepage[idx];
        pg->idx = idx;
        pg->count = 0;
        pg->next = NULL;
        pf->next = pg;
        pf = pg;
    }
}

struct page *palloc() {
    struct page *page;
    if (pgfreelist.next == NULL) {
        panic("palloc(): no free page. \n");
        return NULL;
    }

    cli();
    page = pgfreelist.next;
    page->count++;
    pgfreelist.next = page->next; 
    sti();
    return page;
}

int pfree(struct page* page) {
    if (page->count <= 0) {
        panic("pfree(): page is free"); 
        return -1;
    }

    cli();
    page->count--;
    if (page->count <= 0) {
        page->count = 0;
        page->next = pgfreelist.next;
        pgfreelist.next = page;
    }
    sti();
    return page->idx;
}

struct page *pfind(uint pn) {
    if (pn < 0 || pn >= NPAGE) {
        panic("pfind(): bad parameter");
        return NULL;
    }
    return &freepage[pn];
}

struct pte* pmap(struct pde *pgd, void* vaddr, struct page *page, uchar flag) {
    struct pte *pte;
    pte = pte_find(pgd, vaddr);
    pte->ppn = page->idx;
    pte->flag = flag;
    lpgd(pgd);
    return pte;
}

// setup paging
void page_init() {
    pgd_init(pgd0);
    pm_init();
    irq_install(0x0E, do_page_fault);
    lpgd(pgd0);
    page_enable();
}
