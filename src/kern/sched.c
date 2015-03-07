#include <system.h>
#include <asm.h>
#include <segment.h>
#include <sched.h>

uchar mem0[1024] = {0,};
struct tss_entry tss;
struct ktask *current = NULL;
struct ktask *tasks[NR_TASKS] = {NULL,};

void sched_init() {
    struct ktask *t = current = tasks[0] = (struct ktask*) (uint) mem0;
    t->pid = 0;
    t->ppid = 0;
    t->state = TASK_STOPPED;
    t->ldt_sel = LDT_SEL(t->pid);

    tss.ss0 = KERN_DS;
    tss.esp0 = (uint)t + 0x1000;

    seg_set(&(t->ldts[1]), 0, 420 * 1024, RING3, SEG_CODE_EXRD);
    seg_set(&(t->ldts[2]), 0, 420 * 1024, RING3, SEG_DATA_RW);

    tss_set(&gdt[TSS0], (uint) &tss);
    ldt_set(&gdt[LDT0], (uint) &(t->ldts));
    ltr(TSS_SEL(t->pid));
    lldt(LDT_SEL(t->pid));
}

void sleep() {
    cli();
    if (current == tasks[0]) {    // tasks[0] is init process
        panic("tasks[0] is trying to sleep");
    }
    current->state = TASK_UNINTERRUPTIBLE;
    sti();
    schedule();
}

void sleep_on(struct ktask* task) {
    cli();
    if (!task) {
        return;
    }
    task->state = TASK_UNINTERRUPTIBLE;
    sti();
    schedule();
}

void wakeup() {
    wakeup(current);
}

void wakeup_on(struct ktask* task) {
    cli();
    if (!task) {
        return;
    }
    task->state = TASK_RUNNING;
    sti();
}

void schedule() {
    int idx;
    int _pid = current->pid + 1;    // start from next task
    for(;_pid<NR_TASKS+_pid;_pid++) {
        idx = _pid % NR_TASKS;
        if (!tasks[idx])
            continue;
        if (tasks[idx]->state == TASK_RUNNING) {
            switch_to(tasks[idx]);
        }
    }
}

// refer to linux/sched.h (swtich_to)
void switch_to(struct ktask* target) {
    if (current == target)
        return;
    tss.esp0 = (uint)target + 0x1000;
    lldt(LDT_SEL(target->pid));    // modify esp according to LDT of task
    current = target;
    // restort stack
    asm volatile (
        "mov %%eax, %%esp;"\
        "pop %%gs;"\
        "pop %%fs;"\
        "pop %%es;"\
        "pop %%ds;"\
        "popa;"\
        "add %%esp, 8;"::"a"(&target->regs));
}

int find_empty() {
    int _pid;
    for(_pid=0;_pid<NR_TASKS;_pid++) {
        if (tasks[_pid] == NULL) {
            return _pid;
        }
    }
    return -1;
}
