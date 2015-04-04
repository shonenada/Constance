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
    struct inode *ip;
    struct sblk *sp;

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
    struct sblk *sp;
    struct d_inode *inp;
    struct buf *bp;

    sp = getsblk(ip->idev);
    if (sp == NULL) {
        panic("cannot get super block");
    }
    bp = buf_read(ip->idev, INOBLOCK(sp, ip->inum));
    // error handle
    inp = (struct d_inode*) bp->data;
    memcpy(ip, &inp[(ip->inum-1)%IPB], sizeof(struct d_inode));
    return 0;
}

void iupdate(struct inode *ip) {
    struct sblk *sbp;
    struct d_inode *inp;
    struct buf *bp;

    sbp = getsblk(ip->idev);
    if (sbp == NULL) {
        panic("iupdate: error super block");
    }

    bp = buf_read(ip->idev, IBLOCK(ip->inum));

    inp = (struct d_inode*) bp->data;
    memcpy(&inp[(ip->inum-1)%IPB], ip->zone, sizeof(struct d_inode));
    ip->flag &= ~I_DIRTY;
    hd_sync(bp);
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

void dump_inode(struct inode* ip) {
    printk("address=%x\n", ip);
    printk("mode: %x ", ip->mode);
    printk("uid: %x ", ip->uid);
    printk("size: %x ", ip->size);
    printk("gid: %x ", ip->gid);
    printk("count: %x ", ip->count);
    printk("idev: %x ", ip->idev);
    printk("inum: %x ", ip->inum);
    printk("flag: %x\n", ip->flag);
}
