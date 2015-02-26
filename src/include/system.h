#ifndef __SYSTEM_H
#define __SYSTEM_H

/* string.c */
void *memcpy(void*, const void*, size_t);
void *memset(void*, char, size_t);
ushort *memsetw(ushort*, ushort, size_t);
size_t strlen(char*);
uchar inportb(ushort _port);
void outportb(ushort, uchar);

/* scrn.c */
void cls(void);
void putch(char);
void puts(char*);
void settextcolor(uchar, uchar);
void init_video(void);

#endif
