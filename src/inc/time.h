#ifndef __TIME_H
#define __TIME_H

struct time {
    int tm_year;
    int tm_month;
    int tm_day;
    int tm_hour;
    int tm_minute;
    int tm_second;
};
#define SIZE_TIME sizeof(struct time)

#define MASTER_PIC_PORT 0x20
#define SLAVE_PIC_PORT 0xA0

#define TIMER_HZ 100

void timer_init();
void keyboard_init();

void time_init();
long timestamp();
void timer_wait(int sec);

#endif
