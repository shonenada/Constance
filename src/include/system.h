#ifndef __SYSTEM_H
#define __SYSTEM_H

typedef int size_t;

/* string.c */
void *memcpy(void*, const void*, size_t);
void *memset(void *, char, size_t);
unsigned short *memsetw(unsigned short*, unsigned short, size_t);
size_t strlen(const char*);
unsigned char inportb(unsigned short _port);
void outportb(unsigned short, unsigned char);

/* scrn.c */
void cls();
void putch(unsigned char);
void puts(unsigned char*);
void settextcolor(unsigned char, unsigned char);
void init_video();

#endif
