#include <system.h>
#include <const.h>
#include <asm.h>
#include <console.h>
#include <segment.h>
#include <mm.h>

uint *pdir = (uint *) 0x10000;
uint *ptab = (uint *) 0x1D000;

uchar page_map[NPAGE] = {0,};

// alloc a page
uint palloc() {
    int i;
    for (i=0;i<NPAGE;i++) {
        if (page_map[i] == 0) {
            page_map[i]++;
            return (LO_MEM + (i * PAGE_SIZE));
        }
    }
    panic("palloc(): no enough page");
    return -1;
}

uint pfree(uint addr) {
    int i;
    i = (addr - LO_MEM) / PAGE_SIZE;
    if (page_map[i] > 0) {
        page_map[i]--;
        return i;
    }
    return -1;
}

// addr: linear address
// phyaddr: physical address
int map_page(uint addr, uint phyaddr, uint flag) {
    uint pde = pdir[PD_INDEX(addr)];
    if (!(pde & PTE_P)) {     //
        pde = palloc();
        if (pde < 0)
            panic("map_page(): no enough page");
        pdir[PD_INDEX(addr)] = pde | PTE_P | PTE_RW | PTE_U;
    }
    uint *ptab = (uint *) PTE_ADDR(pde);
    ptab[PT_INDEX(addr)] = phyaddr | flag;
    page_map[phyaddr/0x1000]++;
    return 0;
}
