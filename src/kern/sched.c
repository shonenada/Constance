#include <system.h>
#include <asm.h>
#include <segment.h>
#include <sched.h>
#include <console.h>

uchar mem0[1024] = {0,};
struct tss_entry tss;
struct ktask *current = NULL;
struct ktask *tasks[NTASKS] = {NULL,};

void sched_init() {
    struct ktask *t = current = tasks[0] = (struct ktask*) (uint) mem0;
    t->pid = 0;
    t->ppid = 0;
    t->state = TASK_RUNNING;
    t->ldt_sel = LDT_SEL(t->pid);

    tss.ss0 = KERN_DS;
    tss.esp0 = (uint)t + 0x1000;

    seg_set(&(t->ldts[1]), 0, 640 * 1024, RING3, SEG_CODE_EXRD);
    seg_set(&(t->ldts[2]), 0, 640 * 1024, RING3, SEG_DATA_RW);

    tss_set(&gdt[TSS0], (uint) &tss);
    ldt_set(&gdt[LDT0], (uint) &(t->ldts));
    ltr(TSS_SEL(t->pid));
    lldt(LDT_SEL(t->pid));
}

void sleep(uint channel) {
    cli();
    if (current == tasks[0]) {    // tasks[0] is init process
        panic("tasks[0] is trying to sleep");
    }
    current->channel = channel;
    current->state = TASK_UNINTERRUPTIBLE;
    sti();
    schedule();
}

void sleep_on(struct ktask* task, uint channel) {
    cli();
    if (!task) {
        return;
    }
    task->channel = channel;
    task->state = TASK_UNINTERRUPTIBLE;
    sti(); schedule();
}

void wakeup(uint channel) {
    struct ktask *t;
    for (t=tasks;t<tasks+NTASKS;t++) {
        if (t->state == TASK_UNINTERRUPTIBLE && t->channel == channel) {
            t->channel = 0;
            t->state = TASK_RUNNING;
        }
    }
}

void wakeup_on(struct ktask* task, uint channel) {
    cli();
    if (!task) {
        return;
    }
    if (task->channel == channel) {
        task->state = TASK_RUNNING;
    }
    sti();
}

void schedule() {
    int idx;
    int _pid;    // start from next task
    int next_pid = current->pid + 1; 
    struct ktask *task;
    for(_pid=next_pid;_pid<next_pid+NTASKS;_pid++) {
        idx = _pid % NTASKS;
        task = tasks[idx];
        if (!task || task == NULL || task == 0)
            continue;
        if (tasks[idx] != NULL && tasks[idx]->state == TASK_RUNNING)
            switch_to(tasks[idx]);
    }
}

// refer to linux/sched.h (swtich_to)
void switch_to(struct ktask* target) {
    if (current == target)
        return;
    tss.esp0 = (uint)target + 0x1000;
    lldt(LDT_SEL(target->pid));    // modify esp according to LDT of task
    current = target;
    // restore stack
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
    for(_pid=0;_pid<NTASKS;_pid++) {
        if (tasks[_pid] == NULL) {
            return _pid;
        }
    }
    return -1;
}
