#ifndef __CONST_H
#define __CONST_H

/* Global type define */
typedef int size_t;
typedef unsigned uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

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

// GDT type for data segment on condition where gdt->s = 1
#define SEG_DATA_RD 0x0    // Read-Only
#define SEG_DATA_RDA 0x1    // Read-Only, accessed
#define SEG_DATA_RW 0x2    // Read/Write
#define SEG_DATA_RWA 0x3    // Read/Write, accessed
#define SEG_DATA_RDED 0x4    // Read-Only, expand-down
#define SEG_DATA_RDEDA 0x5    // Read-Only, expand-down, accessed
#define SEG_DATA_RWED 0x6    // Read/Write, expand-down
#define SEG_DATA_RWEDA 0x7    // Read/Write, expand-down, accessed

// GDT type for code segment on condition where gdt->s = 1
#define SEG_CODE_EX 0x8    // Execute-Only
#define SEG_CODE_EXA 0x9    // Execute-Only, accessed
#define SEG_CODE_EXRD 0xA    // Execute/Read
#define SEG_CODE_EXRDA 0xB    // Execute/Read, accessed
#define SEG_CODE_EXC 0xC    // Execute-Only, conforming
#define SEG_CODE_EXCA 0xD    // Execute-Only, conforming, accessed
#define SEG_CODE_EXRDC 0xE    // Execute/Read, conforming
#define SEG_CODE_EXRDCA 0xF    // Execute/Read, conforming, accessed

// Privilege of segment
#define RING0 0x0
#define RING1 0x1
#define RING2 0x2
#define RING3 0x3

#endif
