#include <const.h>
#include <system.h>
#include <buf.h>
#include <fs.h>
#include <inode.h>

uint bmap(struct inode *ip, uint bn) {
    uint *tmp;
    struct buf *bp;
    if (bn < 7) {
        if (ip->zone[bn] == 0)
            ip->zone[bn] = balloc(ip->idev);
        return ip->zone[bn];
    }

    bn -= 7;

    if (bn < NINDBLK) {
        if (ip->zone[7] == 0) {
            ip->zone[7] = balloc(ip->idev);
        }
        bp = read_buffer(ip->idev, ip->zone[7]);
        tmp = (uint*) (bp->data);
        if (tmp[bn] == 0) {
            tmp[bn] = balloc(ip->idev);
            iupdate(ip);
        }
        return ip->zone[7];
    }
    panic("bmap: out of range");
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
        bp = read_buffer(ip->idev, ip->zone[7]);
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
