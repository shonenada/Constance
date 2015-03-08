#include <const.h>
#include <fs.h>

struct sblk mnt[NMOUNT] = { {0, }, };

struct m_sblk *getsblk(ushort dev) {
    struct m_sblk *sbp;
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

// alloc block
int balloc (ushort dev) {
}

// free block 
int bfree (ushort dev) {
}

// alloc zero block
int bzero (ushort dev) {
}

// alloc inode
int ialloc (ushort dev) {
}

// free inode
int ifree (ushort dev) {
}
