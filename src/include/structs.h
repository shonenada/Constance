#ifndef __STRUCTS_H
#define __STRUCTS_H

struct vchar {
    short v_char:8;
    char v_fgcolor:4;
    char v_bgcolor:4;
};
#define SIZE_VCHAR sizeof(struct vchar)

#endif
