#include <system.h>
#include <const.h>
#include <asm.h>
#include <segment.h>
#include <sched.h>
#include <console.h>
#include <mm.h>

void sleep(uint channel, uint pri) {
    if (current == tasks[0]) {
        panic("tasks[0] is trying to sleep");
    }
    if (pri < 0) {
        cli();
        current->channel = channel;
        current->priority = pri;
        current->state = TASK_UNINTERRUPTIBLE;
        sti();
        schedule();
    } else {
        cli();
        current->channel = channel;
        current->priority = pri;
        current->state = TASK_INTERRUPTIBLE;
        sti();
        schedule();
    }
}

void wakeup(uint channel) {
    int _pid;
    struct ktask *t;
    for(_pid=0;_pid<NTASKS;_pid++) {
        t = tasks[_pid];
        if (t == NULL) {
            continue;
        }
        if (t->channel == channel) {
            t->channel = 0;
            t->state = TASK_RUNNING;
        }
    }
}

void cal_pri(struct ktask *task) {
    int pri;
    pri = 2 * task->nice + PUSER + task->cpu / 4;
    if (pri > 128) {
        pri = 128;
    } else if (pri <= -126) {
        pri = -126;
    }
    task->priority = pri;
}

// call per second
void sched_cpu() {
    uint i;
    struct ktask *task;
    for(i=0;i<NTASKS;i++) {
        if ((task=tasks[i])) {
            task->cpu /= 2;
            if (task->priority > PUSER) {
                cal_pri(task);
            }
        }
    }
}

void schedule() {
    int _pid;
    char _pri = 127;
    struct ktask *task=NULL, *next_task=NULL;

    for(_pid=0;_pid<NTASKS;_pid++) {
        task = tasks[_pid];
        if (!task || task == NULL || task == 0 || task == current)
            continue;
        if (task->state == TASK_RUNNING || task->pid == 0) {
            if (task->priority <= _pri) {
                _pri = task->priority;
                next_task = task;
            }
        }
    }
    if (next_task == NULL) {
        _pri = tasks[0]->priority;
        next_task = tasks[0];
    }
    if (current != next_task) {
        switch_to(next_task);
    }
}

// refer to linux/sched.h (swtich_to)
void switch_to(struct ktask* target) {
    struct ktask* old;
    if (current == target)
        return;
    tss.esp0 = (uint)target + PAGE_SIZE;
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

void dump_tasks() {
    int i;
    struct ktask *task;
    for (i=0;i<NTASKS;i++) {
        task = tasks[i];
        if (task) {
            printk("pid: %d, state=%d\n", task->pid, task->state);
        }
    }
}

void dump_task(struct ktask* task) {
    printk("task(%x): state=%x \n", task, task->state);
}
