#include <system.h>
#include <console.h>
#include <segment.h>
#include <page.h>
#include <time.h>
#include <sched.h>

void init() {
}

void kmain() {
    gdt_init();
    idt_init();
    video_init();
    timer_init();
    time_init();
    page_init();
    keyboard_init();
    sched_init();
    sti();

    puts("Hello Constance!\n");
    asm volatile ("int $0x80"::"a"(0));

    for(;;);
}
