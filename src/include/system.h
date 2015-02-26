#ifndef __SYSTEM_H
#define __SYSTEM_H

/* main.c */
unsigned char *memcpy(unsigned char*, const unsigned char*, int);
unsigned char *memset(unsigned char*, unsigned char, int);
unsigned short *memsetw(unsigned short*, unsigned short, int);
int strlen(const char*);
unsigned char inportb(unsigned short _port);
void outportb(unsigned short, unsigned char);

/* scrn.c */
void cls();
void putch(unsigned char);
void puts(unsigned char*);
void settextcolor(unsigned char, unsigned char);
void init_video();

#endif
