#include <system.h>
#include <console.h>
#include <segment.h>
#include <sched.h>
#include <unistd.h>

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
    if (DEBUG) {
        puts("debug: ---- do_fork(); -----");
    }
    return do_fork(rgs);
}

int sys_read(struct regs* rgs) {
    puts("sys_read");
    return 0;
}

int sys_write(struct regs* rgs) {
    int fd = rgs->ebx;
    int cnt = rgs->edx;
    char *buf = (char*) rgs->ecx;
    return do_write(fd, buf, cnt);
}

int sys_open(struct regs* rgs) {
    puts("sys_open");
    return 0;
}

int sys_close(struct regs* rgs) {
    puts("sys_close");
    return 0;
}

int sys_waitpid(struct regs* rgs) {
    puts("sys_waitpid");
    return 0;
}

int sys_creat(struct regs* rgs) {
    puts("sys_creat");
    return 0;
}

int sys_link(struct regs* rgs) {
    puts("sys_link");
    return 0;
}

int sys_unlink(struct regs* rgs) {
    puts("sys_unlink");
    return 0;
}

int sys_chdir(struct regs* rgs) {
    puts("sys_chidr");
    return 0;
}

int sys_access(struct regs* rgs) {
    puts("sys_access");
    return 0;
}

int sys__exit(struct regs* rgs) {
    return do_exit((int) rgs->ebx);
}
