#include <const.h>
#include <system.h>
#include <file.h>
#include <sched.h>
#include <buf.h>
#include <inode.h>
#include <fs.h>

int do_link(char *path1, char *path2) {
    struct inode *tip, *dip;
    int r;
    char *name;

    tip = namei(path1, 0);
    if (tip == NULL) {
        iput(tip);
        // signal
        return -1;
    }

    tip->nlinks++;
    iupdate(tip);
    unlink_inode(tip);

    dip = namei_parent(path2, &name);
    if (strlen(name) == 0 || strlen(name) >= NAMELEN) {
        // signal ENOENT
        goto _bad_name;
    }

    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
        // signal EPERM
        goto _bad_name;
    }
    r = find_entry(dip, name, strlen(name));
    if (r != 0) {
        // signal EEXIST
        goto _bad_name;
    }

    lock_inode(tip);
    r = link_entry(dip, name, strlen(name), tip->inum);
    if (r == 0) {
        panic("bad link");
    }

    iupdate(tip);
    iput(tip);
    iput(dip);
    return 0;

_bad_name:
    lock_inode(tip);
    tip->nlinks--;
    iupdate(tip);
    iput(tip);
    iput(dip);
    return -1;
}

int do_unlink(char *path) {
    struct inode *ip, *dip;
    int ino;
    char *name;

    dip = namei_parent(path, &name);

    if (strlen(name) == 0 || strlen(name) >= NAMELEN) {
        goto _bad_name;
    }
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) {
        goto _bad_name;
    }

    ino = unlink_entry(dip, name, strlen(name));
    if (ino <= 0) {
        goto _bad_name;
    }

    ip = iget(dip->idev, ino);
    if ((ip->mode & S_IFMT) == S_IFDIR) {
        link_entry(dip, name, strlen(name), ip->inum);
        iput(ip);
        iput(dip);
        // signal
        return -1;
    }
    ip->nlinks--;
    iput(ip);
    iput(dip);
    return 0;

_bad_name:
    iput(dip);
    // signal
    return -1;
}

