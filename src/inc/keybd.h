#ifndef __KEYBD_H
#define __KEYBD_H

#define KEY_HOME 0xE0
#define KEY_END 0xE1
#define KEY_UP 0xE2
#define KEY_DOWN 0xE3
#define KEY_LEFT 0xE4
#define KEY_RIGHT 0xE5
#define KEY_PGUP 0xE6
#define KEY_PGDOWN 0xE7
#define KEY_INS 0xE8
#define KEY_DEL 0xE9
#define KP_ENTER '\n'
#define KP_DIV '/'

#define KB_DATA 0x60
#define EXT_SC 0xE0    // extended keyboard scan code

// modes
#define SHIFT 0x1
#define CTRL 0x2
#define ALT 0x4
#define E0SC 0x8
#define CAPSLOCK 0x10

uchar key_map[256] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6',
    '+', '1', '2', '3', '0', '.', 0, 0, 0, 0,
    [0x9C] KP_ENTER, [0xB5] KP_DIV, [0xC8] KEY_UP, [0xD0] KEY_DOWN,
    [0xC9] KEY_PGUP, [0xD1] KEY_PGDOWN, [0xC8] KEY_LEFT, [0xCD] KEY_RIGHT,
    [0x97] KEY_HOME, [0xCF] KEY_END, [0xD2] KEY_INS, [0xD3] KEY_DEL
};

uchar shift_key_map[256] = {
    0, 033, '!', '@', '#' , '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6',
    '+', '1', '2', '3', '0', '.', 0, 0, 0, 0,
    [0x9C] KP_ENTER, [0xB5] KP_DIV, [0xC8] KEY_UP, [0xD0] KEY_DOWN,
    [0xC9] KEY_PGUP, [0xD1] KEY_PGDOWN, [0xC8] KEY_LEFT, [0xCD] KEY_RIGHT,
    [0x97] KEY_HOME, [0xCF] KEY_END, [0xD2] KEY_INS, [0xD3] KEY_DEL
};

#endif
