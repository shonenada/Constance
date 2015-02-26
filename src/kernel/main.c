#include <system.h>

void init() {
    init_video();
}

void kmain() {
    init();
    puts("Hello Constance!\n");
    for(;;) ;
}
