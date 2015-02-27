#include <system.h>

void init() {
}

void kmain() {
    gdt_init();
    idt_init();
    video_init();
    puts("Hello Constance!\n");
    for(;;) ;
}
