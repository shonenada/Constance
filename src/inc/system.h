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

/* console.c */
void cls(void);
void putch(char);
void puts(char*);
void settextcolor(uchar, uchar);
void video_init(void);
void print_number(uint, uint);
void printk(char*, ...);

/* gdt.c */
void gdt_set(struct gdt_entry*, uint, uint, uint, uint);
extern void gdt_flush();
void gdt_init();

/* trap.c */
void idt_set(int num, uint, ushort, uchar, uchar);
inline void set_itr_gate(int num, uint);
inline void set_trap_gate(int num, uint);
inline void set_task_gate(int num, uint);
inline void irq_install(int, void(*)(struct regs*));
inline void irq_uninstall(int);
void irq_remap();
void isrs_init();
void idt_init();
void irq_init();

void int_handler(struct regs*);

void timer_init();
void keyboard_init();

void time_init();
long timestamp();

volatile void panic(const char* s);

#endif
