#include <system.h>
#include <console.h>

// Global variable for vga memory (0xb8000), set in kern.ld
extern struct vchar vgamemptr[25][80];
// Attributes of background color, foreground color
int attrib = 0x0F;
// x and y cursor coordinates;
int csr_x = 0, csr_y = 0;

void flush_csr(void) {
    uint pos = csr_y * 80 + csr_x;
    outportb(0x3D4, 14);
    outportb(0x3D5, pos >> 8);
    outportb(0x3D4, 15);
    outportb(0x3D5, pos);
}

// Clean the screen
void cls(void) {
    memsetw((ushort*) vgamemptr, SCR_BLANK, 80*25);
    csr_x = 0;
    csr_y = 0;
    flush_csr();
}

// Scroll screen
void scroll(void) {
    // Max row number is 25, we need to scroll up
    if (csr_y >= 25) {
        // Move current cursor to the top of screen.
        uint pos = csr_y - 25 + 1;
        memcpy(vgamemptr, &vgamemptr[pos][0], (25 - pos) * 80 * SIZE_VCHAR);
        memsetw((ushort*)&vgamemptr[25-pos][0], SCR_BLANK, 80);
        csr_y =  25 - 1;
    }
}

// Put a char on the screen
void putch(char c) {
    // Handle for special char.
    // backspace
    if (c == '\b') {
        if (csr_x > 0) {
            vgamemptr[csr_y][csr_x].v_char = ' ';
            csr_x--;
        }
        else if (csr_x == 0){
            if (csr_y > 0) csr_y--;
            csr_x = 79;
            vgamemptr[csr_y][csr_x].v_char = ' ';
        }
    }
    // tab
    else if (c == '\t') {
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
        vgamemptr[csr_y][csr_x].v_char = c;
        csr_x++;
    }
    if (csr_x >= 80) {
        csr_x = 0;
        csr_y++;
    }
    scroll();
    flush_csr();
}

// Put a string on the screen
void puts(char *text) {
    int i;
    for (i=0; i<strlen((char *)text); i++) {
        putch(text[i]);
    }
}

void print_number(uint num, uint base) {
    static char * numbers = "0123456789ABCDEF";
    if (num > base) {
        print_number(num/base, base);
    }
    putch(numbers[num % base]);
}

void printk(char *fmt, ...) {
    char c;
    int* arg = (int*)(void*)&fmt + 1;
    while((c = *fmt++) != '\0') {
        if (c == '%') {
            switch(*fmt) {
                case 'c':
                    putch(*arg++);
                    break;
                case 's':
                    puts((char*)(*arg++));
                    break;
                case 'o':
                    print_number((unsigned long) *arg++, 8);
                    break;
                case 'x':
                    print_number((unsigned long) *arg++, 16);
                    break;
                case 'd':
                case 'l':
                    print_number((unsigned long) *arg++, 10);
                    break;
            }
            fmt++;
        } else {
            putch(c);
        }
    }
}

void settextcolor(uchar forecolor, uchar backcolor) {
    attrib = (backcolor << 4) | (forecolor & 0x0f);
}

void video_init(void) {
    cls();
}
