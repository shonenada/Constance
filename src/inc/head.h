#ifndef __HEAD_H
#define __HEAD_H

struct a_out_head {
    uint magic;
    uint tsize;
    uint dsize;
    uint bsize;
    uint syms;
    uint entry;
    uint trsize;
    uint drsize;
};

#endif
