#include <const.h>
#include <system.h>
#include <sched.h>
#include <fs.h>
#include <inode.h>
#include <buf.h>
#include <blk.h>

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
    bp = read_buffer(ip->idev, IBLOCK(ip->inum));
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

    bp = read_buffer(ip->idev, IBLOCK(ip->inum));

    inp = (struct d_inode*) bp->data;
    memcpy(&inp[(ip->inum-1)%IPB], ip->zone, sizeof(struct d_inode));
    ip->flag &= ~I_DIRTY;
    hd_sync(bp);
}
