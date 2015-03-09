#include <const.h>
#include <system.h>
#include <sched.h>
#include <sig.h>
#include <file.h>

/**
 * Terminate current task
 */
int do_exit(int ret) {
    uint nr, fd;
    struct file* fp;
    struct ktask* tp;

    current->ret = ret;

    // clean signal handlers
    for (nr=0;nr<NSIG;nr++) {
        current->sigacts[nr].handler = SIG_DFL;
        current->sigacts[nr].flags = 0;
    }

    for(fd=0;fd<NOFILE;fd++) {
        fp = current->files[fd];
        if (fp != NULL) {
            do_close(fd);
        }
        current->files[fd] = NULL;
    }

    iput(current->iroot);
    iput(current->wdir);

    sendsig(current->ppid, SIGHLD);

    // kfree()

    current->channel = 0;
    current->state = TASK_ZOMBLE;
    for(nr=1; nr<NTASKS; nr++) {
        if ((tp=tasks[nr]) && tp != current && (tp->ppid == current->pid)) {
            tp->ppid = 1;
        }
    }
    tp = tasks[current->ppid];

    wakeup((uint) tp);
    wakeup((uint) tasks[1]);
    return 0;
}
