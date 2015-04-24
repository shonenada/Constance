#ifndef __FILE_H
#define __FILE_H

#include <stat.h>

struct file {
    char flags;
    char count;
    uint offset;
    struct inode *ino;
};

extern struct file files[NFILE];

// define flag for open/fcntl
#define O_ACCMODE 0x3

#define O_RDONLY 0x0
#define O_WRONLY 0x1
#define O_RDWR 0x2
#define O_APPEND 0x40
#define O_CREAT 0x4
#define O_EXCL 0x8
#define O_NOCTTY 0x10
#define O_TRUNC 0x20
#define O_NONBLOCK 0x80

// for fcntl
#define F_DUPFD 0x0
#define F_GETFD 0x1
#define F_SETFD 0x2
#define F_GETFL 0x3
#define F_SETFL 0x4
#define F_GETLK 0x5
#define F_SETLK 0x6
#define F_SETLKW 0x7

#define FD_CLOEXEC 0x1

#define SEEK_SET 0x0
#define SEEK_CUR 0x1
#define SEEK_END 0x2

// for accessing files
#define F_OK 0x0
#define X_OK 0x1
#define W_OK 0x2
#define R_OK 0x4

#define MAX_FILESIZE ((7+NINDBLK+NINDBLK*NINDBLK)*BLK_SIZE)

int do_creat(char* path, int mode);
int do_mknod(char *path, int mode, ushort dev);
int do_stat(struct inode *ip, struct stat *sbuf);
int do_access(struct inode *ip, uint mode);
int do_open(char *path, uint flag, uint mode);
int do_close(int fd);
int do_dup(int fd);
int find_entry(struct inode* dip, char *name, uint len);
int unlink_entry(struct inode *dip, char *name, int len);
uint link_entry(struct inode *dip, char *name, uint len, uint ino);
struct file* falloc(int fd);
int ufalloc();
int do_dup2(int fd, int newfd);
struct inode * _namei(char *path, uchar creat, uchar parent, char **name); 
struct inode* namei(char *path, uchar creat);
struct inode* namei_parent(char *path, char **name);

void dump_file(struct file *fp);

#endif
