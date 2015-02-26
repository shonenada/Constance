#ifndef __SYSTEM_H
#define __SYSTEM_H

/* string.c */
void *memcpy(void*, const void*, size_t);
void *memset(void *, char, size_t);
unsigned short *memsetw(unsigned short*, unsigned short, size_t);
size_t strlen(char*);
unsigned char inportb(unsigned short _port);
void outportb(unsigned short, unsigned char);

/* scrn.c */
void cls(void);
void putch(char);
void puts(char*);
void settextcolor(unsigned char, unsigned char);
void init_video(void);

#endif
