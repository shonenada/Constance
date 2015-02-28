#include <system.h>

void init() {
}

void kmain() {
    video_init();
    puts("Hello Constance!\n");
    gdt_init();
    idt_init();
    timer_init();
    keyboard_init();

    __asm__ __volatile__ ("sti");

    for(;;);
}
