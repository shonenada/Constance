#include <const.h>
#include <asm.h>

inline volatile void cli() {
    asm volatile ("cli");
}

inline volatile void sti() {
    asm volatile ("sti");
}

inline volatile void ltr(uint n) {
    asm volatile ("ltr %%ax" :: "a"(n));
}

inline volatile void lldt(uint n) {
    asm volatile ("lldt %%ax"::"a"(n));
}
