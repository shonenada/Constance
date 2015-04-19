#ifndef __TTY_H
#define __TTY_H

#define TTY_BUF_SIZE 1024

struct tty_queue {
    uint head;
    uint tail;
    uchar buf[TTY_BUF_SIZE];
};

struct tty {
    uint pgrp;
    uint dev;
    void (*putc)(char);    // print char to screen
    uint flag;
    struct tty_queue read_q;
    struct tty_queue write_q;
    struct tty_queue secondary;
};

extern struct tty ttys[NTTY];

// define flags
#define TTY_RAW 0x0
#define TTY_CANON 0x1
#define TTY_ECHO 0x2

#define EMPTY(tq) (tq->head==tq->tail)
#define LEFT(tq) ((tq->tail-tq->head-1) - (TTY_BUF_SIZE-1))
#define FULL(tq) (LEFT(tq)==0)
#define INC(tq) (tq->tail=(tq->tail+1)%TTY_BUF_SIZE)
#define DEC(tq) (tq->head=(tq->head+1)%TTY_BUF_SIZE)

#define CERASE '\b'
#define CTAB '\t'
#define CCR 13
#define CNL 10
#define CINTR 28    // ^C
#define CQUIT 034    /* ^/ */
#define CKILL 025    // ^X
#define CEOF 004    // ^D
#define CEOL 00
#define CSTART 021    // ^Q
#define CSTOP 023    // ^S
#define CSUSP 032    // ^Z

int tty_enqueue(struct tty_queue* queue, char chr);
char tty_dequeue(struct tty_queue *queue);
int tty_print(struct tty* tp);
int tty_output(struct tty* tp, char chr);
int tty_input(struct tty *tp, char chr);
int tty_read(ushort dev, char *buf, uint cnt);
int tty_write(ushort dev, char *buf, uint cnt);
void tty_init();
char tty_erase(struct tty_queue *tq);
int tty_open(ushort dev);
int tty_close(ushort dev);

#endif
