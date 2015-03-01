#ifndef __SCHED_H
#define __SCHED_H

#define NR_TASKS 64    // Max process numbers

#define TASK_RUNNING 0
#define TASK_INTERRUPTIBLE 1
#define TASK_UNINTERRUPTIBLE 2
#define TASK_ZOMBLE 3
#define TASK_STOPPED 4

// define data struct for task
struct tasks {
    int state;     // state of this task
    long counter;    // running time
    long priority;
    long signal;
    long pid;
    long pgrp;
    long sessoin;
    long utime;
    long stime;
    long start_time;
    ushort uid, euid, suid;
    ushort gid, egid, sgid;
    int exit_code;
};

#endif
