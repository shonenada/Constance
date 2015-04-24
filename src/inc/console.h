#ifndef __CONSOLE_H
#define __CONSOLE_H

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

struct vchar {
    short v_char:8;
    char v_fgcolor:4;
    char v_bgcolor:4;
};
#define SIZE_VCHAR sizeof(struct vchar)

void cls(void);
void putch(char);
void puts(char*);
void settextcolor(uchar, uchar);
void video_init(void);
void print_number(uint, uint);
void printk(char*, ...);

int csr_x;
int csr_y;

#endif
