#include <const.h>
#include <system.h>
#include <buf.h>
#include <fs.h>
#include <inode.h>
#include <file.h>

uint bmap(struct inode *ip, uint bn, uchar creat) {
    ushort dev;
    short *tmp, *tmp2;
    struct buf *bp, *bp2;

    if ((bn > MAX_FILESIZE/BLK_SIZE) || ((bn > (ip->size/BLK_SIZE) && (creat==0)))) {
        panic("blk is too big");
    }

    dev = ip->idev;

    if(bn < 7) {
        if (ip->zone[bn] == 0 && creat) {
            ip->zone[bn] = balloc(dev);
            ip->flag |= I_DIRTY;
            iupdate(ip);
        }
        return ip->zone[bn];
    }

    bn -= 7;

    if (bn < NINDBLK) {
        if (ip->zone[7] == 0) {
            if (creat == 0) {
                return 0;
            }
            ip->zone[7] = balloc(dev);
            bzero(dev, ip->zone[7]);
            ip->flag |= I_DIRTY;
            iupdate(ip);
        }
        bp = buf_read(dev, ip->zone[7]);
        tmp = (short*) bp->data;
        if (tmp[bn] == 0 && creat) {
            tmp[bn] = balloc(dev);
            buf_write(bp);
        }
        buf_relse(bp);
        return tmp[bn];
    }

    bn -= NINDBLK;

    if (ip->zone[8] == 0) {
        if (creat == 0) {
            return 0;
        }
        ip->zone[8] = balloc(dev);
        bzero(dev, ip->zone[8]);
        ip->flag |= I_DIRTY;
        iupdate(ip);
    }
    bp = buf_read(dev, ip->zone[8]);
    tmp = (short*)bp->data;
    if (tmp[bn/NINDBLK] == 0) {
        if (creat == 0) {
            buf_relse(bp);
            return 0;
        }
        tmp[bn/NINDBLK] = balloc(dev);
        bzero(dev, tmp[bn/NINDBLK]);
        buf_write(bp);
    }
    bp2 = buf_read(dev, tmp[bn/NINDBLK]);
    tmp2 = (short*) bp2->data;
    if ((tmp2[bn%NINDBLK] == 0) & creat) {
        tmp2[bn%NINDBLK] = balloc(dev);
        buf_write(bp2);
    }
    buf_relse(bp);
    buf_relse(bp2);
    return tmp2[bn%NINDBLK];
}

uint bmap2(struct inode *ip, uint bn, uchar creat) {
    ushort *tmp;
    struct buf *bp;
    if (bn < 7) {
        if (ip->zone[bn] == 0 && creat) {
            ip->zone[bn] = balloc(ip->idev);
            ip->flag |= I_DIRTY;
            iupdate(ip);
        }
        return ip->zone[bn];
    }

    bn -= 7;

    if (bn < NINDBLK) {
        if (ip->zone[7] == 0) {
            if (creat == 0) {
                return 0;
            }
            ip->zone[7] = balloc(ip->idev);
        }
        bp = buf_read(ip->idev, ip->zone[7]);
        tmp = (ushort*) (bp->data);
        if (tmp[bn] == 0 && creat) {
            tmp[bn] = balloc(ip->idev);
            buf_write(ip);
        }
        buf_relse(bp);
        return tmp[bn];
    }
    panic("bmap: out of range");
    return 0;
}

