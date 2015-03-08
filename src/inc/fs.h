#ifndef __FS_H
#define __FS_H

#define S_LOCK 0x1
#define S_WANTED 0x2

#define DIRSIZE 14
#define NINDBLK (BLK_SIZE/sizeof(ushort))


// define super block struct
struct d_sblk {
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

    ushort dev;
    uint flag;
    struct inode * imnt;
};

#define S_LOCK 0x1
#define S_WANTED 0x2
#define S_RDONLY 0x4
#define S_DIRTY 0x8

struct sblk mnt[NMOUNT];

// define directory entry
struct dire {
    ushort inode;
    char name[DIRSIZE];
};

struct sblk *getsblk(ushort dev);
int balloc (ushort dev);
int bfree (ushort dev, uint nr);
int bzero (ushort dev, uint bn);
int ialloc (ushort dev);
int ifree (ushort dev, uint ino);
void unlink_sb(struct ksblk *sb);
int find_free(uchar *bitmap, int size);

// Inodes per block
#define IPB (BLK_SIZE/SIZE_INODE)
// Block containing block i
#define IBLOCK(i) ((i) / IPB + 2)
// Bitmap per block
#define BPB (BLK_SIZE*8)

#define BBLOCK(sp,nr) (((sp)->imap_blks) + ((nr)/BPB) + 2) 

#endif
