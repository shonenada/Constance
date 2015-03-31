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
    gdt_init();    puts("gdt init..........done");
    idt_init();    puts("idt init..........done");
    video_init();    puts("video init..........done");
    timer_init();    puts("timer init..........done");
    time_init();    puts("time init..........done");
    page_init();     puts("page init..........done");
    keyboard_init();    puts("keyboard init..........done");
    sched_init();    puts("sched init..........done");
    hd_init();    puts("hd init..........done");
    tty_init();    puts("tty init..........done");
    puts("Hello Constance!\n");
    sti();
    for(;;);
}
