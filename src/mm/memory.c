#include <const.h>
#include <segment.h>
#include <mm.h>

uint *pdir = (uint *) 0x10000;
uint *ptab = (uint *) 0x1D000;

uchar page_map[NPAGE] = {0,};
struct bucket_desc *free_bk_desc_chain = (struct bucket_desc*) 0;
struct bk_dir bucket_dir[] = {
    {16, (struct bk_dir*) 0},
    {32, (struct bk_dir*) 0},
    {64, (struct bk_dir*) 0},
    {128, (struct bk_dir*) 0},
    {256, (struct bk_dir*) 0},
    {512, (struct bk_dir*) 0},
    {1024, (struct bk_dir*) 0},
    {2048, (struct bk_dir*) 0},
    {4096, (struct bk_dir*) 0},
    {0, (struct bk_dir*) 0}};

int do_page_fault(struct regs *rgs) {
    uint cr2;
    asm volatile ("movl %%cr2, %0":"=a"(cr2));
    printk("do_page_fault(): cr2= %x\n", cr2);
    for(;;);
}

int do_no_page(struct regs *rgs) {
}

int do_wp_page(struct regs *rgs) {
}

uint palloc() {
    int i;
    for (i=0;i<NPAGE;i++) {
        if (page_map[i] == 0) {
            page_map[i]++;
            return (LO_MEM + (i * PAGE_SIZE));
        }
    }
    panic("palloc(): no enough frame");
    return -1;
}

uint pfree(uint addr) {
    int i;
    i = (addr - LO_MEM) - PAGE_SIZE;
    if (page_map[i] > 0) {
        page_map[i]--;
        return i;
    }
    return -1;
}

// setup paging
void page_init() {
    int i;
    uint addr = 0;
    for(i=0;i<NPAGE;i++) {
        ptab[i] = (addr | PG_P | PG_RW | PG_U);
        addr += PAGE_SIZE;    // 4kb each page
    }
    for(i=0;i<NPAGE;i++)
        pdir[i] = 0 | PG_RW;

    // first entry
    pdir[0] = ((uint) ptab) | PG_P | PG_RW | PG_U;

    irq_install(0x0E, do_page_fault);

    flush_cr3();
    page_enable();
}
