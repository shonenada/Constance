#ifndef __A_OUT_H
#define __A_OUT_H

struct ahead {
    uint magic;
    uint tsize;
    uint dsize;
    uint bsize;
    uint syms;
    uint entry;
    uint trsize;
    uint drsize;
};

#define NMAGIC 0x6400CC

#endif
