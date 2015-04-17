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

    do_mount(rootdev, NULL);
    ip = iget(rootdev, 1);
    current->pgrp = 1;
    current->wdir = ip;
    current->iroot = ip;
    ip->count += 2;
    iput(ip);

    if(do_open("/dev/tty0", O_RDWR, 0) < 0) {
        panic("bad /dev/tty0 \n");
    }

    do_dup(0);    // stdout
    do_dup(0);    // stderr

    do_fcntl(0, F_SETFD, 0);
    do_fcntl(1, F_SETFD, 0);
    do_fcntl(2, F_SETFD, 0);

    do_exec("/bin/test", NULL);

    for(;;) {
    }
}

void print_hello() {
    printk("\n");
    printk("                 _____                 _                       \n");
    printk("                /  __ \\               | |                      \n");
    printk("                | /  \\/ ___  _ __  ___| |_ __ _ _ __   ___ ___ \n");
    printk("                | |    / _ \\| '_ \\/ __| __/ _` | '_ \\ / __/ _ \\\n");
    printk("                | \\__/| (_) | | | \\__ | || (_| | | | | (_|  __/\n");
    printk("                 \\____/\\___/|_| |_|___/\\__\\__,_|_| |_|\\___\\___|\n\n");
}

void kmain() {
    gdt_init();    puts("gdt init..........done\n");
    idt_init();    puts("idt init..........done\n");
    video_init();    puts("video init..........done\n");
    page_init();     puts("page init..........done\n");
    task0_init();    puts("task0 init.........done\n");
    time_init();    puts("time init..........done\n");
    timer_init();    puts("timer init..........done\n");
    buf_init();    puts("buf init..........done\n");
    hd_init();    puts("hd init..........done\n");
    keyboard_init();    puts("keyboard init..........done\n");
    tty_init();    puts("tty init..........done\n");
    sti();

    print_hello();

    kspawn(&init);

    for(;;) {
        sti();
        schedule();
    }
}
