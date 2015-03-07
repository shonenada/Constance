#include <const.h>
#include <sig.h>
#include <sched.h>

int sendsig(uint pid, uint sig_no) {
    struct ktask * task = tasks[pid];
    if (pid == 0 || sig_no < 1 || sig_no > NR_SIG || task == NULL)
        return -1;

    task->signal |= (1<<(sig_no-1));
    return 0;
}
