#include <system.h>
#include <const.h>
#include <inode.h>
#include <vm.h>
#include <mm.h>
#include <sched.h>
#include <a.out.h>

int vm_clone(struct vm *to) {
    int i;
    struct vma *vp;

    to->pd = (struct pde*)kmalloc(PAGE_SIZE);
    pgd_init(to->pd);

    for(i=0;i<NVMA;i++) {
        vp = &(current->tvm.area[i]);
        if (vp->flag != 0) {
            if (vp->ino) {
                vp->ino->count++;
            }
            to->area[i] = *vp;
        }
    }
    pt_copy(to->pd, current->tvm.pd);
    return 0;
}

int vm_clear(struct vm *vm) {
    int i;
    struct vma *vp;
    for(i=0;i<NVMA;i++) {
        vp = &vm->area[i];
        if (vp->flag != 0) {
            if (vp->ino) {
                iput(vp->ino);
            }
            vp->ino = NULL;
            vp->flag = 0;
            vp->base = 0;
            vp->size = 0;
        }
    }
    pt_free(vm->pd);
    return 0;
}

int vm_renew(struct vm *vm, struct ahead *ah, struct inode *ip) {
    uint base, text, data, bss, heap;
    base = ah->entry - sizeof(struct ahead);
    text = ah->entry - sizeof(struct ahead);
    data = text + ah->tsize;
    bss = data + ah->bsize;
    heap = bss + ah->bsize;

    pgd_init(vm->pd);
    vm->entry = ah->entry;
    vma_init(&(vm->text), text, ah->tsize, VMA_MMAP | VMA_RDONLY | VMA_PRIVATE, ip, text - base);
    vma_init(&(vm->data), data, ah->dsize, VMA_MMAP | VMA_PRIVATE, ip, data - base);
    vma_init(&(vm->bss), bss, ah->bsize, VMA_ZERO | VMA_PRIVATE, NULL, 0);
    vma_init(&(vm->heap), heap, PAGE_SIZE, VMA_ZERO | VMA_PRIVATE, NULL, 0);
    vma_init(&(vm->stack), VM_STACK, PAGE_SIZE, VMA_STACK | VMA_ZERO, VMA_PRIVATE, NULL, 0);
    return 0;
}

int vm_verify(void* vaddr, uint size) {
    uint page;
    struct pte *pte;
    uint addr = (uint)vaddr;

    if (addr < KMEM_END || size < 0) {
        return -1;
    }
    for (page=PG_ADDR(addr);page<=PG_ADDR(addr+size-1);page+=PAGE_SIZE) {
        pte = find_pte(current->tvm.pd, page, 1);
        if ((pte->flag & PTE_P) == 0) {
            do_no_page(page);
        } else if ((pte->flag & PTE_RW) == 0) {
            do_wp_page(page);
        }
    }
    return 0;
}

struct vma* find_vma(uint addr) {
    struct vma *vma;
    struct vma *vp;
    vma = current->tvm.area;
    for(vp=&vma[0];vp<&vma[NVMA]; vp++) {
        if (addr >= vp->base && addr < vp->base + vp->size) {
            return vp;
        }
    }
    return NULL;
}

int vma_init(struct vma *vp, uint base, uint size, uint flag, struct inode *ip, uint ioff) {
    vp->flag = flag;
    vp->base = base;
    vp->size = size;
    vp->off = ioff;
    if (ip) {
        ip->count++;
        vp->ino = ip;
    }
    return 0;
}
