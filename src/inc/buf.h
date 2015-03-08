#ifndef __BUF_H
#define __BUF_H

#include <const.h>

struct buf {
    uint flag;
    uint dev;
    uint sector;
    struct buf *prev;
    struct buf *next;
    struct buf *queue;
    uchar data[512];
};

#define B_BUSY 0b1
#define B_VALID 0b01
#define B_DIRTY 0b001

void buffer_init();
struct buf* get_buffer(uint dev, uint sector);
void del_buffer(struct buf *b);
struct buf* read_buffer(uint dev, uint sector);
void write_buffer(struct buf* b);

#endif
