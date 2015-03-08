#include <const.h>
#include <system.h>
#include <asm.h>
#include <sched.h>
#include <inode.h>
#include <fs.h>
#include <buf.h>
#include <blk.h>

struct sblk mnt[NMOUNT] = { {0, }, };

struct sblk *getsblk(ushort dev) {
    struct sblk *sbp;
_loop:
    for (sbp=mnt;sbp<mnt+NMOUNT;sbp++) {
        if (dev == sbp->dev) {
            if (sbp->flag & S_LOCK) {
                sbp->flag |= S_WANTED;
                sleep((uint) sbp);
                goto _loop;
            }
            return sbp;
        }
    }
    panic("no fs");
    return NULL;
}

void unlink_sb(struct sblk *sb) {
    cli();
    if (sb->flag & S_WANTED) {
        wakeup((uint)sb);
    }
    sb->flag &= ~(S_LOCK | S_WANTED);
    sti();
}

// alloc block
int balloc (ushort dev) {
    struct sblk *sbp;
    struct buf *bp;

    int i, bl, bn;

    sbp = getsblk(dev);
    for (i=0;i<sbp->nzones;i++) {
        bp = read_buffer(dev,BBLOCK(sbp, i));
        bl = find_free(bp->data, BLK_SIZE);
        if (bl < 0) {
            continue;
        }
        bn = i * BPB + bl;
        bp->data[bn/8] |= 1 << (bn % 8);
        hd_sync(bp);
        unlink_sb(sbp);
        return sbp->zone0 + bn;
    }
    unlink_sb(sbp);
    panic("no free block");
    return -1;
}

// free block 
int bfree (ushort dev, uint nr) {
    struct buf* bp;
    struct sblk *sbp;
    int bn;

    sbp = getsblk(dev);
    if ((nr < sbp->zone0) || (nr >= sbp->nzones)) {
        panic("block not in data zones");
    }
    bn = nr + 1 - sbp->zone0;
    bp = read_buffer(dev, BBLOCK(sbp, bn));
    if ((bp->data[bn/8] & (1 << (bn%8))) == 0) {
        panic("freeing free block");
    }
    bp->data[bn/8] &= ~(1 << (bn%8));
    hd_sync(bp);
    unlink_sb(sbp);
    return 0;
}

// alloc zero block
int bzero (ushort dev, uint bn) {
    struct buf *bp;
    bp = get_buffer(dev, bn);
    memset(bp->data, 0, BLK_SIZE);
    hd_sync(bp);
    return 0;
}

// alloc inode
int ialloc (ushort dev) {
    uint i, ino;
    int r;
    struct sblk *sbp;
    struct buf *bp;
    sbp = getsblk(dev);

    for (i=0;i<sbp->ninodes;i++) {
        bp = read_buffer(dev, IBLOCK(i));
        r = find_free(bp->data, BLK_SIZE);
        if (r < 0) {
            continue;
        }
        ino = i*BLK_SIZE + r;
        bp->data[ino/8] |= 1 << (ino%8);
        hd_sync(bp);
        unlink_sb(sbp);
        return ino;
    }
    unlink_sb(sbp);
    panic("no enough inode");
    return -1;
}

// free inode
int ifree (ushort dev, uint ino) {
    struct buf *bp;
    struct sblk *sbp;
    sbp = getsblk(dev);
    if ((ino <= 0) || (ino >= sbp->ninodes)) {
        panic("inode no exist");
    }
    bp = read_buffer(dev, IBLOCK(ino));
    if ((bp->data[ino/8] & (1 << (ino%8))) == 0) {
        panic("inode is free");
    }
    bp->data[ino/8] &= ~(1<<(ino%8));
    hd_sync(bp);
    unlink_sb(sbp);
    return 0;
}

// find free block by bitmap
int find_free(uchar *bitmap, int size) {
    int byte, i, j;
    for (i=0;i<size;i++) {
        byte = bitmap[i];
        if (0 == ~byte) {
            continue;
        }
        for (j=0;j<8;j++) {
            if ((byte & (1 << j)) == 0) {
                return (i*8+j);
            }
        }
    }
    return -1;
}
