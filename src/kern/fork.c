#include <system.h>
#include <const.h>
#include <sched.h>
#include <mm.h>
#include <page.h>

struct ktask task0;
struct tss_entry tss;
struct ktask *current = NULL;
struct ktask *tasks[NTASKS] = {NULL,};

extern void _int_common_ret();

struct ktask* kspawn(void (*func)) {
    int i;
    uint pid;
    struct ktask *task;
    struct file *fp;

    pid = find_empty();
    if (pid == -1) {
        panic("no free pid");
        return NULL;
    }

    task = (struct ktask*) kmalloc(PAGE_SIZE);

    if (task == NULL) {
        panic("no page");
        return NULL;
    }

    tasks[pid] = task;
    task->state = TASK_INTERRUPTIBLE;

    task->cpu = current->cpu;
    task->nice = current->nice;
    task->priority = PUSER;

    task->pid = pid;
    task->ppid = current->pid;
    task->pgrp = current->pgrp;
    task->uid = current->uid;
    task->euid = current->euid;
    task->suid = current->suid;
    task->gid = current->gid;
    task->egid = current->egid;
    task->sgid = current->sgid;

    task->signal = current->signal;
    task->sigmask = current->sigmask;
    for (i=0;i<NSIG;i++) {
        task->sigacts[i] = current->sigacts[i];
    }

    for(i=0;i<NOFILE;i++) {
        fp = current->files[i];
        if (fp != NULL) {
            fp->count++;
            fp->ino->count++;
        }
        task->files[i] = fp;
        task->fdflag[i] = current->fdflag[i];
    }

    task->pdir = (struct pde *)kmalloc(PAGE_SIZE);
    pgd_init(task->pdir);
    pgd_copy(task->pdir, current->pdir);

    task->context = current->context;
    task->context.eip = (uint)func;

    task->context.esp = (uint)task+PAGE_SIZE;

    task->state = TASK_RUNNING;
    return task;
}

int do_fork(struct regs *rgs) {
    struct ktask *task;
    struct regs *_rgs;
    
    task = kspawn(&_int_common_ret);
    _rgs = (struct regs *)((uint)task+PAGE_SIZE) - 1;
    *_rgs = *rgs;
    _rgs->eax = 0;
    //task->context.esp = (uint)_rgs;
    task->rgs = _rgs;
    return task->pid;
}

void task0_init() {
    int fd;
    struct ktask *task = current = tasks[0] = &task0;

    task->state = TASK_RUNNING;
    task->cpu = 0;
    task->priority = 0;
    task->nice = PDEFAULT;

    task->pid = 0;
    task->ppid = 0;
    task->pgrp = 0;
    task->uid = 0;
    task->gid = 0;
    task->euid = 0;
    task->egid = 0;
    task->counter = 0;

    task->wdir = NULL;
    task->iroot = NULL;

    task->pdir = &pgd0;

    tss.ss0 = KERN_DS;
    tss.esp0 = KSTACK0;

    for(fd=0;fd<NOFILE;fd++) {
        task->files[fd] = NULL;
    }
}

void dump_context (struct context * c) {
    printk("edi(%x)=%x esi(%x)=%x ebx(%x)=%x ebp(%x)=%x eip(%x)=%x\n",
           &(c->edi), c->edi, &(c->esi), c->esi, &(c->ebx), c->ebx, &(c->ebp), c->ebp, &(c->eip), c->eip);
}
