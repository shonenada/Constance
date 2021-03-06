#include <const.h>
#include <system.h>
#include <fs.h>
#include <inode.h>
#include <stat.h>
#include <file.h>

struct sblk *rootsp = NULL;
ushort rootdev = DEVNO(0, 1);
struct sblk mnt[NMOUNT] = {{0,},};

struct sblk* do_mount(ushort dev, struct inode *ip) {
    struct sblk *sbp;

    for (sbp=&mnt[0]; sbp<&mnt[NMOUNT]; sbp++) {
        if (sbp->dev == dev) {
            sbp->flag |= S_LOCK;
            goto _found;
        }
    }

    for(sbp=&mnt[0];sbp<&mnt[NMOUNT];sbp++) {
        if (sbp->dev == NODEV) {
            sbp->dev = dev;
            sblk_load(sbp);
            goto _found;
        }
    }

    panic("do_mount: no free mount slot");
    return NULL;

_found:
    if ((ip != NULL) && (ip->mode & S_IFMT) != S_IFDIR) {
        putsp(sbp);
        return NULL;
    }
    if (dev == rootdev) {
        rootsp = sbp;
    }
    if (ip != NULL) {
        ip->count++;
    }
    sbp->imnt = ip;
    unlink_sb(sbp);
    return sbp;
}

int do_unmount(ushort dev) {
    struct sblk *sbp;
    struct inode *ip;

    sbp = getsblk(dev);

    for (ip=&inodes[0];ip<&inodes[NINODE];ip++) {
        if (ip->inum != 0 && ip->idev == dev) {
            // signal busy
            return -1;
        }
    }
    sblk_update(sbp);
    sbp->dev = 0;
    sbp->flag = 0;
    iput(sbp->imnt);
    return 0;
}

void dump_mnts() {
    int i;
    for(i=0;i<NMOUNT;i++) {
        dump_sblk(&mnt[i]);
    }
}
