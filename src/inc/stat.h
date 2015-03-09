#ifndef __STAT_H
#define __STAT_H

struct stat {
    ushort dev;
    ushort ino;
    ushort mode;
    ushort nlink;
    ushort uid;
    ushort gid;
    uint size;
    ushort atime;
    ushort mtime;
    ushort ctime;
};

#define S_IFMT 00170000
#define S_IFREG 0100000
#define S_IFBLK 0060000
#define S_IFDIR 0040000
#define S_IFCHR 0020000
#define S_IFIFO 0010000
#define S_ISUID 0004000
#define S_ISGID 0002000
#define S_ISVTX 0001000

// access
#define RWXMODES 000777
#define R_BIT 0000004
#define W_BIT 0000002
#define X_BIT 0000001

#endif
