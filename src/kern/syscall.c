#include <system.h>
#include <console.h>
#include <segment.h>
#include <unistd.h>

int errno = 0;

static int sys_routines[MAX_SYSCALL] = {
    [__NR_nosys] = &nosys,
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

int nosys(struct regs* rgs) {
    puts("No Syscall");
    return 0;
}
