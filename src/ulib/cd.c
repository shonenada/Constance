#include <const.h>
#include <system.h>
#include <fs.h>
#include <file.h>
#include <sched.h>
#include <inode.h>
#include <errno.h>

int do_chdir(char *path) {
    struct inode* ip;
    ip = namei(path, 0);
    if (ip == NULL) {
        syserr(ENOTDIR);
        return -1;
    }
    current->wdir = ip;
    iput(ip);
    return 0;
}

