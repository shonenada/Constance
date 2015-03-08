#ifndef __BLK_H
#define __BLK_H

#define HD_BUSY 0x80
#define HD_READ 0x40
#define HD_DF 0x20
#define HD_ERR 0x01

#define HD_STATUS 0x1F7    // Hd CMD

#define HD_CMD_PORT0 0x1F0

#define HD_CMD_READ 0x20
#define HD_CMD_WRITE 0x30

int hd_ready();
int hd_out();

#endif
