#include <system.h>

void init() {
    init_video();
}

void kmain() {
    init();
    putch('H');
    putch('\r');
    putch('\n');
    puts("Hello Constance!\n");
    for(;;) ;
}
