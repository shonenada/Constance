#include <system.h>
#include <const.h>
#include <asm.h>
#include <mm.h>
#include <page.h>
#include <sched.h>

struct pte* pte_find(struct pde* pgd, uint vaddr) {
    uint pdx;
    struct pde *pde;
    struct pte *pt;
    struct page *pg;

    pdx = PD_INDEX(vaddr);
    pde = &pgd[pdx];

    if ((pde->flag & PTE_P) == 0) {
        pg = palloc();
        pde->flag = PTE_P | PTE_U | PTE_RW;
        pde->ppn = pg->idx;
        pt = (struct pte *)(pde->ppn * PAGE_SIZE);
        memset(pt, 0, PAGE_SIZE);
        flush_pgd();
    }

    pt = (struct pte*)(pde->ppn * PAGE_SIZE);
    return &pt[PT_INDEX(vaddr)];
}

void ptab_init(struct pte *pt, uint flag) {
    int i;
    for (i=0;i<1024;i++) {
        pt[i].ppn = (i * PAGE_SIZE) >> 12;
        pt[i].flag = flag;
    }
}

void pgd_init(struct pde *pd) {
    uint pn;
    for (pn=0;pn<NPGD;pn++) {
        pd[pn].ppn = 0;
        pd[pn].flag = PTE_U | PTE_RW;
    }
}

int pgd_copy(struct pde *to_pd, struct pde *from_pd) {
    uint idx, pn;
    struct page *pg, *fpg;
    struct pde *to_pde, *from_pde;
    struct pte *to_pt, *from_pt, *to_pte, *from_pte;

    for (idx=0;idx<NPGD;idx++) {
        from_pde = &from_pd[idx];
        to_pde = &to_pd[idx];
        to_pde->flag = from_pde->flag;
        if (from_pde->flag * PTE_P) {
            from_pt = (struct pte*)(from_pde->ppn * PAGE_SIZE);
            pg = palloc();
            to_pt = (struct pte*)(pg->idx * PAGE_SIZE);    // linear address
            to_pd->ppn = PPN(to_pt);
            for (pn=0;pn<1024;pn++) {
                from_pte = &from_pt[pn];
                to_pte = &to_pt[pn];
                to_pte->ppn = from_pte->ppn;
                to_pte->flag = from_pte->flag;
                if (from_pte->flag & PTE_P) {
                    from_pte->flag &= ~PTE_RW;
                    to_pte->flag &= ~PTE_RW;
                    fpg = pfind(from_pte->ppn);
                    fpg->count++;
                }
            }
        }
    }
    return 0;
}

int pgd_free(struct pde* pgd) {
    uint idx, ptn;
    struct pde *pde;
    struct pte *pt, *pte;
    struct page *page;

    for(idx=0;idx<1024;idx++) {
        pde = &pgd[idx];
        if (pde->flag & PTE_P) {
            pt = (struct pte*)(pde->ppn * PAGE_SIZE);
            for (ptn=0;ptn<1024;ptn++) {
                pte = &pt[ptn];
                if (pte->flag & PTE_P) {
                    page = pfind(pte->ppn);
                    pfree(page);
                }
            }
            page = pfind(PPN(pt));
            pfree(page);
        }
    }
    return 0;
}

void flush_pgd() {
    // lpgd(current->tvm.pgd);
}
