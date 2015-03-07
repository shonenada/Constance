#include <system.h>
#include <segment.h>
#include <sched.h>

uchar mem0[1024] = {0,};
struct ktask *current = 0;
struct ktask *tasks[NR_TASKS] = {0,};

void sched_init() {
    struct ktask *t = current = tasks[0] = (struct ktask*) (uint) mem0;
    t->pid = 0;
    t->ppid = 0;
    t->state = TASK_STOPPED;
    t->ldt_sel = LDT_SEL(t->pid);

    //memset(&tss, 0, sizeof(tss));
    t->tss.ss0 = KERN_DS;
    t->tss.esp0 = (uint)t + 0x1000;

    seg_set(&(t->ldts[1]), 0, 420 * 1024, RING3, SEG_CODE_EXRD);
    seg_set(&(t->ldts[2]), 0, 420 * 1024, RING3, SEG_DATA_RW);

    tss_set(&gdt[TSS0], (uint) &(t->tss));
    ldt_set(&gdt[LDT0], (uint) &(t->ldts));
    ltr(TSS_SEL(t->pid));
    lldt(LDT_SEL(t->pid));

    //seg_set(&(t->ldts[1]), 0, NR_LDT * SIZE_DESC_ENTRY, RING3, SEG_CODE_EXRD);
    //seg_set(&(t->ldts[2]), 0, NR_LDT * SIZE_DESC_ENTRY, RING3, SEG_DATA_RW);


}

inline void ltr(uint n) {
    asm volatile ("ltr %%ax" :: "a"(n));
}

inline void lldt(uint n) {
    asm volatile ("lldt %%ax"::"a"(n));
}
