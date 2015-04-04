#include <const.h>
#include <system.h>
#include <segment.h>
#include <sched.h>
#include <fs.h>
#include <buf.h>
#include <blk.h>

struct buf buffer[NBUF];
struct buf bfreelist;

/**
 * Initialize for Buffer Block.
 *
 * Initialize linked list of free list of buffer.
 */
void buf_init() {
    int i;
    bfreelist.prev = &bfreelist;
    bfreelist.next = &bfreelist;
    for (i=0;i<NBUF;i++) {
        buffer[i].next = bfreelist.next;
        buffer[i].prev = &bfreelist;
        buffer[i].dev = NODEV;
        bfreelist.next->prev = &buffer[i];
        bfreelist.next = &buffer[i];
    }
}

struct buf* buf_get(uint dev, uint sector) {
    struct buf* bp;

_loop:
    // Search 
    bp = bfreelist.next;
    while (bp != &bfreelist) {
        if (bp->dev == dev && bp->sector == sector) {
            // found, check flags
            if (!(bp->flag & B_BUSY)) {
                bp->flag |= B_BUSY;
                return bp;
            } else {
                sleep((uint) bp);
                goto _loop;
            }
        }
        bp = bp->next;
    }

    // not found
    bp = bfreelist.prev;
    while (bp != &bfreelist) {
        if ((!(bp->flag & B_BUSY)) && (!(bp->flag & B_DIRTY))) {
            bp->dev = dev;
            bp->sector = sector;
            bp->flag |= B_BUSY;
            return bp;
        }
        bp = bp->prev;
    }
    panic("buf_get(): no buffers");
    return NULL;
}

int buf_relse(struct buf* bp) {
    if (!(bp->flag & B_BUSY)) {
        panic("buf_relse(): buffer released");
        return -1;
    }

    bp->next->prev = bp->prev;
    bp->prev->next = bp->next;
    bp->next = &bfreelist.next;
    bp->prev = &bfreelist;
    bfreelist.next->prev = bp;
    bfreelist.next = bp;

    bp->flag &= ~B_BUSY;

    wakeup((uint) bp);
    return 0;
}

struct buf* buf_read(uint dev, uint sector) {
    struct buf *bp;
    bp = buf_get(dev, sector);

    if (bp == NULL) {
        panic("buf_read(): not buf for dev and sector");
    }

    if (!(bp->flag & B_VALID)) {
        hd_sync(bp);
    }
    return bp;
}

int buf_write(struct buf *bp) {
    if (!(bp->flag & B_BUSY)) {
        panic("buf_write(): buffer block is not busy");
        return -1;
    }
    bp->flag |= B_DIRTY;
    hd_sync(bp);
}

void dump_buffer_freelist() {
    int i;
    for(i=0;i<NBUF;i++) {
        printk("%d(%x): dev=%d, prev=%x, next=%x\n", i, &buffer[i], buffer[i].dev, buffer[i].prev, buffer[i].next);
    }
    printk("free=%x, prev=%x, next=%x\n", &bfreelist, bfreelist.prev, bfreelist.next);
}

void dump_buf(struct buf* buf) {
    int i;
    char* str = buf->data;
    printk("buf(%x): flag=%d, dev=%x, sector=%x\n", buf, buf->flag, buf->dev, buf->sector);
    printk("data: ");
    for(i=0;i<100;i++) {
        printk("%x ", str[i]);
    }
    printk("\n");
}

void hexdump_buf(struct buf* buf) {
    int i;
    char * str = (char*)(buf);
    for(i=0;i<80;i++) {
        printk("%x", str[i]);
        if (i % 4 == 0) {
            printk(" ");
        }
         if (i % 32 == 0) {
            printk("\n");
        }
           }
    printk("\n");
}
