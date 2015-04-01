#include <const.h>
#include <system.h>
#include <sched.h>
#include <inode.h>
#include <fs.h>
#include <stat.h>
#include <buf.h>
#include <blk.h>

uint find_entry(struct inode* dip, char *name, uint len) {
    struct buf *bp;
    struct dire *dep;
    int i, j, bn=0, ino=0;

    if ((dip->mode & S_IFMT) != S_IFDIR) {
        // TODO signal fault
        return 0;
    }
    len = min(len, NAMELEN);

    for(i=0;i<dip->size/BLK_SIZE+1;i++) {
        bn = bmap(dip, i);
        bp = buf_read((uint) dip->idev, bn);
        dep = (struct dire*) bp->data;
        for (j=0;i<BLK_SIZE/(sizeof(struct dire))+1;j++) {
            if (len == strlen(dep[j].name) && strncmp(name, dep[j].name, len) == 0) {
                ino = dep[j].inode;
                return ino;
            }
        }
    }
    return 0;
}

int unlink_entry(struct inode *dip, char *name, int len) {
    struct buf *bp;
    struct dire *dep;
    int i, j, bn=0, ino=0;
    if ((dip->mode & S_IFMT) != S_IFDIR) {
        // signal FAULT
        return 0;
    }

    len = min(len, NAMELEN);

    for(i=0;i<dip->size/BLK_SIZE+1;i++) {
        bn = bmap(dip, i);
        bp = buf_read(dip->idev, bn);
        dep = (struct dire*) bp->data;
        for(j=0;j<BLK_SIZE/(sizeof(struct dire))+1;j++) {
            if (len == strlen(dep[j].name) && strncmp(name, dep[j].name, len) == 0) {
                ino = dep[j].inode;
                dep[j].inode = 0;
                hd_sync(bp);
                return ino;
            }
        }
    }
    return 0;
}

uint link_entry(struct inode *dip, char *name, uint len, uint ino) {
    struct dire de;
    int r, off;

    if ((dip->mode & S_IFMT) != S_IFDIR) {
        // signal not dir
        return 0;
    }

    len = min(len, NAMELEN);

    for (off=0;off<dip->size;off+=sizeof(struct dire)) {
        r = readi(dip, (char*)&de, off, sizeof(struct dire));
        if (r != sizeof(struct dire)) {
            panic("link_entry: wrong dir inode");
        }

        if (de.inode == 0) {
            break;
        }
    }
    strncpy(de.name, name, len + 1);
    de.inode = ino;
    r = writei(dip, (char*) &de, off, sizeof(struct dire));
    if (r != sizeof(struct dire)) {
        panic ("bad inode");
    }
    return ino;
}

struct inode * _namei(char *path, uchar creat, uchar parent, char **name) {
    struct inode *wip=NULL, *cdp=NULL;
    ushort dev, ino, len;
    char* tmp;

    if (*path == '/') {
        wip = iget(rootdev, ROOTINO);
    } else {
        cdp = current->wdir;
        wip = iget(cdp->idev, cdp->inum);
    }

    while(*path != '\0') {
        if (*path == '/') {
            path++;
            continue;
        }
        if ((wip->inum == ROOTINO) && (strncmp(path, "..", 2) == 0)) {
            // do nothing
        }

        if ((wip->mode & S_IFMT) != S_IFDIR) {
            iput(wip);
            // signal
            return NULL;
        }
        tmp = strchr(path, '/');
        if (tmp == NULL) {
            if (parent) {
                *name = path;
                return wip;
            }
            len = strlen(path);
            dev = wip->idev;
        }
        else {
            len = tmp - path;
        }
        ino = find_entry(wip, path, len);
        if (ino <= 0) {
            if (creat == 0) {
                iput(wip);
                return NULL;
            }
            ino = ialloc(wip->idev);
            wip->flag |= I_LOCK;
            link_entry(wip, path, len, ino);
        }
        iput(wip);
        wip = iget(wip->idev, ino);
        path += len;
    }
    return wip;
}

struct inode* namei(char *path, uchar creat) {
    return _namei(path, creat, 0, NULL);
}

struct inode* namei_parent(char *path, char **name) {
    return _namei(path, 0, 1, name);
}
