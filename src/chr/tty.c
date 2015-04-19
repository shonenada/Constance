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
    
    read_q = &(tp->read_q);
    secondary_q = &(tp->secondary);

    // if TTY_RAW is set, do not parse any char
    if (tp->flag & TTY_RAW) {
        while((chr = tty_dequeue(read_q)) >= 0) {
            tty_enqueue(secondary_q, chr);
        }
        return 0;
    }

    while((chr = tty_dequeue(read_q)) >= 0) {
        switch (chr) {
            // TODO
            case CTAB:
                for(i=0;i<4;i++)
                    tty_enqueue(secondary_q, ' ');
                break;
            case CERASE:
                // need to refactor
                if (COUNT(secondary_q) == 1) {
                    tty_output(tp, chr);
                    tty_print(tp);
                }

                tty_erase(secondary_q);
                break;
            default:
                tty_enqueue(secondary_q, chr);
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
    tp->read_q.head = tp->read_q.tail = 0;
    tp->write_q.head = tp->write_q.tail = 0;
    tp->secondary.head = tp->secondary.tail = 0;
    return 0;
}

int tty_close(ushort dev) {
    if (MINOR(dev) >= NTTY) {
        syserr(ENODEV);
        return -1;
    }
    return 0;
}

int tty_print(struct tty* tp) {
    void (*_putc)(char);
    char chr;
    _putc = tp->putc;
    while ((chr=tty_dequeue(&tp->write_q)) >= 0) {
        _putc(chr);
    }
    return 0;
}

int tty_output(struct tty* tp, char chr) {
    tty_enqueue(&tp->write_q, chr);
    return 0;
}

int tty_input(struct tty *tp, char chr) {
    struct tty_queue *secondary = &(tp->secondary);
    tty_enqueue(&(tp->read_q), chr);
    tty_parse(tp);

    if (tp->flag & TTY_ECHO) {
        if (chr == CERASE) {
            if (COUNT(secondary) <= 0)
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
        tty_erase(secondary);
        wakeup((uint)tp);
        return 0;
    }
    return 0;
}

int tty_read(ushort dev, char *buf, uint cnt) {
    int i;
    char chr;
    struct tty* tp;
    struct tty_queue *secondary;

    if (MINOR(dev) >= NTTY) {
        syserr(ENODEV);
        return -1;
    }
    tp = &ttys[MINOR(dev)];
    secondary = &(tp->secondary);
    if (COUNT(secondary) < cnt) {
        sleep((uint) tp, PRITTY);
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
        syserr(ENODEV);
        return -1;
    }
    tp = &ttys[MINOR(dev)];
    for(i=0;i<cnt;i++) {
        tty_output(tp, buf[i]);
    }
    tty_print(tp);
    return 0;
}

char tty_erase(struct tty_queue *tp) {
    if (EMPTY(tp)) {
        return -1;
    }
    tp->tail = (tp->tail - 1 + TTY_BUF_SIZE) % TTY_BUF_SIZE;
    return 0;
}

void tty_init() {
    ttys[0].flag = TTY_ECHO;
    ttys[0].putc = &putch;
    ttys[0].read_q.head = ttys[0].read_q.tail = 0;
    ttys[0].write_q.head = ttys[0].write_q.tail = 0;
    ttys[0].secondary.head = ttys[0].secondary.tail = 0;
}

void dump_tty(struct tty *tp) {
    printk("tty(%x): pgrp=%x, dev=%x, putc=%x, flag=%b, read_q=%x, write_q=%x, secondary=%x\n", tp, tp->pgrp, tp->dev, tp->putc, tp->flag, tp->read_q, tp->write_q, tp->secondary);
}

void dump_tty_queue(struct tty_queue *qp) {
    int i;
    printk("tty_queue(%x): head=%x, tail=%x, buf=%x\n", qp,
            qp->head,
            qp->tail,
            qp->buf);
    printk("buf: ");
    for(i=0;i<TTY_BUF_SIZE;i++) {
        putch(qp->buf[i]);
    }
    printk("\n");
}
