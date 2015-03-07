#include <const.h>
#include <asm.h>

inline void cli() {
    asm volatile ("cli");
}

inline void sti() {
    asm volatile ("sti");
}

inline void ltr(uint n) {
    asm volatile ("ltr %%ax" :: "a"(n));
}

inline void lldt(uint n) {
    asm volatile ("lldt %%ax"::"a"(n));
}
