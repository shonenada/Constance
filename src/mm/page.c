#include <system.h>
#include <const.h>
#include <asm.h>
#include <segment.h>
#include <mm.h>
#include <page.h>

extern char __kend__;
struct page pgfreelist;
struct page freepage[NPAGE];

struct pde pgd0[NPGD] __attribute__((aligned(4096)));
struct pte pgt0[1024] __attribute__((aligned(4096)));

void pm_init() {
    uint idx;
    struct page *pg, *pf;

    freepage[0].idx = 0;
    freepage[0].flag = PG_RSVD;

    for (idx=640*1024/PAGE_SIZE; idx<PPN(&__kend__)+1;idx++) {
        freepage[idx].idx = idx;
        freepage[idx].flag = PG_RSVD;
    }

    pf = &pgfreelist;
    for (idx=0;idx<NPAGE;idx++) {
        if (freepage[idx].flag & PG_RSVD || freepage[idx].count > 0) {
            continue;
        }
        pg = &freepage[idx];
        pg->idx = idx;
        pg->count = 0;
        pg->flag = 0;
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
        printk("pfind(): pn = %d", pn);
        panic("pfind(): bad parameter.");
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
    ptab_init(pgt0, PTE_P | PTE_RW);
    pgd_init(pgd0);
    pgd0[0].ppn = PPN((uint)pgt0);
    pgd0[0].flag = (PTE_P | PTE_RW);

    pm_init();
    irq_install(0x0E, do_page_fault);
    lpgd(pgd0);
    page_enable();
}
