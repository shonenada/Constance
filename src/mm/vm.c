#include <system.h>
#include <const.h>
#include <vm.h>
#include <head.h>

int vm_clone(struct vm *target) {
}

int vm_clear(struct vm *vm) {
}

int re_renew(struct vm *vm, struct ahead *ah, struct inode *ip) {
}

int vm_verify(void *vaddr, uint size) {
}

struct vma *find_vma(uint addr) {
}

int vma_init(struct vma *vp, uint base, uint size, uint flag, struct inode *ip, uint ioff) {
}
