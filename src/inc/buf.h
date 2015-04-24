#ifndef __BUF_H
#define __BUF_H

#include <const.h>

struct buf {
    uint flag;
    struct buf *prev;
    struct buf *next;
    struct buf *io_prev;
    struct buf *io_next;
    uint dev;
    uint sector;
    uchar data[BLK_SIZE];
};

struct dev {
    uint active;
    struct buf *prev;
    struct buf *next;
    struct buf *io_prev;
    struct buf *io_next;
};

#define B_BUSY 0b1
#define B_VALID 0b10
#define B_DIRTY 0b100
#define B_ERROR 0b1000

extern struct buf buffer[NBUF];
extern struct buf bfreelist;
extern struct dev hd_dev;

void buf_init();
struct buf* buf_get(uint dev, uint sector);
int buf_relse(struct buf* bp);
struct buf* buf_read(uint dev, uint sector);
int buf_write(struct buf* b);

void dump_buf(struct buf* buf);
void hexdump_buf(struct buf* buf);
void dump_buffer_freelist();

#endif
