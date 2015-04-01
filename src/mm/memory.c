#include <const.h>
#include <system.h>
#include <console.h>
#include <segment.h>
#include <mm.h>
#include <page.h>

int do_page_fault(struct regs *rgs) {
    uint cr2;
    asm volatile ("movl %%cr2, %0":"=a"(cr2));
    printk("do_page_fault(): cr2= %x\n", cr2);
    for(;;);
}

int do_no_page(struct regs *rgs) {
    return 0;
}

int do_wp_page(struct regs *rgs) {
    return 0;
}

