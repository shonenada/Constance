#ifndef __VM_H
#define __VM_H

#include <mm.h>
#include <inode.h>

#define NVMA 5

#define VMA_RDONLY 0x1
#define VMA_STACK 0x2
#define VMA_ZERO 0x4
#define VMA_MMAP 0x8
#define VMA_PRIVATE 0x10

#define VM_STACK 0x80000000


struct vma {
    uint flag;
    uint base;
    uint size;
    struct inode *ino;
    uint off;
};

struct vm {
    struct pde *pd;
    uint entry;
    struct vma area[0];
    struct vma text;
    struct vma data;
    struct vma bss;
    struct vma heap;
    struct vma stack;
};


#endif
