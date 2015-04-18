#ifndef __LIB_H
#define __LIB_H

#include <const.h>

/* string.c */
void *memcpy(void*, const void*, size_t);
void *memset(void*, char, size_t);
ushort *memsetw(ushort*, ushort, size_t);
size_t strlen(char*);
uchar inportb(ushort _port);
void outportb(ushort, uchar);
int strcmp(char *lhp, char *rhp);
int strncmp(char *lhp, char* rhp, uint n);
char *strncpy (char *dst, const char *src, uint cnt); 
char* strchr(const char *str, char c);
void putch(char c);
void puts(char *str);

#endif
