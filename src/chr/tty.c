#include <const.h>
#include <system.h>
#include <console.h>
#include <sched.h>
#include <tty.h>
#include <errno.h>
#include <sig.h>

struct tty ttys[NTTY];
#define MINOR(dev) ((uchar)((dev)&0xFF))

/**
 * Put a char into tty queue.
 */
int tty_enqueue(struct tty_queue* queue, char chr) {
    if (FULL(queue)) {
        return -1;
    }

    queue->buf[queue->tail] = chr;
    INC(queue);
    return 0;
}

/**
 * Get a char from a tty queue
 */
char tty_dequeue(struct tty_queue *queue) {
    char chr;
    if (EMPTY(queue)) {
        return -1;
    }
    chr = queue->buf[queue->head];
    DEC(queue);
    return chr;
}

/**
 * Parse raw char(get from read_q) into cooked (regular) char, 
 * and save the result to secondary queue list.
 */
int tty_parse(struct tty* tp) {
    int i;
    char chr;
    struct tty_queue *read_q, *secondary_q;
    
    // if TTY_RAW is set, do not parse any char
    if (tp->flag & TTY_RAW) {
        while((chr = tty_dequeue(&tp->read_q))) {
            tty_enqueue(&tp->secondary, chr);
        }
        return 0;
    }

    read_q = &(tp->read_q);
    secondary_q = &(tp->secondary);

    while(!EMPTY(read_q) && !FULL(secondary_q)) {
        chr = tty_dequeue(&tp->read_q);
        switch (chr) {
            // TODO
            case CTAB:
                for(i=0;i<4;i++)
                    tty_enqueue(&tp->secondary, ' ');
                return 0;
                break;
            case CERASE:
                tty_erase(&tp->secondary);
                break;
            default:
                tty_enqueue(&tp->secondary, chr);
                break;
        }
    }
    return 0;
}

int tty_open(ushort dev) {
    struct tty *tp;
    if (MINOR(dev) >= NTTY) {
        syserr(ENODEV);
        return -1;
    }
    tp = &ttys[MINOR(dev)];
    tp->flag = TTY_ECHO;
    tp->putc = &putch;
    tp->pgrp = current->pgrp;
    return 0;
}

int tty_print(struct tty* tp) {
    void (*putc)(char);
    char chr;
    putc = tp->putc;
    while (chr=tty_dequeue(&tp->write_q) >= 0) {
        putc(chr);
    }
    return 0;
}

int tty_output(struct tty* tp, char chr) {
    tty_enqueue(&tp->write_q, chr);
    return 0;
}

int tty_input(struct tty *tp, char chr) {
    struct tty_queue *secondary = &(tp->secondary);
    tty_enqueue(&tp->read_q, chr);
    tty_parse(tp);

    if (tp->flag & TTY_ECHO) {
        if (chr == CERASE) {
            if (EMPTY(secondary))
                return 0;
        }
        tty_output(tp, chr);
        tty_print(tp);
    }

    if ((tp->flag & TTY_RAW) == 0) {
        if (CINTR == chr) {
            sendsig(tp->pgrp, SIGINT);
        }
    }
    if (chr == CEOF || chr == '\n') {
        tty_erase(&tp->secondary);
        wakeup((uint)tp);
        return 0;
    }
    return 0;
}

int tty_read(ushort dev, char *buf, uint cnt) {
    int i;
    char chr;
    struct tty* tp;
    struct tty_queue *secondary = &(tp->secondary);

    if (MINOR(dev) >= NTTY) {
        syserr(ENODEV);
        return -1;
    }
    tp = &ttys[MINOR(dev)];
    if (LEFT(secondary) < cnt) {
        sleep((uint) tp);
    }
    for(i=0;i<cnt;i++) {
        if ((chr=tty_dequeue(&tp->secondary)) < 0) {
            break;
        }
        buf[i] = chr;
    }
    return (i + 1);
}


int tty_write(ushort dev, char *buf, uint cnt) {
    struct tty *tp;
    int i;

    if (MINOR(dev) >= NTTY) {
        syserr(ENODEV);    // TODO
        return -1;
    }
    tp = &ttys[MINOR(dev)];
    for(i=0;i<cnt;i++) {
        tty_output(tp, buf[i]);
    }
    tty_print(tp);
    return 0;
}

void tty_init() {
    ttys[0].flag = TTY_ECHO;
    ttys[0].putc = &putch;
}

char tty_erase(struct tty_queue *tp) {
    if (FULL(tp)) {
        return -1;
    }
    tp->tail = (tp->tail - 1 + TTY_BUF_SIZE) % TTY_BUF_SIZE;
    return 0;
}
