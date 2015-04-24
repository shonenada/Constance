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
    uint max_size;    // max file size
    ushort magic;    // 138F
    ushort state;
};

// define super block struct for memory
struct sblk {
    ushort ninodes;    // number of inodes
    ushort nzones;    // number of data zones
    ushort imap_blks;    // space used by inode map. (blocks)
    ushort zmap_blks;    // space used by zone map. (blocks)
    ushort zone0;    // first data zone 
    ushort log_zone_size;    // size of data zone = (1024 << log_zone_size)
    uint max_size;    // max file size
    ushort magic;    // 138F
    ushort state;

    ushort dev;
    uint flag;
    struct inode * imnt;
};

#define S_MAGIC 0x138F

#define S_LOCK 0x1
#define S_WANTED 0x2
#define S_RDONLY 0x4
#define S_DIRTY 0x8

struct sblk mnt[NMOUNT];

// define directory entry
struct dire {
    ushort inode;
    char name[DIRSIZE];
    char __padding[16];
};

struct sblk *getsblk(ushort dev);
int balloc (ushort dev);
int bfree (ushort dev, uint nr);
int bzero (ushort dev, uint bn);
int ialloc (ushort dev);
int ifree (ushort dev, uint ino);
void unlink_sb(struct sblk *sb);
int find_free(uchar *bitmap, int size);
int sblk_load(struct sblk *sbp);
int sblk_update(struct sblk *sbp);
void putsp(struct sblk* sbp);
uint bmap(struct inode *ip, uint bn, uchar creat);
int do_read(int fd, char* buf, int cnt);
int do_write(int fd, char *buf, int cnt);
int do_lseek(uint fd, int off, int whence);
int readi(struct inode *ip, char *buf, uint off, uint cnt);
int writei(struct inode* ip, char *buf, uint off, uint cnt);
struct sblk* do_mount(ushort dev, struct inode *ip);

int do_ls(char* path);
int do_cd(char* path);
int do_cat(char* path);

void dump_sblk(struct sblk* blk);
void dump_mnts();

// Inodes per block
#define IPB (BLK_SIZE/SIZE_INODE)
// Block containing block i
#define IBLOCK(i) ((i) / IPB + 2)
// Bitmap per block
#define BPB (BLK_SIZE*8)

#define BBLOCK(sp,nr) (((sp)->imap_blks) + ((nr)/BPB) + 2) 
#define INOBLOCK(sp,ino) (((sp)->imap_blks) + ((sp)->zmap_blks) + (((ino)-1)/IPB) + 2)

#define NAMELEN 12

#define NODEV (DEVNO(0,0))
#define DEVNO(major,minor) ((ushort)(((uchar)major<<8)+(uchar)minor))

extern ushort rootdev;

#endif
