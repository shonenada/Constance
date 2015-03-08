#ifndef __UNISTD_H
#define __UNISTD_H
#include <segment.h>

#define MAX_SYSCALL 10

enum __NR {
    __NR_nosys,
    __NR_fork,
    __NR_read,
    __NR_write,
    __NR_open,
    __NR_close,
    __NR_waitpid,
    __NR_creat,
    __NR_link,
    __NR_unlink,
    __NR_chdir,
};

// syscall with no params
#define _syscall0(type,name) \
type name(void) {\
    int _res;\
    asm volatile ("int $0x80" \
        : "=a" (_res)\
        : "0" (__NR_##name)); \
    if (_res < 0) {\
        errno = -_res;\
        return -1;\
    } else {\
        return (type) _res;\
    }\
}

// syscall with 1 param
#define _syscall1(type,name,atype) \
type name(atype ap) {\
    int _res;\
    asm volatile ("int $0x80" \
        : "=a" (_res) \
        : "0" (__NR_##name), "b" ((int) (ap)));\
    if (_res < 0) { \
        errno = -_res; \
        return -1; \
    }\
    return (type) _res;\
}

// syscall with 2 params
#define _syscall2(type,name,atype,btype) \
type name(atype ap,btype bp) { \
    int _res;\
    asm volatile ("int $0x80" \
        : "=a" (_res) \
        : "0" (__NR_##name), "b" ((int) (ap)), "c" ((int) (bp)));\
    if (_res < 0) { \
        errno = -_res;\
        return -1;\
    }\
    return (type) _res;\
}

// syscall with 3 params
#define _syscall3(typem,name,atype,btype,ctype) \
type name(atype ap,btype bp,ctype cp) { \
    int _res;\
    asm volatile ("int $0x80" \
        : "=a" (_res) \
        : "0" (__NR_#name), "b" ((int) (ap)), "c" ((int) (bp)), "d" ((int) (dp)));\
    if (_res < 0) {\
        errno = -_res;\
        return -1;\
    }\
    return (type) _res;\
}

extern int errno;

int do_syscall(struct regs *);

int sys_nosys(struct regs* rgs); 
int sys_fork(struct regs* rgs);
int sys_read(struct regs* rgs);
int sys_write(struct regs* rgs);
int sys_open(struct regs* rgs);
int sys_close(struct regs* rgs);
int sys_waitpid(struct regs* rgs);
int sys_creat(struct regs* rgs);
int sys_link(struct regs* rgs);
int sys_unlink(struct regs* rgs);
int sys_chdir(struct regs* rgs);

static inline _syscall0(int, nosys);
static inline _syscall0(int, fork);
static inline _syscall0(int, read);
static inline _syscall0(int, write);
static inline _syscall0(int, open);
static inline _syscall0(int, close);
static inline _syscall0(int, waitpid);
static inline _syscall0(int, creat);
static inline _syscall0(int, link);
static inline _syscall0(int, unlink);
static inline _syscall0(int, chdir);

#endif
