#include <const.h>
#include <system.h>
#include <asm.h>
#include <sched.h>
#include <fs.h>
#include <inode.h>
#include <blk.h>
#include <stat.h>
#include <buf.h>

struct inode inodes[NINODE];

struct inode * iget(uint dev, uint num) {
    struct sblk *sp;
    struct inode *ip;

_loop:
    for (ip=inodes; ip<inodes+NINODE; ip++) {
        if(ip->idev == dev && ip->inum == num) {
            if (ip->flag & I_LOCK) {
                ip->flag |= I_WANTED;
                sleep((uint) ip);
                goto _loop;
            }
            if (ip->flag & I_MOUNT) {
                for (sp=mnt;sp<mnt+NMOUNT;sp++) {
                    if (ip == sp->imnt) {
                        dev = sp->dev;
                        num = ROOTINO;
                        goto _loop;
                    }
                }
            }
            ip->count++;
            ip->flag |= I_LOCK;
            return ip;
        }
    }
    for (ip=inodes;ip<inodes+NINODE;ip++) {
        if (ip->count == 0) {
            ip->idev = dev;
            ip->inum = num;
            ip->flag = I_LOCK;
            ip->count++;
            iload(ip);
            return ip;
        }
    }
    panic("inode table overflow");
    return NULL;
}

int iload(struct inode *ip) {
    struct buf *bp;
    struct sblk *sp;
    struct d_inode *inp;

    sp = getsblk(ip->idev);
    if (sp == NULL) {
        panic("cannot get super block");
    }
    bp = buf_read(ip->idev, INOBLOCK(sp, ip->inum));
    // error handle
    inp = (struct d_inode*) bp->data;
    memcpy(ip, &inp[(ip->inum-1)%IPB], sizeof(struct d_inode));
    buf_relse(bp);
    return 0;
}

void iupdate(struct inode *ip) {
    struct buf *bp;
    struct sblk *sbp;
    struct d_inode *inp;

    sbp = getsblk(ip->idev);
    if (sbp == NULL) {
        panic("iupdate: error super block");
    }
    bp = buf_read(ip->idev, INOBLOCK(sbp, ip->inum));
    inp = (struct d_inode*) bp->data;
    memcpy(&inp[(ip->inum-1)%IPB], ip, sizeof(struct d_inode));
    ip->flag &= ~I_DIRTY;
    buf_write(bp);
    buf_relse(bp);
}

int iput(struct inode *ip) {
    ushort dev;
    if (ip == NULL) {
        panic("iput: wrong inode");
    }
    ip->flag |= I_LOCK;
    if (ip->count > 0) {
        ip->count--;
    }
    if (ip->count == 0) {
        if (ip->nlinks == 0) {
            itrunc(ip);
            ifree(ip->idev, ip->inum);
        }
        dev = ip->zone[0];
        switch (ip->mode & S_IFMT) {
            case S_IFBLK:
                // TODO
                break;
            case S_IFCHR:
                // TODO
                break;
        }
        iupdate(ip);
        ip->flag = 0;
        ip->inum = 0;
    }
    iunlink(ip);
    return 0;
}

int iunlink(struct inode *ip) {
    cli();
    if (ip->flag & I_WANTED) {
        wakeup((uint) ip);
    }
    ip->flag &= ~(I_LOCK | I_WANTED);
    sti();
    return 0;
}

int itrunc(struct inode* ip) {
    int i;
    uint *tmp;
    struct buf *bp;

    for(i=0;i<7;i++) {
        if (ip->zone[i]) {
            bfree(ip->idev, ip->zone[i]);
            ip->zone[i] = 0;
        }
    }

    if (ip->zone[7]) {
        bp = buf_read(ip->idev, ip->zone[7]);
        tmp = (uint *) bp->data;
        for (i=0;i<NINDBLK;i++) {
            if (tmp[i]) {
                bfree(ip->idev, tmp[i]);
            }
        }
        bfree(ip->idev, ip->zone[7]);
    }
    ip->size = 0;
    iupdate(ip);
    return 0;
}

int lock_inode(struct inode *ip) {
    while (ip->flag & I_LOCK) {
        sleep((uint) ip);
    }
    ip->flag |= I_LOCK;
    return 0;
}

int unlink_inode(struct inode *ip) {
    cli();
    if (ip->flag & I_WANTED) {
        wakeup((uint) ip);
    }
    ip->flag &= ~(I_LOCK | I_WANTED);
    sti();
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
        bp = buf_read(dev, INOBLOCK(sbp, i));
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
    bp = buf_read(dev, INOBLOCK(sbp, ino));
    /**
    if ((bp->data[ino/8] & (1 << (ino%8))) == 0) {
        panic("inode is free");
    }
    */
    bp->data[ino/8] &= ~(1<<(ino%8));
    hd_sync(bp);
    buf_relse(bp);
    unlink_sb(sbp);
    return 0;
}

void dump_inode(struct inode* ip) {
    int i;
    printk("address=%x\n", ip);
    printk("mode: %x ", ip->mode);
    printk("uid: %x ", ip->uid);
    printk("size: %x ", ip->size);
    printk("gid: %x ", ip->gid);
    printk("count: %x ", ip->count);
    printk("idev: %x ", ip->idev);
    printk("inum: %x ", ip->inum);
    printk("flag: %x\n", ip->flag);
    for(i=0;i<9;i++) {
        printk("zone[%d]=%d ", i, ip->zone[i]);
    }
    printk("\n");
}
