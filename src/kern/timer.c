#include <system.h>
#include <segment.h>
#include <time.h>
#include <console.h>

static uint timer_ticks = 0;

void set_timer_phaser(int hz) {
    int div = 1193180 / hz;
    outportb(0x43, 0x36);
    outportb(0x40, div & 0xFF);
    outportb(0x40, div >> 8);
}

int timer_handler(struct regs *r) {
    timer_ticks++;
    if (timer_ticks % TIMER_HZ == 0) {
    }
    return 0;
}

void timer_init() {
    set_timer_phaser(TIMER_HZ);
    irq_install(0, timer_handler);
}
