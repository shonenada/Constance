#ifndef __PAGE_H
#define __PAGE_H
#include <const.h>

#define PTE_P           0b1    // present
#define PTE_RW          0b10    // writable
#define PTE_U           0b100    // user, supervior
#define PTE_WT          0b1000    // wirte-through
#define PTE_CD          0b10000    // cache-disabled
#define PTE_A           0b100000    // accessed
#define PTE_D           0b1000000    // dirty
#define PTE_PS          0b10000000     // Page size
#define PTE_MBZ         0b110000000    // bits must be zero

#define LO_MEM 0x100000    // TODO: fix addr
#define HI_MEM 0x1000000
#define PAGE_SIZE 0x1000    // 4kb

#define PD_INDEX(addr) ((uint)((addr>>0x16)&0x3FF))    // get page directory index by linear address, 10 bits
#define PT_INDEX(addr) ((uint)((addr>>0x0C)&0x3FF))    // get page table index by linear address, 10 bits
#define POFF(addr) ((uint)(addr&0xFFF))
#define PTE_ADDR(addr) ((uint)(addr) & ~0xFFF)
#define PPN(addr) (((uint)(addr)) >> 0x0C)

extern uint *pdir;
extern uint *ptab;

// bucket allocation
struct bucket_desc {
    void *page;
    void *free_ptr;
    ushort cnt;
    ushort bk_size;
    struct bucket_desc *next;
};

struct bk_dir {
    int size;
    struct bucket_desc *chain;
};

struct pde {
    uint ppn:20;
    uint flag:9;
    uint avl:3;
};

struct pte {
    uint ppn:20;
    uint flag:9;
    uint avl:3;
};

extern struct pde pgd0[];
extern struct bucket_desc free_bk_desc_chain;
extern struct bk_dir bucket_dir[];

// set cr3 to be the base address of page directory
extern void flush_cr3();
// set cr0(PG) to the 1, enable paging
extern void page_enable();

int bkslot(uint size);
void* kmalloc();
int do_page_fault(struct regs *rgs);
int do_no_page(struct regs *rgs);
int do_wp_page(struct regs *rgs);

struct pte* pte_find(struct pde* pgd, uint vaddr, uint creat);
void pgd_init(struct pde *pgd);
int pgd_copy(struct pde *to, struct pde *from);
int pgd_free(struct pde *pgd);
void flush_pgd();

#endif
