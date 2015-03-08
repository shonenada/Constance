#ifndef __FS_H
#define __FS_H

#define S_LOCK 0x1
#define S_WANTED 0x2

#define DIRSIZE 14

// define super block struct
struct sblk {
    ushort ninodes;    // number of inodes
    ushort nzones;    // number of data zones
    ushort imap_blks;    // space used by inode map. (blocks)
    ushort zmap_blks;    // space used by zone map. (blocks)
    ushort zone0;    // first data zone 
    ushort log_zone_size;    // size of data zone = (1024 << log_zone_size)
    unsigned long max_size;    // max file size
    ushort magic;    // minix 14/30 ID number
    ushort state;    // mount state.
};

// define super block struct for memory
struct m_sblk {
    ushort ninodes;    // number of inodes
    ushort nzones;    // number of data zones
    ushort imap_blks;    // space used by inode map. (blocks)
    ushort zmap_blks;    // space used by zone map. (blocks)
    ushort zone0;    // first data zone 
    ushort log_zone_size;    // size of data zone = (1024 << log_zone_size)
    unsigned long max_size;    // max file size
    ushort magic;    // minix 14/30 ID number
    ushort state;    // mount state.

    ushort dev;
    uint flag;
};

// define inode
struct inode {
    ushort mode;
    ushort uid;
    unsigned long size;
    unsigned long time;
    uchar gid;
    uchar nlinks;
    ushort zone[9];
};

// define directory entry
struct dire {
    ushort inode;
    char name[DIRSIZE];
};


struct m_sblk *getsblk(ushort dev);
int balloc (ushort dev);
int bfree (ushort dev);
int bzero (ushort dev);
int ialloc (ushort dev);
int ifree (ushort dev);

#endif
