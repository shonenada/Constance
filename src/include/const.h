#ifndef __CONST_H
#define __CONST_H

/* Global type define */
typedef int size_t;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

/* Common variable */
#define KERN_CS 0x08    // kernel code segment

/* Define value colors */
#define VC_BLACK 0x0
#define VC_BLUE 0x1
#define VC_GREEN 0x2
#define VC_CYAN 0x3
#define VC_RED 0x4
#define VC_MAGENTA 0x5
#define VC_BROWN 0x6
#define VC_LIGHT_GREY 0x7
#define VC_DARK_GREY 0x8
#define VC_LIGHT_BLUE 0x9
#define VC_LIGHT_GREEN 0xA
#define VC_LIGHT_CYAN 0xB
#define VC_LIGHT_RED 0xC
#define VC_LIGHT_MEGENTA 0xD
#define VC_LIGHT_LIGHT_BROWN 0xE
#define VC_WHITE 0xF

#define VALUE_COLOR(BG, FG) ((unsigned char) (BG << 4 | FG))

#define VGA_BLACK (VALUE_COLOR(VC_BLACK, VC_BLACK))
#define VGA_WHITE (VALUE_COLOR(VC_BLACK, VC_WHITE))

#define SCR_BLANK (' ' | VGA_WHITE << 8)


/* GDT */
#define GDT_SIZE 4

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

#define MASTER_PIC_PORT 0x20
#define SLAVE_PIC_PORT 0xA0

#endif
