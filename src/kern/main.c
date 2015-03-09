#include <system.h>
#include <const.h>
#include <console.h>
#include <asm.h>
#include <segment.h>
#include <mm.h>
#include <time.h>
#include <sched.h>
#include <blk.h>
#include <fs.h>
#include <inode.h>
#include <file.h>


void init() {
    struct inode *ip;

    current->pgrp = 1;
    do_mount(rootdev, NULL);
    current->wdir = ip;
    current->iroot = ip;
    ip->count += 2;
    iput(ip);

    do_open("/dev/tty0", O_RDWR, 0);
    do_dup(0);    // stdout
    do_dup(0);    // stderr

    do_fcntl(0, F_SETFD, 0);
    do_fcntl(1, F_SETFD, 0);
    do_fcntl(2, F_SETFD, 0);

//    do_exec("/bin/init", NULL);

    for(;;);
}

void kmain() {
    gdt_init();
    idt_init();
    video_init();
    timer_init();
    time_init();
    page_init();
    keyboard_init();
    sched_init();
    hd_init();
    tty_init();
    puts("Hello Constance!\n");
    sti();
    for(;;);
}
