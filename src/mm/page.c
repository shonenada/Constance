#include <page.h>

uint *pdir = (uint *) 0x9C000;
uint *ptab = (uint *) 0x9D000;

void page_init() {
    int i;
    uint addr = 0;
    for(i=0;i<1024;i++) {
        ptab[i] = (addr | PG_P | PG_RW);
        addr += 4096;
    }
    for(i=0;i<1024;i++) {
        pdir[i] = 0 | PG_RW;
    }

    // first entry
    pdir[0] = ((uint) ptab) | PG_P | PG_RW;

    flush_cr3();
    page_enable();
}

