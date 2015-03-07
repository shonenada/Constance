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
#define SIGSIGHLD 18    // Child, Ignore 
#define SIGCONT 19    // Continue, Continue
#define SIGSTOP 20    // Stop, Stop
#define SIGTSTP 21    // Terminal Stop, Stop
#define SIGTTIN 22    // TTY Input on Background, Stop
#define SIGTTOU 23    // TTY Output on Background, Stop

#define NR_SIG 23

int sendsig(uint pid, uint sig_no);

#endif
