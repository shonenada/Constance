#include <system.h>
#include <console.h>

volatile void panic(const char* s) {
    printk("Kernel panic: %s\n\r", s);
    for(;;);
}
