#include <system.h>

void *memcpy(void *dest, const void *src, size_t count) {
    const char *sp = (const char *) src;
    char *dp = (char *) dest;
    for(;count>0;count--) *dp++ = *sp++;
    return dest;
}

void *memset(void *dest, char val, size_t count) {
    char *dp = (char *)dest;
    for (;count>0;count--) *dp++ = val;
    return dest;
}

ushort *memsetw(ushort *dest, ushort val, size_t count) {
    ushort *dp = (ushort *)dest;
    for (;count>0; count--) *dp++ = val;
    return dest;
}

size_t strlen(char *str) {
    char *sp;
    for (sp=str; *sp != '\0'; ++sp);
    return (size_t) (sp - str);
}

uchar inportb(ushort _port) {
    uchar rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb(ushort _port, uchar _data){
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}
