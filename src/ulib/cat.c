#include <const.h>
#include <system.h>
#include <fs.h>
#include <file.h>

int do_cat(char *path) {
    int fd, n;
    char buf[32];

    fd = do_open(path, O_RDONLY, 0);
    if (fd < 0) {
        return -1;
    }

    while((n = do_read(fd, buf, 32)) > 0) {
        do_write(1, buf, n);
    }
    do_close(fd);
}
