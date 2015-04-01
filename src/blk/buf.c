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
    bfreelist.next = &bfreelist;
    bfreelist.prev = &bfreelist;
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
        if (!(bp->flag & B_BUSY) && !(bp->flag & B_DIRTY)) {
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
