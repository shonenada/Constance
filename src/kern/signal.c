#include <const.h>
#include <segment.h>
#include <inode.h>
#include <stat.h>
#include <file.h>
#include <sig.h>
#include <sched.h>

int sendsig(uint pid, uint sig_no) {
    struct ktask * task = tasks[pid];
    if (pid == 0 || sig_no < 1 || sig_no > NSIG || task == NULL)
        return -1;

    if (task->sigacts[sig_no-1].handler == SIG_IGN && sig_no != SIGKILL)
        return -1;

    task->signal |= (1<<(sig_no-1));
    if (task->state == TASK_INTERRUPTIBLE) {
        task->channel = 0;
        task->state = TASK_RUNNING;
    }
    return 0;
}

int sendsig_g(uint grp, uint sig_no) {
    int i;
    struct ktask *tk;
    for (i=0;i<NTASKS;i++) {
        if ((tk = tasks[i]) && tk->pgrp == grp) {
            sendsig(tk->pid, sig_no);
        }
    }
    return 0;
}

/**
 * Check whether current task gets some signals.
 */
int issig() {
    uint n, sig;
    sig = current->signal;
    if (sig == 0) {
        return 0;
    }

    for(n=0;n<32;n++) {
        if ((sig & (1 << (n-1))) && current->pid != 0) {
            if ((current->sigacts[n-1].handler == SIG_IGN) && n != SIGKILL) {
                current->signal &= ~(1 << (n-1));
                return 0;
            }
            if (current->sigmask & (1 << (n-1)) && n != SIGKILL) {
                return 0;
            }
            current->cursig = n;
            return n;
        }
    }
    return 0;
}

/**
 * Perform action for specific signal. 
 */
int do_sig() {
    uint sig;
    struct stackframe *sf;
    struct sigact *sa;

    void *ufunc;

    sig = current->cursig;

    if (sig == 0 || sig > NSIG) {
        return -1;
    }

    current->signal &= ~(1 << (sig-1));
    sa = &(current->sigacts[sig-1]);

    current->cursig = 0;

    if (sa->handler != SIG_DFL) {
        sf = &(current->regs);
        //
        if ((sa->flags & SIG_NOMASK) == 0) {
            current->sigmask |= sa->mask;
        }
    }

    switch (sig) {
    }

    return 0;
}
