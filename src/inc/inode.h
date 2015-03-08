#ifndef __INODE_H
#define __INODE_H

#define ROOTINO 1

// define inode for disk
struct d_inode {
    ushort mode;
    ushort uid;
    unsigned long size;
    unsigned long time;
    uchar gid;
    uchar nlinks;
    ushort zone[9];
};

// define inode for memory
struct inode {
    ushort mode;
    ushort uid;
    unsigned long size;
    unsigned long time;
    uchar gid;
    uchar nlinks;
    ushort zone[9];

    ushort count;
    ushort idev;
    ushort inum;
    ushort flag;
};
#define SIZE_INODE sizeof(struct inode)

#define NINODE 128

#define I_LOCK 0x1
#define I_WANTED 0x2
#define I_DIRTY 0x4
#define I_MOUNT 0x8
#define I_TEXT 0x10

void iupdate(struct inode *ip);

#endif
