#include <system.h>

unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count) {
    int i;
    unsigned char *sp = (unsigned char *) src;
    unsigned char *dp = dest;
    for (i=0; i<count; i++) {
        *dp++ = *sp++;
    }
    return dest;
}

unsigned char *memset(unsigned char *dest, unsigned char val, int count) {
    int i;
    unsigned char *dp = dest;
    for (i=0; i<count; i++) {
        *dp++ = val;
    }
    return dest;
}

unsigned short *memsetw(unsigned short *dest, unsigned short val, int count) {
    int i;
    unsigned short *dp = dest;
    for (i=0; i<count; i++) {
        *dp++ = val;
    }
    return dest;
}

/**
int strlen(const char *str) {
    int length;
    length = 0;
    while(str[length++] == '\0');
    return length;
}
**/

int strlen(const char *str) {
    char *sp; 
    for (sp=(char*)str; *sp!='\0'; ++sp);
    return sp - str;
}

unsigned char inportb(unsigned short _port) {
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb(unsigned short _port, unsigned char _data){
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void init() {
    init_video();
}

void kmain() {
    init();
    char * video_memory = (char *) 0xb8000;
    *video_memory = 'H';
    for(;;) {}
}
