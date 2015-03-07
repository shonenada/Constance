#ifndef __UNISTD_H
#define __UNISTD_H
#include <segment.h>

#define MAX_SYSCALL 10

#define __NR_nosys 0

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

static inline _syscall0(int, nosys);

#endif
