#include <system.h>
#include <console.h>
#include <segment.h>
#include <sched.h>
#include <unistd.h>

int errno = 0;

static void* sys_routines[MAX_SYSCALL] = {
    [__NR_nosys] = &sys_nosys,
    [__NR_fork] = &sys_fork,
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
    current->error = eno;
    return -1;
}

int sys_nosys(struct regs* rgs) {
    puts("No Syscall");
    return 0;
}

int sys_fork(struct regs* rgs) {
    puts("sys_fork");
    return 0;
}

int sys_read(struct regs* rgs) {
    puts("sys_read");
    return 0;
}

int sys_write(struct regs* rgs) {
    puts("sys_write");
    return 0;
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

