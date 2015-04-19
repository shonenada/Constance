#include <system.h>
#include <segment.h>
#include <console.h>
#include <keybd.h>
#include <tty.h>

static uint mode = 0;

/**
 * Detects status of shift, ctrl, alt and capslock
 *
 */ 
uchar translate(char chr) {
    char ch = chr & 0x7F;    // 0b01111111

    if (mode & E0SC) {
        switch (ch) {
            case 0x1D:    // ctrl press 
                return CTRL;
            case 0x38:    // alt press 
                return ALT;
        }
    } else {
        switch (ch) {
            case 0x2A:    // left shift press
            case 0x36:    // right shift press
                return SHIFT;
            case 0x1D:    // left ctrl press
                return CTRL;
            case 0x38:    // left alt press
                return ALT;
        }
    }
    return 0;
}

/**
 * Interrupt Service Routinue for keyboard
 * Scan codes refer to http://wiki.osdev.org/PS/2_Keyboard
 */
int keyboard_handler(struct regs *rgs) {
    uchar kbcode, mask, chr;
    uchar *map;

    kbcode = inportb(KB_DATA);    // Read from data buffer of keyboard

    // 0x3A, Cap Lock pressed
    if ((kbcode & 0x7F) == 0x3A) {
        return 0;
    }

    // some extended key hold 2 ~ 4 bytes scan code
    if (kbcode == 0xE0) {
        mode |= E0SC;
    }

    if ((mask = translate(kbcode))) {
        if (kbcode & 0x80) {
            // Release a key
            mode &= ~mask;
        } else {
            mode |= mask;
        }
        return 0;
    }

    map = (mode & SHIFT) ? shift_key_map : key_map;

    chr = map[kbcode & 0x7F];
    
    if (mode & CTRL) {
        // TODO: send signal
    }

    if (!(kbcode & 0x80)) {
        if (map[kbcode] != '\0')
            tty_input(&ttys[0], map[kbcode]);
        // putch(map[kbcode]);
    } else {
        mode &= ~E0SC;
    }
    return 0;
}

void keyboard_init() {
    irq_install(1, keyboard_handler);
}
