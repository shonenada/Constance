#include <buf.h>

struct {
    struct buf buf[NBUF];
    struct buf head;
} buffer;

// doubly-linked list
void buffer_init() {
    int i;
    buffer.head.prev = &buffer.head;
    buffer.head.next = &buffer.head;
    for (i=0;i<NBUF;i++) {
        buffer.buf[i].next = buffer.head.next;
        buffer.buf[i].prev = &buffer.head;
        buffer.buf[i].dev = -1;
        buffer.head.next->prev = &buffer.buf[i];
        buffer.head.next = &buffer.buf[i];
    }
}

struct buf* get_buffer(uint dev, uint sector) {
    struct buf* p;
    _loop:
    for (p=buffer.head.next;p!=&buffer.head;p=p->next) {
        if (p->dev == dev && p->sector == sector) {
            if (!(p->flag & B_BUSY)) {
                p->flag |= B_BUSY;
                return p;
            }
            // TODO: sleep
            goto _loop
        }
    }

    // cache not found
    for (p=buffer.head.prev;p!=buffer.head;p=p->prev) {
        if (!(p->flag & B_BUSY) == 0 && (p->flag & B_DIRTY) == 0) {
            p->dev = dev;
            p->sector = sector;
            p->flag |= B_BUSY;
            return p;
        }
    }
    panic("get_buffer: no buffers");
}

void del_buffer(struct buf *b) {
    if ((b->flag & B_BUSY) == 0) {
        panic("del_buffer: buffer deleted");
    }
    b->next->prev = b->prev;
    b->prev->next = b->next;
    b->next = buffer.head.next;
    b->prev = &buffer.head;
    buffer.head.next->prev = b;
    buffer.head.next = b;
    b->flag &= ~B_BUSY;

    // TODO: wakeup
}

struct buf* read_buffer(uint dev, uint sector) {
    struct buf* b;
    b = get_buffer(dev, sector);
    if (!(b->flag & B_VALID)) {
        // TODO: blk read/write
    }
    return b;
}

void write_buffer(struct buf* b) {
    if ((b->flag & B_BUSY) == 0)
        panic("read_buffer: buffer busy");
    b->flag |= B_DIRTY;
    // TODO: blk read/write
}

