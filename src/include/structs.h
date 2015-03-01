#ifndef __STRUCTS_H
#define __STRUCTS_H

struct vchar {
    short v_char:8;
    char v_fgcolor:4;
    char v_bgcolor:4;
};
#define SIZE_VCHAR sizeof(struct vchar)

// Define a struct of GDT
struct gdt_entry {
    uint limit_low:16;    // Limit of segment, low 16 bits
    uint base_low:16;    // Base address of segment, low 16 bits
    uint base_mid:8;    // Base address of segment, middle 8 bits
    uint attr_type:4;     // The type of segment
    uint attr_s:1;    // s = 1: data/code segment descriptor; s = 0: system segment descriptor
    uint attr_dpl:2;    // Descriptor Privilege Level of segment, RING0 ~ RING3
    uint attr_present:1;    // Whether segment present in memory
    uint limit_high:4;    // Limit of segment, high 4 bits
    uint attr_avl:1;    // Reserved
    uint attr_0:1;    // Always 0
    uint attr_db:1;    // Operand size (0 -> 16-bit, 1 -> 32-bit)
    uint attr_g:1;    // Granularity (0 -> 1B, 1 -> 4KB)
    uint base_high:8;    // High 8 bits of segment base address
} __attribute__ ((packed));
#define SIZE_GDT_ENTRY sizeof(struct gdt_entry)

struct gdt_ptr {
    ushort limit;
    uint base;
} __attribute__ ((packed));
#define SIZE_GDT_PTR sizeof(struct gdt_ptr)

struct idt_entry {
    uint base_low:16;    // base address of ISR (Interrupt Service Routine) 
    uint selector:16;     // segment selector
    uint zero:8;    // always 0
    uint attr_type:4;     // gate type
    uint attr_s:1;    // s = 0 for interrupt gates; must be 0
    uint attr_dpl:2;    // Descriptor Privilege Level
    uint attr_present:1;    // present
    uint base_high:16;    // higher part of base address
} __attribute__ ((packed));
#define SIZE_IDT_ENTRY sizeof(struct idt_entry)

struct idt_ptr {
    ushort limit;
    uint base;
} __attribute__ ((packed));

struct regs {
    uint gs, fs, es, ds;
    uint edi, esi, ebp, esp, ebx, edx, ecx, eax;    // pushed by `pusha`
    uint int_no, err_code;
    uint eip, cs, eflags, useresp, ss;    // pushed by processor 
};
#define SIZE_REGS sizeof(struct regs)

struct time {
    int tm_year;
    int tm_month;
    int tm_day;
    int tm_hour;
    int tm_minute;
    int tm_second;
};
#define SIZE_TIME sizeof(struct time)

#endif
