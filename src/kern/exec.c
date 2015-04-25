#include <system.h>
#include <const.h>
#include <errno.h>
#include <unistd.h>
#include <buf.h>
#include <sched.h>
#include <mm.h>
#include <page.h>
#include <inode.h>
#include <elf.h>
#include <vm.h>
// #include <a.out.h>    not support yet

char** store_argv(char* path, char **argv) ;

int do_exec(char *path, char **args) {
    uint i, bn, off, in_off, size, esp, argc;
    char **tmp_args;
    struct buf *bp;
    struct elfh hp;
    struct page* pg;
    struct inode *ip;
    struct progh phead;
    struct pde* pdir;

    ip = namei(path, 0);
    if (ip == NULL) {
        syserr(ENOENT);
        goto _badf;
    }

    bn = bmap(ip, 0, 0);
    if (bn == 0) {
        syserr(EINVAL);
        goto _badf;
    }

    if(readi(ip, (char*)&hp, 0, sizeof(struct elfh)) != sizeof(struct elfh)) {
        syserr(ENOENT);
        goto _badf;
    }
    if (hp.magic != ELF_MAGIC) {
        syserr(EINVAL);
        goto _badf;
    }

    tmp_args = store_argv(path, args);
    for(i=0, off=hp.phoff;i<hp.phnum; i++, off+=sizeof(struct progh)) {
        if (readi(ip, (char*)&phead, off, sizeof(struct progh)) != sizeof(struct progh)) {
            syserr(EINVAL);
            goto _badf;
        }
        if (phead.type == ELF_PROG_LOAD) {
            if (phead.memsz < phead.filesz)
                goto _badf;
            if (size = allocuvm(&(current->pdir), size, phead.vaddr + phead.memsz) == 0)
                goto _badf;
            if (loaduvm(&(current->pdir), (char*)phead.vaddr, ip, phead.off, phead.filesz) < 0)
                goto _badf;
            break;
        }
    }

    esp = 0x400000;
    argc = upush_argv(&esp, tmp_args);
    upush(&esp, (char*)&argc, sizeof(uint));
    iput(ip);
    //current->rgs->eip = hp.entry;
    //current->rgs->esp = esp;
    _jump_u(hp.entry, esp);
    //jump_to_user_mode();
    return 0;

_badf:
    iput(ip);
    return -1;
}

char** store_argv(char* path, char **argv) {
    char **tmp;
    int i, argc;

    argc = 0;
    if (argv != NULL)
        while(argv[argc++]);

    argc += 1;

    tmp = (char**) kmalloc(PAGE_SIZE);
    tmp[0] = (char*) kmalloc(PAGE_SIZE);
    strncpy(tmp[0], path, PAGE_SIZE-1);
    tmp[0][PAGE_SIZE-1] = '\0';
    for(i=1;i<argc;i++) {
        tmp[i] = (char*)kmalloc(PAGE_SIZE);
        strncpy(tmp[i], argv[i-1], PAGE_SIZE-1);
        tmp[i][PAGE_SIZE-1] = '\0';
    }
    tmp[argc] = NULL;
    return tmp;
}

int upush_argv(uint *esp, char *tmp[]) {
    uint argc, arglen;
    int i, r, tmp_esp;
    char *str, **uargv;

    argc = 0;
    arglen = 0;
    if (tmp != NULL) {
        for (i=0;(str=tmp[i])!=NULL;i++) {
            arglen += strlen(str) + 1;
            argc++;
        }
    }
    arglen += sizeof(char*) * argc;
    if (pte_verify((void*)(*esp-arglen), arglen) < 0) {
        syserr(EINVAL);
        return -1;
    }
    uargv = (char**)(*esp - arglen);
    for (i=argc-1;i>=0;i--) {
        str = tmp[i];
        upush(esp, str, strlen(str)+1) ;
        uargv[i] = (char*) *esp;
    }
    *esp = (uint) uargv;
    upush(esp, (char*)&uargv, sizeof(uint));
    return argc;
}

int upush(uint *esp, char *buf, int len) {
    uint tmp = *esp;
    char* tmp_test = (char*) tmp;
    tmp -= len;
    if (pte_verify((void*)tmp, len) < 0) {
        panic("upush(): ");
    }
    memcpy(tmp, buf, len);
    return (*esp=tmp);
}

