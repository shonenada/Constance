#include <system.h>
#include <const.h>
#include <asm.h>
#include <mm.h>
#include <sched.h>

struct pde pgd0[1024];

struct pte* find_pte(struct pde* pgd, uint vaddr, uint creat) {
    uint page;
    struct pde *pde;
    struct pte *pt;

    if (vaddr < KMEM_END) {
        panic("find_pte(): cannot access kernel space");
    }

    pde = &(pgd[PD_INDEX(vaddr)]);
    if ((pde->flag & PTE_P) == 0) {
        if (creat == 0) {
            return NULL;
        }
        page = palloc();
        pde->flag = PTE_P | PTE_U | PTE_RW;
        pde->off = page;
        pt = (struct pte*) (page);
        memset(pt, 0, PAGE_SIZE);
        flmmu();
    }
    pt = (struct pte*)(pde->off);
    return &pt[PT_INDEX(vaddr)];
}

int pgd_init(struct pde *pgd) {
    uint pn;

    for (pn=0;pn<PMEM/(PAGE_SIZE*1024);pn++) {
        pgd[pn].off = pn << 10;
        pgd[pn].flag = PTE_PS | PTE_P | PTE_RW;
    }
    for(pn=PMEM/(PAGE_SIZE*1024);pn<1024;pn++) {
        pgd[pn].off = 0;
        pgd[pn].flag = PTE_U;
    }
    return 0;
}

int pt_copy(struct pde *npgd, struct pde *opgd) {
    /**
    uint page, pdn, pn;
    struct pde *opde, *npde;
    struct pte *opte, *npte, *old_pt, *new_pt;

    for (pdn=PD_INDEX(KMEM_END);pdn<1024;pdn++) {
        opde = &opgd[pdn];
        npde = &npgd[pdn];
        pnde->flag = opde->flag;
        if (opde->flag & PTE_P) {
            old_pt = (struct pte*)(opde->off * PAGE_SIZE);
            new_pt = (struct pte*)kmalloc(PAGE_SIZE);
            npde->off = PPN(new_pt);
            for(pn=0;pn<1042; pn++) {
                opte = &old_pt[pn];
                npte = &new_pt[pn];
                npte->off = opte->off;
                npte->flag = opte->flag;
                if (opte->flag & PTE_P) {
                    npte->flag &= ~PTE_W;
                    npte->flag &= ~PTE_W;
                    pg = pgfind(opte->off);
                    pg->count++;
                }
            }
        }
    }
    return 0;
    **/
}

int pt_free(struct pde *pgd) {
    uint pdn, pn;
    struct pde *pde;
    struct pte *pte, *pt;
    struct page *pg;

    for(pdn=PD_INDEX(KMEM_END);pdn<1024;pdn++) {
        pde = &pgd[pdn];
        if (pde->flag & PTE_P) {
            pt = (struct pte *)(pde->off * PAGE_SIZE);
            for (pn=0;pn<1024;pn++) {
                pte = &pt[pn];
                if (pte->flag & PTE_P) {
                    pg = pgfind(pte->off);
                    pgfree(pg);
                }
            }
            kfree(pt, PAGE_SIZE);
        }
    }
    return 0;
}

void flmmu() {
    lpgd(current->tvm.pd);
}
