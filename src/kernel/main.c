#include <system.h>

void init() {
}

void kmain() {
    video_init();
    puts("Hello Constance!\n");
    puts("Started initializing kernel \n");

    puts("Started initializing Global Descriptor Table\n");
    gdt_init();
    puts("Finished initializing Global Descriptor Table\n");

    puts("Started initializing Interrupt Descriptor Table\n");
    idt_init();
    puts("Finished initializing Interrupt Descriptor Table\n");

    puts("Started initializing Interrupt Service Routines\n");
    isrs_init();
    puts("Finished initializing Interrupt Service Routines\n");

    puts("Started initializing Interrupt Request\n");
    irq_init();
    puts("Finished initializing Interrupt Request\n");

    __asm__ __volatile__ ("sti");

    for(;;);
}
