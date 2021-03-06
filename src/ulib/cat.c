#include <const.h>
#include <system.h>
#include <fs.h>
#include <file.h>
#include <sched.h>
#include <inode.h>
#include <stat.h>

int do_cat(char *path) {
    int fd, n;
    char buf[32];
    struct file *fp;
    struct stat statbuf;

    fd = do_open(path, O_RDONLY, 0);

    if (fd < 0) {
        printk("file %s not found.\n", path);
        do_close(fd);
        return -1;
    }

    fp = current->files[fd];
    do_stat(fp->ino, &statbuf);

    if ((statbuf.mode & S_IFMT) == S_IFDIR) {
        printk("%s is a director.\n", path);
        do_close(fd);
        return -1;
    }

    while((n = do_read(fd, buf, 32)) > 0) {
        do_write(1, buf, n);
    }

    do_close(fd);
    return 0;
}
