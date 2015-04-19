#include <const.h>
#include <system.h>
#include <fs.h>
#include <file.h>
#include <inode.h>
#include <stat.h>
#include <sched.h>

int do_ls(char *path) {
    int r, fd;
    char path_buf[1024];
    struct stat statbuf;
    struct dire dirbuf;
    struct file *fp;

    if (path == NULL) {
        strcpy(path_buf, ".");
    } else {
        strncpy(path_buf, path, 1024);
    }

    fd = do_open(path_buf, O_RDONLY, 0);

    if (fd < 0) {
        return -1;
    }

    fp = current->files[fd];
    do_stat(fp->ino, &statbuf);

    if (!(((statbuf.mode) & S_IFMT) == S_IFDIR)) {
        return -1;
    }

    while((r=do_read(fd, &dirbuf, sizeof(struct dire))) > 0) {
        printk("%s\t", dirbuf.name);
    }
    return 0;
}
