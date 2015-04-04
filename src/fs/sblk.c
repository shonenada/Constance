#include <const.h>
#include <system.h>
#include <asm.h>
#include <sched.h>
#include <inode.h>
#include <fs.h>
#include <buf.h>
#include <blk.h>

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
    panic("no fs\n");
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

void putsp(struct sblk* sbp) {
    unlink_sb(sbp);
}

// alloc block
int balloc (ushort dev) {
    struct sblk *sbp;
    struct buf *bp;

    int i, bl, bn;

    sbp = getsblk(dev);
    for (i=0;i<sbp->nzones;i++) {
        bp = buf_read(dev,BBLOCK(sbp, i));
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
    bp = buf_read(dev, BBLOCK(sbp, bn));
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
    bp = buf_get(dev, bn);
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
        bp = buf_read(dev, IBLOCK(i));
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
    bp = buf_read(dev, IBLOCK(ino));
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

int sblk_load(struct sblk *sbp) {
    struct buf *bp;
    bp = buf_read(sbp->dev, 1);
    if ((bp->flag & B_ERROR) != 0) {
        panic("sblk_load(): disk read error");
        return -1;
    }
    memcpy(sbp, bp->data, sizeof(struct d_sblk));
    buf_relse(bp);
    if (sbp->magic != S_MAGIC) {
        panic("sblk_load(): inavailible device");
        return -1;
    }
    sbp->imnt = NULL;
    return 0;
}

int sblk_update(struct sblk *sbp) {
    struct buf *bp;
    bp = getsblk(sbp->dev);
    memcpy(bp->data, (char*) sbp, sizeof(struct d_sblk));
    hd_sync(bp);
    return 0;
}

void dump_sblk(struct sblk* blk) {
    printk("superblock(%x): dev=%x, flag=%x, max_size=%x, magic=%x, state=%x, ninode=%x, nzones=%x, imap_blks=%x, zmap_blks=%x\n", blk, blk->dev, blk->flag, blk->max_size, blk->magic, blk->state, blk->ninodes, blk->nzones, blk->imap_blks, blk->zmap_blks);
}

void hexdump_sblk(struct sblk* blk) {
    int i;
    char * str = (char*)(blk);
    for(i=0;i<sizeof(struct d_sblk);i++) {
        printk("%x ", str[i]);
    }
    printk("\n");
}
