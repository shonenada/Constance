#include <const.h>
#include <system.h>
#include <sched.h>
#include <blk.h>
#include <buf.h>
#include <stat.h>
#include <inode.h>
#include <fs.h>
#include <file.h>

struct file files[NFILE];

int do_read(int fd, char* buf, int cnt) {
    struct file *fp;
    struct inode *ip;
    uint dev;
    int r;

    fp = current->files[fd];
    if (fd < 0 || fd > NOFILE || fp == NULL) {
        // signal enfile
        return -1;
    }

    if (fp->flags & O_WRONLY) {
        // signal
        return -1;
    }

    ip = fp->ino;
    switch(ip->mode & S_IFMT) {
        case S_IFBLK:
            break;
        case S_IFCHR:
            dev = ip->zone[0];
            //r = ()
            break;
        case S_IFDIR:
        case S_IFREG:
        default:
            lock_inode(fp->ino);
            r = readi(ip, buf, fp->offset, cnt);
    }

    if (r < 0) {
        iunlink(ip);
        return -1;
    }
    fp->offset += cnt;
    iunlink(ip);
    return r;
}

int do_write(int fd, char *buf, int cnt) {
    struct file *fp;
    struct inode *ip;
    int r, off;
    uint dev;
    fp = current->files[fd];
    if (fd < 0 || fd > NOFILE || fp == NULL) {
        // signal ENFILE;
        return -1;
    }
    if (fp->flags & O_RDONLY) {
        // signal EBADF
        return -1;
    }

    if (fp->flags & O_APPEND) {
        off = fp->ino->size;
    } else {
        off = fp->offset;
    }

    ip = fp->ino;
    switch(ip->mode & S_IFMT) {
        case S_IFBLK:
            // TODO
            break;
        case S_IFCHR:
            dev = ip->zone[0];
            // r = ()
            break;

        case S_IFDIR:
        case S_IFREG:
        default:
            lock_inode(ip);
            r = writei(ip, buf, off, cnt);
    }
    if (r < 0) {
        unlink_inode(ip);
        return -1;
    }
    fp->offset = off + cnt;
    iunlink(ip);
    return r;
}

int do_lseek(uint fd, int off, int whence) {
    struct file *fp;

    fp = current->files[fd];
    if ((fd >= NOFILE) || (fp == NULL) || (fp->ino == NULL)) {
        // signal EBADF
        return -1;
    }
    if (fp->ino->mode & S_IFIFO) {
        // signal ESPIPE
        return -1;
    }

    if (whence == SEEK_SET) {
        if (off < 0) {
            goto _einval;
        }
        fp->offset = off;
    }
    if (whence == SEEK_CUR) {
        if (fp->offset + off < fp->offset) {
            goto _einval;
        }
        fp->offset += off;
    }
    if (whence == SEEK_END) {
        if (fp->ino->size + off < fp->ino->size) {
            goto _einval;
        }
        fp->offset = fp->ino->size + off;
    }
    return fp->offset;

_einval:
    // EINVAL
    return -1;
}

int readi(struct inode *ip, char *buf, uint off, uint cnt) {
    struct buf *bp;
    uint tot=0, m=0, bn=0;
    if ((off > ip->size) || (off + cnt < off)) {
        //current->error = E2BIG;
        return -1;
    }
    if (off + cnt > ip->size) {
        cnt = ip->size - off;
    }

    for(tot=0;tot<cnt;tot+=m, buf+=m) {
        m = min(cnt - tot, BLK_SIZE - off % BLK_SIZE);
        bn = bmap(ip, off/BLK_SIZE);
        if (bn == 0) {
            memset(bp->data + off % BLK_SIZE, 0, m);
        } else {
            bp = buf_read(ip->idev, bn);
            memcpy(buf, bp->data + off % BLK_SIZE, m);
            buf_relse(bp);
        }
    }
    return cnt;
}

int writei(struct inode* ip, char *buf, uint off, uint cnt) {
    struct buf *bp;
    uint tot=0, m=0, bn=0;
    if (off + cnt < off) {
        return -1;
    }
    if (off + cnt > MAX_FILESIZE) {
        cnt = MAX_FILESIZE - off;
    }
    for (tot=0;tot<cnt; tot+=m, off+=m, buf+=m) {
        m = min(cnt - tot, BLK_SIZE - off % BLK_SIZE);
        bn = bmap(ip, off/BLK_SIZE);
        if (bn == 0) {
            panic("writei: bad block");
        } else {
            bp = buf_read(ip->idev, bn);
            memcpy(bp->data + off % BLK_SIZE, buf, m);
            hd_sync(bp);
        }
    }
    if (cnt > 0 && off > ip->size) {
        ip->size = off;
        iupdate(ip);
    }
    return cnt;
}
