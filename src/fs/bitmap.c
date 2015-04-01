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
        bp = buf_read(ip->idev, ip->zone[7]);
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

