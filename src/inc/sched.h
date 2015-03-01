#ifndef __SCHED_H
#define __SCHED_H

#define NR_TASKS 64    // Max process numbers

#define TASK_RUNNING 0
#define TASK_INTERRUPTIBLE 1
#define TASK_UNINTERRUPTIBLE 2
#define TASK_ZOMBLE 3
#define TASK_STOPPED 4

struct tts_entry {
    uint link;    // previous task link
    uint esp0;
    ushort ss0, __0;
    uint esp1;
    ushort ss1, __1;
    uint esp2;
    ushort ss2, __2;
    uint cr3;
    uint eip;
    uint eflags;
    uint eax, ecx, edx, ebx, esp, ebp, esi, edi;
    ushort es, __3;
    ushort cs, __4;
    ushort ss, __5;
    ushort ds, __6;
    ushort fs, __7;
    ushort gs, __8;
    ushort ldt, __9;
    ushort trap, io_base;
} __attribute__ ((packed));

// define data struct for task
struct ktask {
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

extern struct ktask *current;
extern struct ktask *tasks[NR_TASKS];

#endif
