#include <system.h>
#include <const.h>
#include <errno.h>
#include <unistd.h>
#include <buf.h>
#include <mm.h>
#include <page.h>
#include <a.out.h>

int do_exec(char *path, char **args) {
    uint bn;
    char **tmp;
    struct ahead *ah;
    struct inode *ip;
    struct buf* bp;

    ip = namei(path, 0);
    if (ip == NULL) {
        return syserr(ENOENT);
    }
    // first block
    bn = bmap(ip, 0, 0);
    if (bn == 0) {
        syserr(EINVAL);
        goto _badf;
    }

    bp = buf_read(ip->idev, bn);
    ah = (struct ahead*) bp->data;

    if (ah->magic != AOUT_MAGIC) {
        syserr(EINVAL);
        goto _badf;
    }

    tmp = store_argv(path, argv);

    return 0;
}

int upush_argv(uint *esp, char *tmp[]) {
    uint arglen, argc;
    int i, r, tmp_esp;
    char *str, **uargv;

    argc = 0;

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
    tmp -= len;
    if (pte_verify((void*)tmp, len) < 0) {
        panic("upush(): ");
    }
    memcpy(tmp, buf, len);
    return (*esp=tmp);
}

