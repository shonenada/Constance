#include <system.h>

// Global variable for vga memory (0xb8000)
unsigned short *vgamemptr;
// Attributes of background color, foreground color
int attrib = 0x0F;
// x and y cursor coordinates;
int csr_x = 0, csr_y = 0;

// Scroll screen
void scroll(void) {
    unsigned blank, temp;
    blank = (' ' | (attrib << 8));    // Space, set background as well.

    // Max row number is 25, we need to scroll up
    if (csr_y >= 25) {
        // Move current cursor to the top of screen.
        temp = csr_y - 25 + 1;
        memcpy (vgamemptr, vgamemptr + temp * 80, (25 - temp) * 80 * 2);
        memsetw (vgamemptr + (25 - temp) * 80, blank, 80);
        csr_y =  25 - 1;
    }
}

void move_csr(void) {
    unsigned temp;
    temp = csr_y * 80 + csr_x;
    outportb(0x3D4, 14);
    outportb(0x3D5, temp >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, temp);
}

// Clean the screen
void cls(void) {
    unsigned blank = (' ' | (attrib << 8));
    memsetw(vgamemptr, blank, 80*25);
    csr_x = 0;
    csr_y = 0;
    move_csr();
}

// Put a char on the screen
void putch(unsigned char c) {
    unsigned short *where;
    unsigned att = attrib << 8;    // background color

    // Handle for special char.
    // backspace
    if (c == 0x08) {
        if (csr_x != 0)
            csr_x--;
    }
    // tab
    else if (c == 0x09) {
       csr_x = (csr_x + 8) & ~(8 - 1);
    }
    else if (c == '\r') {
        csr_x = 0;
    }
    else if (c == '\n') {
        csr_x = 0;
        csr_y++;
    }
    // printable
    else if (c >= ' ') {
        where = vgamemptr + (csr_y * 80 + csr_x);
        *where = c | att;
        csr_x++;
    }
    if (csr_x >= 80) {
        csr_x = 0;
        csr_y++;
    }
    scroll();
    move_csr();
}

// Put a string on the screen
void puts(unsigned char *text) {
    int i;
    for (i=0; i<strlen(text); i++) {
        putch(text[i]);
    }
}

void settextcolor(unsigned char forecolor, unsigned char backcolor) {
    attrib = (backcolor << 4) | (forecolor & 0x0f);
}

void init_video(void) {
    vgamemptr = (unsigned short *) 0xb8000;
    cls();
}
