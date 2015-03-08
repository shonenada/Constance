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

inline void insl(int port, void* addr, int cnt) {
    asm volatile ("cld; rep insl" :
                  "=D"(addr), "=c"(cnt) :
                  "d"(port), "0" (addr), "1" (cnt) :
                  "memory", "cc");
}

inline void outsl(int port, const void* addr, int cnt) {
    asm volatile ("cld; rep outsl" : 
                  "=S" (addr), "=c" (cnt) :
                  "d" (port), "0" (addr), "1" (cnt) :
                  "cc");
}
