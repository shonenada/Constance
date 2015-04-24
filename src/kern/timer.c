#include <system.h>
#include <segment.h>
#include <time.h>
#include <console.h>
#include <sched.h>

static uint timer_ticks = 0;
static uint wait_timer = 0;

void set_timer_phaser(int hz) {
    int div = 1193180 / hz;
    outportb(0x43, 0x36);
    outportb(0x40, div & 0xFF);
    outportb(0x40, div >> 8);
}

int timer_handler(struct regs *r) {
    timer_ticks++;
    if (timer_ticks % TIMER_HZ == 0) {
        sched_cpu();
    }
    return 0;
}

void timer_init() {
    set_timer_phaser(TIMER_HZ);
    irq_install(0, timer_handler);
}

void timer_wait(int sec) {
    long eticks;
    eticks = timer_ticks + sec;
    while(timer_ticks < eticks);
}
