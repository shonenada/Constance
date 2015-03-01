#include <system.h>
#include <syscall.h>

#define MAX_SYSCALL 10

static void* sys_routines[MAX_SYSCALL];

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

    result = handler(rgs);
    if (result < 0) {
    } else {
        rgs->eax = result;
    }
    return 0;
}
