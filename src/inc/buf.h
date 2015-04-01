#ifndef __BUF_H
#define __BUF_H

#include <const.h>

struct buf {
    uint flag;
    uint dev;
    uint sector;
    struct buf *prev;
    struct buf *next;
    struct buf *io_prev;
    struct buf *io_next;
    uchar data[BLK_SIZE];
};

#define B_BUSY 0b1
#define B_VALID 0b01
#define B_DIRTY 0b001
#define B_ERROR 0b0001

extern struct buf buffer[NBUF];
extern struct buf bfreelist;

void buf_init();
struct buf* buf_get(uint dev, uint sector);
int buf_relse(struct buf* bp);
struct buf* buf_read(uint dev, uint sector);
void buf_write(struct buf* b);

#endif
