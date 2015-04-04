#include <system.h>
#include <segment.h>
#include <asm.h>
#include <sched.h>
#include <buf.h>
#include <blk.h>

struct dev hd_dev = {0, NULL, NULL, NULL, NULL};

// wait for hard disk
int hd_ready() {
    int retry = 100000;
    while (--retry || ((inportb(HD_STATUS)) & HD_BUSY))
        ;    // loop, do nothing
    return retry;
}

int hd_out(int sector_num, int sector, int dev_no, int rw) {
    hd_ready();

    outportb(0x3F6, 0);    // Interrupt
    outportb(HD_CMD_PORT0+2, sector_num);    // 0x1f2, the number of sectors
    outportb(HD_CMD_PORT0+3, (sector & 0xFF));    // 0x1f3, beginning of sector
    outportb(HD_CMD_PORT0+4, (sector >> 0x08 ) & 0xFF);    // 0x1f4, low 8 bits of cyl
    outportb(HD_CMD_PORT0+5, (sector >> 0x10 ) & 0xFF);    // 0x1f5, high 8 bits of cyl
    outportb(HD_CMD_PORT0+6, 0xE0 | ((dev_no & 1) << 4) | ((sector >> 0x18) & 0x0F));    // 0x1f6,
    outportb(HD_CMD_PORT0+7, rw);
    return 0;
}

int hd_start() {
    if (hd_dev.io_next == NULL || hd_dev.io_next == (struct buf*)&hd_dev) {
        // io list is empty
        return 0;
    }

    struct buf *bp;
    bp = hd_dev.io_prev;
    hd_dev.active = 1;
    if (bp->flag & B_DIRTY) {
        // write to disk
        if (DEBUG) {
            printk("write to disk\n");
        }
        hd_out(BLK_SIZE/PBLK, bp->sector * BLK_SIZE / PBLK, 0, HD_CMD_WRITE);
        outsl(0x1F0, bp->data, 512/4);
    } else {
        // read from disk
        hd_out(BLK_SIZE/PBLK, bp->sector * BLK_SIZE / PBLK, 0, HD_CMD_READ);
    }
    return 0;
}

// sync buffer with disk
// if B_DIRTY is set, write buffer to disk, reset B_DIRTY and set B_VALID
// if B_VALID is not set, read from disk and load into buffer, set B_VALID
int hd_sync(struct buf *bp) {
    if (!(bp->flag & B_BUSY))
        panic("hd_sync(): Buffer is not busy");
    if ((bp->flag & (B_VALID|B_DIRTY)) == B_VALID)
        return 0;
    if (bp->dev < 0)
        panic("hd_sync(): device not set");

    bp->io_prev = (struct buf *)(&hd_dev);
    bp->io_next = hd_dev.io_next;
    hd_dev.io_next->io_prev = bp;
    hd_dev.io_next = bp;

    if (hd_dev.active == 0) {
        hd_start();
    }

    return 0;
}

int do_hd_intr(struct regs *rgs) {
    struct buf *bp;
    if (hd_dev.active == 0)
        return 0;

    hd_dev.active = 0;
    bp = hd_dev.io_prev;
    bp->io_prev->io_next = bp->io_next;
    bp->io_next->io_prev = bp->io_prev;

    if (!(bp->flag & B_DIRTY)) {
        // read into memory from disk
        insl(0x1F0, bp->data, BLK_SIZE/4);
    }

    bp->flag |= B_VALID;
    bp->flag &= ~B_DIRTY;
    wakeup((uint) bp);
    hd_start();
    return 0;
}

void hd_init() {
    hd_dev.next = hd_dev.prev = (struct buf *)&hd_dev;
    hd_dev.io_next = hd_dev.io_prev = (struct buf *)&hd_dev;
    irq_install(0x0E, do_hd_intr);
}
