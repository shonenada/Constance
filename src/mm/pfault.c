#include <const.h>
#include <system.h>
#include <console.h>
#include <segment.h>
#include <mm.h>
#include <page.h>
#include <sched.h>

int do_page_fault(struct regs *rgs) {
    uint cr2;
    asm volatile ("movl %%cr2, %0":"=a"(cr2));
    if ((rgs->err_code & PFE_P) == 0) {
        do_no_page(cr2);
        return 0;
    }

    if ((rgs->err_code & PFE_W)) {
        do_wp_page(cr2);
        return 0;
    }

    if (rgs->err_code & PFE_U) {
        // TODO
    }

    return 0;
}

int do_wp_page(uint addr) {
    char *old, *new;
    struct pte *pte;
    struct page *page;

    pte = pte_find(current->pdir, addr);
    if (pte == NULL) {
        panic("do_wp_page(): wrong addr");
        return -1;
    }

    page = pfind(pte->ppn);

    if (page->count <= 0) {
        panic("do_wp_page(): invalid page");
        return -1;
    } else if (page->count == 1) {
        page->flag |= PTE_RW;
        flush_pgd();
    } else if (page->count > 1) {
        old = (char*)(pte->ppn * PAGE_SIZE);
        new = (char*)kmalloc(PAGE_SIZE); 
        memcpy(new, old, PAGE_SIZE);
        pte->ppn = PPN(new);
        pte->flag |= PTE_RW;
        flush_pgd();
    }
    return 0;
}

int do_no_page(uint addr) {
    struct page *page;

    page = palloc();
    pmap(current->pdir, PTE_ADDR(addr), page, PTE_P | PTE_RW | PTE_U);

    flush_pgd();

    return 0;
}

