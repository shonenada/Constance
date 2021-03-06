#ifndef __SCHED_H
#define __SCHED_H

#include <const.h>
#include <segment.h>
#include <inode.h>
#include <stat.h>
#include <file.h>
#include <sig.h>
#include <vm.h>

#define TSS0 0x5
#define TSS_SEL(n) ((n<<4)+(TSS0<<3))    // offset of tss in GDT, size of descriptor = 8 bytes
#define LDT0 (TSS0+1)
#define LDT_SEL(n) ((n<<4)+(LDT0<<3))

#define PINOD (-90)
#define PBIO (-50)
#define PRITTY 10
#define PDEFAULT 20
#define PWAIT 40
#define PSLEP 90
#define PUSER 100

enum task_state {TASK_RUNNING, TASK_INTERRUPTIBLE, TASK_UNINTERRUPTIBLE, TASK_ZOMBLE, TASK_STOPPED};

struct context {
    uint eip;
    uint esp;
    uint ebx;
    uint ecx;
    uint edx;
    uint esi;
    uint edi;
    uint ebp;
    uint __sigmask;
};

struct tss_entry {
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
// TODO remove useless fileds
struct ktask {
    int state;     // state of this task
    long counter;    // running time
    uint priority;
    uint nice;
    uint cpu;
    uint ret;
    uint signal;
    uint sigmask;
    uint cursig;
    long pid;
    long ppid;
    long pgrp;
    long start_time;
    uint channel;
    ushort uid, euid, suid;
    ushort gid, egid, sgid;
    uint error;
    char *kstack;
    struct pde* pdir;
    struct sigact sigacts[NSIG];
    struct regs *rgs;
    struct file *files[NOFILE];
    uint fdflag[NOFILE];
    struct inode *wdir;
    struct inode *iroot;
    struct context context;
};

extern struct tss_entry tss;
extern struct ktask task0;
extern struct ktask *current;
extern struct ktask *tasks[NTASKS];

void sleep(uint channel, uint pri);
void wakeup(uint channel);
void cal_pri(struct ktask *task);
void sched_cpu();
void schedule();
int find_empty();
void switch_to(struct ktask* target);    // switch from current to target task
void do_swtch(struct context* old, struct context* new);
struct ktask* kspawn(void *func);
extern void _jump_u(uint eip, uint esp);
void jump_to_user_mode();

void dump_tasks();
void dump_task(struct ktask*);

#endif
