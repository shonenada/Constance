#include <system.h>

void init() {
}

void kmain() {
    gdt_init();
    video_init();
    init();
    puts("Hello Constance!\n");
    for(;;) ;
}
