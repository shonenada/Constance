#include <system.h>

volatile void panic(const char* s) {
    printk("Kernel panic: %s\n\r", s);
    for(;;);
}
