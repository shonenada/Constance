#ifndef __VM_H
#define __VM_H

int allocuvm(struct pde *pdir, uint oldsize, uint newsize);

#define PGROUNDUP(size) (((size)+PAGE_SIZE-1) & ~(PAGE_SIZE-1))
#define PGROUNDDOWN(addr) ((addr) & ~(PAGE_SIZE - 1))

#endif
