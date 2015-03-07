#ifndef __CONST_H
#define __CONST_H

/* Global type define */
typedef int size_t;
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;

/* Common variable */
#define KERN_CS 0x08    // kernel code segment
#define KERN_DS 0x10
#define USER_CS 0x18
#define USER_DS 0x20

#define NR_TASKS 64    // Max process numbers

#define NULL ((void*)0)

#endif
