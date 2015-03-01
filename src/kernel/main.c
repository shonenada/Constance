#include <system.h>

void init() {
}

void kmain() {
    gdt_init();
    idt_init();
    video_init();
    timer_init();
    time_init();
    keyboard_init();
    __asm__ __volatile__ ("sti");

    puts("Hello Constance!\n");

    for(;;);
}
