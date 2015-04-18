#include <usr/lib.h>
#include <unistd.h>

void putch(char c) {
    write(1, &c, 1);
}

void puts(char *str) {
    int i;
    for(i=0; i<strlen(str); i++) {
        putch(str[i]);
    }
}
