#include <const.h>
#include <segment.h>
#include <inode.h>
#include <stat.h>
#include <file.h>
#include <sig.h>
#include <sched.h>

void _sigret() {
}

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
    uint sig, usr, esp;
    struct regs *rf;
    struct sigact *sa;
    struct context context, cp;

    void *ufunc;

    sig = current->cursig;

    if (sig == 0 || sig > NSIG) {
        return -1;
    }

    current->signal &= ~(1 << (sig-1));
    sa = &(current->sigacts[sig-1]);

    current->cursig = 0;

    if (sa->handler != SIG_DFL) {
        rf = &(current->rgs);
        store_regs(&context, rf, current->sigmask);

        if ((sa->flags & SIG_NOMASK) == 0) {
            current->sigmask |= sa->mask;
        }

        esp = rf->esp;
        usr = upush(&esp, (char*)&_sigret, 16);
    }

    switch (sig) {
    }

    return 0;
}

void store_regs(struct context* context, struct regs* rgs, uint mask) {
    context->eip = rgs->eip;
    context->esp = rgs->esp;
    context->ebx = rgs->ebx;
    context->ecx = rgs->ecx;
    context->edx = rgs->edx;
    context->esi = rgs->esi;
    context->edi = rgs->edi;
    context->ebp = rgs->ebp;
    context->__sigmask = mask;

}
