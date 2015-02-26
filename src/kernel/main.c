#include <system.h>

void init() {
    init_video();
}

void kmain() {
    init();
    char * video_memory = (char *) 0xb8000;
    *video_memory = 'H';
    for(;;) {}
}
