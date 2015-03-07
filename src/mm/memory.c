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

// setup paging
void page_init() {
    int i;
    uint addr = 0;
    for(i=0;i<NPAGE;i++) {
        ptab[i] = (addr | PTE_P | PTE_RW | PTE_U);
        addr += PAGE_SIZE;    // 4kb each page
    }
    for(i=0;i<NPAGE;i++)
        pdir[i] = 0 | PTE_RW;

    // first entry
    pdir[0] = ((uint) ptab) | PTE_P | PTE_RW | PTE_U;

    irq_install(0x0E, do_page_fault);

    flush_cr3();
    page_enable();
}
