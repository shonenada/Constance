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

/* scrn.c */
void cls(void);
void putch(char);
void puts(char*);
void settextcolor(uchar, uchar);
void video_init(void);

/* gdt.c */
void gdt_set(struct gdt_entry*, uint, uint, uint, uint);
extern void gdt_flush();
void gdt_init();

/* trap.c */
void idt_set(int num, uint, ushort, uchar, uchar);
inline void set_itr_gate(int num, uint);
inline void set_trap_gate(int num, uint);
inline void set_task_gate(int num, uint);
void isrs_init();
void idt_init();

void fault_handler(struct regs*);

#endif
