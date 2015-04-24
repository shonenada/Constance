#ifndef __SIG_H
#define __SIG_H

#define SIGHUP 1    // Hang up, Abort
#define SIGINT 2    // Interrupt, Abort
#define SIGQUIT 3    // Quit, Dump
#define SIGILL 4    // Illegal Instruction, Dump
#define SIGTRAP 5    // Bearkpoint/Trace Trap
#define SIGABRT 6    // Abort, Dump
#define SIGIOT 7    // IO Trap, Dump
#define SIGUNUSED 8    // Unused
#define SIGFPE 9    // Floating Point Exception, Dump
#define SIGKILL 10    // Kill process, Abort
#define SIGUSR1 11    // User defined signal 1, Abort
#define SIGSEGV 12    // Segmentation Violation, Dump
#define SIGUSR2 13    // User defined signal 2, Abort
#define SIGPIPE 14     // Pipe, Abort
#define SIGALRM 15    // Alarm, Abort
#define SIGTERM 16    // Terminate, Abort
#define SIGSTKFLT 17    // Stack fault on coprocessor, Abort
#define SIGHLD 18    // Child, Ignore 
#define SIGCONT 19    // Continue, Continue
#define SIGSTOP 20    // Stop, Stop
#define SIGTSTP 21    // Terminal Stop, Stop
#define SIGTTIN 22    // TTY Input on Background, Stop
#define SIGTTOU 23    // TTY Output on Background, Stop

#define NSIG 32

#define SIG_DFL ((void (*)(int))0)
#define SIG_IGN ((void (*)(int))1)

#define SIG_NOMASK 0x40000000
#define SIG_ONESHOT 0x80000000

#define SA_NOMASK 0x400000000

int sendsig(uint pid, uint sig_no);
int sendsig_g(uint pgrd, uint sig_no);    // send to group

struct sigact {
    void (*handler)(int);
    uint mask;
    int flags;
    void (*restorer)();
};

#endif
