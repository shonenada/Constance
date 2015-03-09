File System
===========

Definitions
-----------

inode: 保存文件的信息（除了文件名）
block: 一个尺寸单位。对于大多数设备来说，1 block = 1024b. (BLOCK_SIZE)
zone: disk 中存放文件数据的区域
super block: disk 的第一个 block，保存文件系统的类型和大小信息。

Physical Layout
---------------

+-----------------+--------------------------------+------------------------------------+
|                 | Size (Blocks)                  | Description                        |
+-----------------+--------------------------------+------------------------------------+
| Blank Block     | 1                              | Reserved for partition boot code.  |
+-----------------+--------------------------------+------------------------------------+
| Super Block     | 1                              | 文件系统信息                       |
+-----------------+--------------------------------+------------------------------------+
| Inode Map       | #Inocdes/BLOCK_SIZE            | 跟踪 inode 使用/未使用的情况       |
+-----------------+--------------------------------+------------------------------------+
| Zone Map        | #Data Zones/BLOCK_SIZE         | 跟踪 zones 使用/未使用的情况       |
+-----------------+--------------------------------+------------------------------------+
| Inode Table     | ((32/16) * #Inode)/BLOCK_SIZE  | 保存文件/设备的信息                |
+-----------------+--------------------------------+------------------------------------+
| Data Zones      | Big                            | 文件/目录的内容                    |
+-----------------+--------------------------------+------------------------------------+

Inodes
------

struct of incode::

    struct minix_inode {
        unsigned short i_mode;
        unsigned short i_uid;
        unsigned short i_size;
        unsigned short i_time;
        unsigned short i_gid;
        unsigned short i_nlinks;
        unsigned short i_zone[9];
    };

保存文件的重要信息，包括
  1. 文件权限
  2. 文件类型
  3. 用户
  4. 用户组
  5. 大小
  6. 修改时间
  7. links 的数量
  8. 位置
  9. 文件块的顺序


Directory
---------

struct of directory entry::

    struct minix_dir_entry {
        unsigned short inode;
        char name[0];
    };

关联文件名和一个 inode。


Super Block
-----------

struct of super block::

    struct minx_super_block {
        unsigned short s_nincodes;
        unsigned short s_nzones;
        unsigned short s_imap_blocks;
        unsigned short s_zmap_blocks;
        unsigned short s_firstdatazone;
        unsigned short s_s_log_zone_size;
        unsigned long s_max_size;
        unsigned short s_magic;
        unsigned short s_state;
    };


