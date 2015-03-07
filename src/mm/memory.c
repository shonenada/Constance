#include <const.h>
#include <mm.h>

uint *pdir = (uint *) 0x100000;
uint *ptab = (uint *) 0x10D000;

// setup paging
void page_init() {
    int i;
    uint addr = 0;
    for(i=0;i<NR_PAGE;i++) {
        ptab[i] = (addr | PG_P | PG_RW);
        addr += PAGE_SIZE;    // 4kb each page
    }
    for(i=0;i<NR_PAGE;i++)
        pdir[i] = 0 | PG_RW;

    // first entry
    pdir[0] = ((uint) ptab) | PG_P | PG_RW;

    flush_cr3();
    page_enable();
}
