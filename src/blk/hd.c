#include <system.h>
#include <asm.h>
#include <buf.h>
#include <blk.h>

struct buf * hdqueue;

// wait for hard disk
int hd_ready() {
    int retry = 100000;
    while (--retry && ((inportb(HD_STATUS) & (HD_READ | HD_BUSY)) != HD_READ))
        ;    // loop, do nothing
    return retry;
}

int hd_out(int sector_num, int sector, int dev_no, int rw) {
    if (!hd_ready())
        panic("HD Controller is not ready");

    outportb(0x3F6, 0);    // Interrupt
    outportb(HD_CMD_PORT0+2, sector_num);    // 0x1f2, the number of sectors
    outportb(HD_CMD_PORT0+3, (sector & 0xFF));    // 0x1f3, beginning of sector
    outportb(HD_CMD_PORT0+4, (sector >> 0x08 ) & 0xFF);    // 0x1f4, low 8 bits of cyl
    outportb(HD_CMD_PORT0+5, (sector >> 0x10 ) & 0xFF);    // 0x1f5, high 8 bits of cyl
    outportb(HD_CMD_PORT0+6, 0xE0 | ((dev_no & 1) << 4) | ((sector >> 0x18) & 0x0F));    // 0x1f6,
    outportb(HD_CMD_PORT0+7, rw);
    return 0;
}

int hd_start(struct buf *bp) {
    if (bp->flag & B_DIRTY) {
        hd_out(1, bp->sector, bp->dev, HD_CMD_WRITE);
        outsl(0x1F0, bp->data, 512/4);
    } else {
        hd_out(1, bp->sector, bp->dev, HD_CMD_READ);
    }
}

// sync buffer with disk
// if B_DIRTY is set, write buffer to disk, reset B_DIRTY and set B_VALID
// if B_VALID is not set, read from disk and load into buffer, set B_VALID
int hd_sync(struct buf *b) {
    struct buf **p;

    if (!(b->flag & B_BUSY))
        panic("hd_sync: Buffer is not busy");
    if (b->flag & (B_VALID|B_DIRTY) == B_VALID)
        panic("hd_sync: synced");
    if (b->dev < 0)
        panic("hd_sync: device not set");

    b->queue = 0;
    for (p=&hdqueue;*p;p=&(*p)->next)
        ;
    *p = b;

    if (hdqueue == b)
        hd_start(b);

    while((b->flag & (B_VALID|B_DIRTY)) != B_VALID)
        sleep(b);
}
