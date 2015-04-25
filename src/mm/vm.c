#include <system.h>
#include <const.h>
#include <mm.h>
#include <head.h>
#include <inode.h>
#include <vm.h>
#include <page.h>

int allocuvm(struct pde *pdir, uint oldsize, uint newsize) {
    uint addr;
    void *mem;
    struct page *pg;
    if (newsize < oldsize) {
        return oldsize;
    }
    addr = PGROUNDUP(oldsize);
    for(; addr<newsize; addr+=PAGE_SIZE) {
        pg = palloc();
        pmap(pdir, addr, pg, PTE_RW | PTE_U);
    }
    return newsize;
}

int loaduvm(struct pde* pdir, char *addr, struct inode *ip, uint offset, uint size) {
    uint i, pa, n;
    struct pte *pte;

    if ((uint) addr % PAGE_SIZE != 0)
        panic("loaduvm(): addr must be page aligned");

    for(i=0; i<size; i+=PAGE_SIZE) {
        pte = pte_find(pdir, addr+i, 0);
        if (pte == NULL) {
            return -1;
        }
        if (size - i < PAGE_SIZE)
            n = size - i;
        else
            n = PAGE_SIZE;
        readi(ip, (char*)(pte->ppn * PAGE_SIZE), offset+i, n);
    }
    return 0;
}
