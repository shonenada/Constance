#include <const.h>
#include <system.h>
#include <time.h>
#include <sched.h>
#include <buf.h>
#include <blk.h>
#include <stat.h>
#include <fs.h>
#include <inode.h>
#include <file.h>

struct file files[NFILE];

int do_fcntl(int fd, uint cmd, uint arg) {
    struct file *fp;
    fp = current->files[fd];
    if (fd >= NOFILE || fd < 0 || fp == NULL) {
        panic("No such file");
        return -1;
    }

    switch (cmd) {
        case F_DUPFD:    // duplicate fd
            return do_dup(fd);
        case F_GETFL:
            return fp->flags;
        case F_SETFL:
            fp->flags = arg;
            return 0;
        case F_GETFD:
            return current->fdflag[fd];
        case F_SETFD:
            current->fdflag[fd] = arg;
            return 0;
        default:
            return -1;
    }
}

// get a struct stat of inode
int do_stat(struct inode *ip, struct stat *sbuf) {
    struct stat tmp;
    tmp.dev = ip->idev;
    tmp.ino = ip->inum;
    tmp.mode = ip->mode;
    tmp.nlink = ip->nlinks;
    tmp.uid = ip->uid;
    tmp.gid = ip->gid;
    tmp.dev = ip->zone[0];
    tmp.mtime = ip->mtime;
    tmp.ctime = ip->ctime;
    tmp.atime = ip->atime;
    tmp.size = ip->size;
    *sbuf = tmp;
    return 0;
}

int do_access(struct inode *ip, uint mode) {
    struct sblk *sp;
    uint m;

    if (ip == NULL) {
        panic("do_access: inode is NULL");
        return -1;
    }
    if (mode & W_OK) {
        sp = getsblk(ip->idev);
        unlink_sb(sp);
        if (sp->flag & S_RDONLY) {
            panic("do_access: RDONLY");
            return -1;
        }
        if (ip->flag & I_TEXT) {
            return -1;
        }
    }
    // TODO: super user
    m = ip->mode & 0777;
    if (current->euid == ip->uid) {
        m >>= 6;
    } else if (current->egid == ip->gid) {
        m >>= 3;
    }
    if ((m & 07 & mode) == mode) {
        return 0;
    }
    // TODO Signal EACCESS
    return -1;
}

int do_creat(char *path, int mode) {
    return do_open(path ,O_CREAT | O_TRUNC, mode);
}

int do_mknod(char *path, int mode, ushort dev) {
    struct inode *ip;
    ip = namei(path, 1);
    if (ip == NULL) {
        iput(ip);
        return -1;
    }

    if (ip->nlinks != 0) {
        // TODO Signal EEXIST
        iput(ip);
        return -1;
    }
    if (mode == S_IFBLK || mode == S_IFCHR) {
        ip->zone[0] = dev;
    }
    ip->mode = mode;
    ip->nlinks = 1;
    ip->mtime = timestamp();
    ip->uid = current->euid;
    ip->gid = current->egid;
    iupdate(ip);
    iput(ip);
    return 0;
}
