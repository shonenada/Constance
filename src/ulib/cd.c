#include <const.h>
#include <system.h>
#include <fs.h>
#include <file.h>
#include <sched.h>
#include <inode.h>
#include <errno.h>
#include <stat.h>

int do_chdir(char *path) {
    struct inode* ip, *old_ip;
    struct stat statbuf;

    ip = namei(path, 0);
    if (ip == NULL) {
        printk("no such file or direcotry: %s\n", path);
        return -1;
    }

    do_stat(ip, &statbuf);
    if ((statbuf.mode & S_IFMT) != S_IFDIR) {
        printk("%s is a file\n", path);
        iput(ip);
        return -1;
    }

    old_ip = current->wdir;
    iput(old_ip);

    current->wdir = ip;
    ip->count++;
    unlink_inode(ip);

    return 0;
}
