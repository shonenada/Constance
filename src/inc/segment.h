#ifndef __SEGMENT_H
#define __SEGMENT_H

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

/* GDT */
#define GDT_SIZE 5

// Descriptor type bits
#define SEG_DATA 0b0000    // Data segment
#define SEG_CODE 0b1000     // Code segment
#define SEG_TYPE_A 0b0001    // Accessed
#define SEG_TYPE_W 0b0010    // Writable
#define SEG_TYPE_R 0b0010    // Readable
#define SEG_TYPE_E 0b0100    // Expand-down
#define SEG_TYPE_C 0b0100    // Conforming

// Descriptor type for data segment on condition where gdt->s = 1
#define SEG_DATA_RD SEG_DATA    // 0x0 Read-Only
#define SEG_DATA_RDA (SEG_DATA | SEG_TYPE_A)   // 0x1 Read-Only, accessed
#define SEG_DATA_RW (SEG_DATA | SEG_TYPE_W) // 0x2 Read/Write
#define SEG_DATA_RWA (SEG_DATA | SEG_TYPE_W | SEG_TYPE_A) // 0x3 Read/Write, accessed
#define SEG_DATA_RDED (SEG_DATA | SEG_TYPE_E)    // 0x4 Read-Only, expand-down
#define SEG_DATA_RDEDA (SEG_DATA | SEG_TYPE_E | SEG_TYPE_A)    // 0x5 Read-Only, expand-down, accessed
#define SEG_DATA_RWED (SEG_DATA | SEG_TYPE_E | SEG_TYPE_W)    // 0x6 Read/Write, expand-down
#define SEG_DATA_RWEDA (SEG_DATA | SEG_TYPE_E | SEG_TYPE_W | SEG_TYPE_A)    // 0x7 Read/Write, expand-down, accessed

// Descriptor type for code segment on condition where gdt->s = 1
#define SEG_CODE_EX (SEG_CODE)    // 0x8 Execute-Only
#define SEG_CODE_EXA (SEG_CODE | SEG_TYPE_A)    // 0x9 Execute-Only, accessed
#define SEG_CODE_EXRD (SEG_CODE | SEG_TYPE_R)    // 0xA Execute/Read
#define SEG_CODE_EXRDA (SEG_CODE | SEG_TYPE_R | SEG_TYPE_A)    // 0xB Execute/Read, accessed
#define SEG_CODE_EXC (SEG_CODE | SEG_TYPE_C)    // 0xC Execute-Only, conforming
#define SEG_CODE_EXCA (SEG_CODE | SEG_TYPE_C | SEG_TYPE_A)    // 0xD Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC (SEG_CODE | SEG_TYPE_C | SEG_TYPE_R)    // 0xE Execute/Read, conforming
#define SEG_CODE_EXRDCA (SEG_CODE | SEG_TYPE_C | SEG_TYPE_R | SEG_TYPE_A)    // 0xF Execute/Read, conforming, accessed

// System Segment or Gate Descriptor type
// 0x0 Reserved
#define STS_TYPE_TTS16A 0x1     // 16-bit TTS (Available)
#define STS_TYPE_LDT 0x2     // LDT
#define STS_TYPE_TTS16B 0x3    // 16-bit TTS (Busy)
#define STS_TYPE_CG16 0x4    // 16-bit Call Gate
#define STS_TYPE_TG 0x5    // Task Gate
#define STS_TYPE_IG16 0x6    // 16-bit Interrupt Gate
#define STS_TYPE_TG16 0x7    // 16-bit Trap Gate
// 0x8 Reserved
#define STS_TYPE_TTS32A 0x9    // 32-bit TTS (Available)
// 0xA Reserved
#define STS_TYPE_TTS32B 0xB    // 32-bit TTS (Busy)
#define STS_TYPE_CG32 0xC    // 32-bit Call Gate
// 0xD Reserved
#define STS_TYPE_IG32 0xE    // 32-bit Interrupt Gate
#define STS_TYPE_TG32 0xF    // 32-bit Trap Gate

// Privilege of segment
#define RING0 0x0
#define RING1 0x1
#define RING2 0x2
#define RING3 0x3

void gdt_set(struct gdt_entry*, uint, uint, uint, uint);
extern void gdt_flush();
void gdt_init();
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

#endif
