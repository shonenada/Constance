#include <system.h>
#include <console.h>
#include <segment.h>
#include <time.h>

void init() {
}

void kmain() {
    gdt_init();
    idt_init();
    video_init();
    timer_init();
    time_init();
    keyboard_init();
    asm volatile("sti");

    puts("Hello Constance!\n");

    for(;;);
}
