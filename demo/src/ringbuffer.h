/******************************************
 * Filename : ringbuffer.h
 * Time     : 2021-08-18 06:11
 * Author   : 小骆
 * Dcription: 
*******************************************/

#ifndef LIBPCAP_DEMO_RINGBUFFER_H
#define LIBPCAP_DEMO_RINGBUFFER_H

#include <stddef.h>

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
typedef  unsigned long  uint64_t;

typedef struct ringbuf_t *ringbuf_t;

ringbuf_t ringbuf_new(size_t capacity);

#endif
