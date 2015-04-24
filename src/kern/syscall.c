#include <system.h>
#include <console.h>
#include <segment.h>
#include <sched.h>
#include <unistd.h>
#include <inode.h>
#include <namei.h>

int errno = 0;

static void* sys_routines[MAX_SYSCALL] = {
    [__NR_nosys] = &sys_nosys,
    [__NR_fork] = &sys_fork,
    [__NR_read] = &sys_read,
    [__NR_write] = &sys_write,
    [__NR_open] = &sys_open,
    [__NR_close] = &sys_close,
    [__NR_waitpid] = &sys_waitpid,
    [__NR_creat] = &sys_creat,
    [__NR_link] = &sys_link,
    [__NR_unlink] = &sys_unlink,
    [__NR_chdir] = &sys_chdir,
    [__NR__exit] = &sys__exit,
};

// System call
// eax: which system call
// ebx, ecx, edx are params
int do_syscall(struct regs *rgs) {
    int result;
    int (*handler)(struct regs* r);
    if (rgs->eax > MAX_SYSCALL) {
        // Wrong EAX
    }
    handler = sys_routines[rgs->eax];

    if (handler) {
        result = handler(rgs);
        if (result < 0) {

        } else {
            rgs->eax = result;
        }
    }
    return 0;
}

int syserr(int eno) {
    if (DEBUG) {
        printk("error: %d\n", eno);
    }
    current->error = eno;
    return -1;
}

int sys_nosys(struct regs* rgs) {
    puts("No Syscall");
    return 0;
}

int sys_fork(struct regs* rgs) {
    return do_fork(rgs);
}

// int read(fd, buf, size);
int sys_read(struct regs* rgs) {
    int fd = rgs->ebx;
    char *buf = (char*)rgs->ecx;
    int size = rgs->edx;

    return do_read(fd, buf, size);
}

// int write(fd, buf, size)
int sys_write(struct regs* rgs) {
    int fd = rgs->ebx;
    int size = rgs->edx;
    char *buf = (char*) rgs->ecx;

    return do_write(fd, buf, size);
}

// int open(path, flag, mode);
int sys_open(struct regs* rgs) {
    char* path = (char*)(rgs->ebx);
    int flag = rgs->flag;
    int mode = rgs->mode;

    return do_open(path, flag, mode);
}

// int close(int fd);
int sys_close(struct regs* rgs) {
    int fd = rgs->ebx;
    return do_close(fd);
}

int sys_waitpid(struct regs* rgs) {
    // TODO
    puts("sys_waitpid");
    return 0;
}

// int creat(char *path, int mode);
int sys_creat(struct regs* rgs) {
    char* path = (char*) rgs->ebx;
    int mode = rgs->ecx;
    return do_creat(path, mode);
}

// int link(char* path1, char* path2);
int sys_link(struct regs* rgs) {
    char *path1 = (char*) rgs->ebx;
    char* path2 = (char*) rgs->ecx;
    return do_link(path1, path2);
}

// int unlink(path);
int sys_unlink(struct regs* rgs) {
    char* path = (char*)rgs->ebx;
    return do_unlink(path);
}

int sys_chdir(struct regs* rgs) {
    // TODO
    return 0;
}

int sys_access(struct regs* rgs) {
    char* path = (char*)rgs->ebx;
    int mode = rgs->ecx;
    struct inode *ip;
    ip = namei(path, 0);
    if (ip == NULL) {
        return -1;
    }
    return do_access(ip, mode);
}

int sys__exit(struct regs* rgs) {
    // TODO
    return do_exit((int) rgs->ebx);
}
