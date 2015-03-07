#include <system.h>
#include <const.h>
#include <console.h>
#include <segment.h>
#include <mm.h>
#include <time.h>
#include <sched.h>
#include <asm.h>

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

    for(;;);
}
