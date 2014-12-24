#ifndef __SYSTEM_H
#define __SYSTEM_H

/* main.c */
extern unsigned char *memcpy(unsigned char*, const unsigned char*, int);
extern unsigned char *memset(unsigned char*, unsigned char, int);
extern unsigned short *memsetw(unsigned short*, unsigned short, int);
extern int strlen(const char*);
extern unsigned char inportb(unsigned short _port);
extern void outportb(unsigned short, unsigned char);

#endif
