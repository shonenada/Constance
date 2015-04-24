#include <const.h>
#include <system.h>
#include <errno.h>
#include <time.h>
#include <sched.h>
#include <inode.h>
#include <file.h>
#include <tty.h>

int do_open(char *path, uint flag, uint mode) {
    int fd;
    ushort dev;
    struct file *fp;
    struct inode *ip;

    if (flag & O_CREAT) {
        ip = namei(path, 1);
        if (ip->nlinks == 0) {
            ip->mode = mode;
            ip->uid = current->euid;
            ip->gid = current->egid;
            ip->mtime = timestamp();
            ip->nlinks = 1;
            iupdate(ip);
        }
    } else {
        ip = namei(path, 0);
        if (ip == NULL) {
#ifdef DEBUG
            printk("do_open(): ip is null");
#endif
            syserr(ENFILE);
            iput(ip);
            return -1;
        }

        dev = ip->zone[0];
        switch(ip->mode && S_IFMT) {
            case S_IFBLK:
                // TODO
                break;
            case S_IFCHR:
                tty_open(dev);
                break;
        }
    }

    if (((fd = ufalloc()) < 0) || (fp=falloc(fd)) == NULL) {
        iput(ip);
        return -1;
    }
    if (flag & O_TRUNC) {
        itrunc(ip);
    }
    unlink_inode(ip);
    fp->flags = flag;
    fp->ino = ip;
    current->fdflag[fd] = FD_CLOEXEC;
    return fd;
}

int do_close(int fd) {
    struct file *fp;
    if ((fd > NOFILE) || (fd < 0)) {
#ifdef DEBUG
        printk("do_close(): fd > NOFILE || fd < 0, fd=%d\n", fd);
#endif
        syserr(EBADF);
        return -1;
    }

    fp = current->files[fd];
    if (fp == NULL) {
#ifdef DEBUG
        printk("fp == NULL\n");
#endif
        syserr(EBADF);
        return -1;
    }
    current->files[fd] = NULL;
    iput(fp->ino);
    fp->count--;
    if (fp->count <= 0) {
        fp->count = 0;
        fp->flags = 0;
        fp->offset = 0;
    }
    return 0;
}

int do_dup(int fd) {
    struct file *fp;
    int newfd;

    fp = current->files[fd];
    if (fd >= NOFILE || fp == NULL) {
#ifdef DEBUG
        printk("do_dup(): fd >= NOFILE || fp == NULL, fd=%d\n", fd);
#endif
        syserr(EBADF);
        return -1;
    }
    if ((newfd = ufalloc()) < 0) {
        return -1;
    }
    fp->count++;
    fp->ino->count++;
    current->files[newfd] = fp;
    return newfd;
}

int do_dup2(int fd, int newfd) {
    struct file *fp;
    fp = current->files[fd];
    if (fd >= NOFILE || fp == NULL) {
#ifdef DEBUG
        printk("do_dup2(): fd >= NOFILE || fp == NULL, fd=%d\n", fd);
#endif
        syserr(EBADF);
        return -1;
    }
    do_close(newfd);
    fp->count++;
    fp->ino->count++;
    current->files[newfd] = fp;
    return newfd;
}

int ufalloc() {
    int i;
    for (i=0;i<NOFILE; i++) {
        if (current->files[i] == NULL) {
            return i;
        }
    }
    syserr(ENFILE);
    return -1;
}

struct file* falloc(int fd) {
    struct file *fp;
    for(fp=&files[0]; fp<&files[NFILE]; fp++) {
        if (fp->count == 0) {
            fp->count = 1;
            fp->offset = 0;
            fp->flags = 0;
            current->files[fd] = fp;
            return fp;
        }
    }
    syserr(EMFILE);
#ifdef DEBUG
    panic("falloc: no free file");
#endif
    return NULL;
}
