#ifndef __ELF_H
#define __ELF_H

#define ELF_MAGIC 0x464c457F

struct elfh {
    uint magic;
    uchar elf[12];
    ushort type;
    ushort machine;
    uint version;
    uint entry;
    uint phoff;
    uint shoff;
    uint flag;
    ushort ehsize;
    ushort tisze;
    ushort phnum;
    ushort entsize;
    ushort shnum;
    ushort shstrndx;
};

struct progh {
    uint type;
    uint off;
    uint vaddr;
    uint paddr;
    uint filesz;
    uint memsz;
    uint flags;
    uint align;
};

#define ELF_PROG_LOAD 1

void dump_elfh(struct elfh* hp) {
    printk("magic=%x entry=%x\n", hp->magic, hp->entry);
}

void dump_progh(struct progh *hp) {
    printk("type=%x, off=%x, vaddr=%x, paddr=%x, filesz=%d, memsz=%d, flags=%b, align=%x\n", hp->type, hp->off, hp->vaddr, hp->paddr, hp->filesz, hp->memsz, hp->flags, hp->align);
}

#endif
