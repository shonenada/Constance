#include <system.h>
#include <asm.h>
#include <segment.h>
#include <sched.h>
#include <console.h>

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
    for (t=&tasks[0];t<&tasks[NTASKS];t++) {
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
    struct ktask* old;

    if (current == target)
        return;
    tss.esp0 = (uint)target + 0x1000;
    old = current;
    current = target;
    lpgd(target->pdir);
    // restore stack
    do_swtch(&(old->context), &(target->context));
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

void dump_procs() {
    int i;
    struct ktask *task;
    for (i=0;i<NTASKS;i++) {
        task = tasks[i];
        if (task) {
            printk("pid: %d\n", task->pid);
        }
    }
}
