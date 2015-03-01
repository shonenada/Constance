#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <const.h>
#include <structs.h>

/* string.c */
void *memcpy(void*, const void*, size_t);
void *memset(void*, char, size_t);
ushort *memsetw(ushort*, ushort, size_t);
size_t strlen(char*);
uchar inportb(ushort _port);
void outportb(ushort, uchar);

volatile void panic(const char* s);

#endif
