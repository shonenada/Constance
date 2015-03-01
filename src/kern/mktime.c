#include <system.h>

static long startup_ts;

// Read time information from CMOS
#define CMOS_READ(addr) ({outportb(0x80|addr,0x70);inportb(0x71);})

#define RTC_YEAR 9
#define RTC_MONTH 8
#define RTC_DAY 7
#define RTC_HOUR 4
#define RTC_MINUTE 2
#define RTC_SECOND 0

#define BCD2BIN(num) ((num)=((num)&15)+((num)>>4)*10)

#define MINUTE 60
#define HOUR (MINUTE*60)
#define DAY (HOUR*24)
#define YEAR (DAY*365)

static int mdays[12] = {
    0,
    DAY*(31),
    DAY*(31+29),
    DAY*(31+29+31),
    DAY*(31+29+31+30),
    DAY*(31+29+31+30+31),
    DAY*(31+29+31+30+31+30),    // 6
    DAY*(31+29+31+30+31+30+31),
    DAY*(31+29+31+30+31+30+31+31),
    DAY*(31+29+31+30+31+30+31+31+30),
    DAY*(31+29+31+30+31+30+31+31+30+31),
    DAY*(31+29+31+30+31+30+31+31+30+31+30),
};

void cmos_time(struct time* tm) {
    do {
        tm->tm_year = CMOS_READ(RTC_YEAR);
        tm->tm_month = CMOS_READ(RTC_MONTH);
        tm->tm_day = CMOS_READ(RTC_DAY);
        tm->tm_hour = CMOS_READ(RTC_HOUR);
        tm->tm_minute = CMOS_READ(RTC_MINUTE);
        tm->tm_second = CMOS_READ(RTC_SECOND);
    } while(tm->tm_second != CMOS_READ(RTC_SECOND));

    BCD2BIN(tm->tm_year);
    BCD2BIN(tm->tm_month);
    BCD2BIN(tm->tm_day);
    BCD2BIN(tm->tm_hour);
    BCD2BIN(tm->tm_minute);
    BCD2BIN(tm->tm_second);

    tm->tm_month--;
}

long kern_ts(struct time* tm) {
    long ts; 
    int y;
    y = tm->tm_year - 1970;
    ts = y * YEAR + ((y+1)/4) * DAY;    // leap-year
    ts += mdays[tm->tm_month];
    if (tm->tm_month > 1 && ((y+2)%4))
        tm -= DAY;
    tm += DAY * (tm->tm_day - 1);
    tm += HOUR * (tm->tm_hour);
    tm += MINUTE * (tm->tm_minute);
    tm += tm->tm_second;
    return ts;
}

// Set timestamp from CMOS when os sstart up.
// Refer from linux
void time_init() {
    struct time tm;
    cmos_time(&tm);
    startup_ts = kern_ts(&tm);
}

long timestamp() {
    return startup_ts;
}
