/******************************************
 * Filename : ringbuffer.c
 * Time     : 2021-08-18 06:11
 * Author   : 小骆
 * Dcription: 
*******************************************/

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <assert.h>
#include <unistd.h>
#include <sys/param.h>

#include "ringbuffer.h"

struct ringbuf_t{
    uint8_t *buf;
    uint8_t *head, *tail;
    size_t size;
};

/*
 * 创建一个指定大小的环形队列
 * */
ringbuf_t ringbuf_new(size_t capacity)
{
    ringbuf_t rb = malloc(sizeof(struct ringbuf_t));
    if (rb) {
        /* one byte is used for detecting the full condition. */
        /* 一个字节用于检测满的情况 */
        rb->size = capacity + 1;
        rb->buf = malloc(rb->size);
        if (rb->buf) {
            ringbuf_reset(rb);
        }
        else{
            free(rb);
            return 0;
        }
    }
    return rb;
}


/*
 * 获取环形队列大小
 * */
size_t ringbuf_buffer_size(const struct ringbuf_t *rb)
{
    return rb->size;
}

/*
 * 初始化ringbuf */
void ringbuf_reset(ringbuf_t rb)
{
    rb->head = rb->tail = rb->buf;
}

/*
 * 回收释放ringbuf资源
 * */
void ringbuf_free(ringbuf_t *rb)
{
    assert(rb && *rb);
    free((*rb)->buf);
    free(*rb);
    *rb = 0;
}

size_t ringbuf_capacity(const struct ringbuf_t *rb)
{
    ringbuf_buffer_size(rb) - 1;
}

/*
 * 跳转至结尾
 * */
static const uint8_t *ringbuf_end(const struct ringbuf_t *rb)
{
    return rb->buf + ringbuf_buffer_size(rb);
}

/*
 * ringbuf 空闲空间长度
 * */
size_t ringbuf_bytes_free(const struct ringbuf_t *rb)
{
    if(rb->head >= rb->tail)
        return ringbuf_capacity(rb) - (rb->head - rb->tail);
    else
        return rb->tail - rb->head - 1;
    return 0;
}

/*
 * ringbuf已使用长度
 * */
size_t ringbuf_bytes_uesd(const struct ringbuf_t *rb)
{
    return ringbuf_capacity(rb) - ringbuf_bytes_free(rb);
}

/*
 * 判断ringbuf是否满 */
int ringbuf_is_full(const struct ringbuf_t *rb)
{
    return ringbuf_bytes_free(rb) == 0;
}

/*
 * 判断ringbuf是否为空
 * */
int ringbuf_is_empty(const struct ringbuf_t *rb)
{
    return ringbuf_bytes_free(rb == ringbuf_capacity(rb);
}

const void *ringbuf_tail(const struct ringbuf_t *rb)
{
    return  rb->tail;
}

const void *ringbuf_head(const struct ringbuf_t *rb)
{
    return rb->head;
}


static uint8_t *ringbuf_nextp(ringbuf_t *rb, const uint8_t *p)
{
    assert( (p>= rb->buf) && (p < ringbuf_end(rb)) );
    return rb->buf + ((++p - rb->buf) % ringbuf_buffer_size(rb));
}

size_t ringbuf_findchr(const struct ringbuf_t *rb, int c, size_t offset)
{
    const uint8_t *bufend = ringbuf_end(rb);
    size_t bytes_used = ringbuf_bytes_uesd(rb);
    if(offset >=  bytes_used)
        return bytes_used;
    const uint8_t *start = rb->buf + (((rb->tail - rb->buf) + offset) % ringbuf_buffer_size(rb));
    assert(bufend > start);
    size_t n = MIN(bufend - start, bytes_used - offset);
    const uint8_t *found = memchr(start, c, n);
    if (found)
        return offset + (found - start);
    else
        return ringbuf_findchr(rb, c, offset + n);
}

size_t ringbuf_memset(ringbuf_t *dst, int c, size_t len);
{
    const uint8_t *bufend = ringbuf_end(dst);
    size_t nwritten = 0;
    size_t count = MIN(len, ringbuf_buffer_size(dst));
    int overflow = count > ringbuf_bytes_free(dst);

    while(nwritten != count)
    {
        assert(bufend > dst->head);
        size_t n = MIN(bufend - dst->head, count - nwritten);
        memset(dst->head, c, n);
        dst->head += n;

        /* wrap */
        if(dst->head == bufend)
            dst->head = dst->buf;
    }
    if(overflow){
        dst->tail = ringbuf_nextp(dst, dst->head);
        assert(ringbuf_is_full(dst));

    }
    return nwritten;
}
void ringbuf_memcpy_into(ringbuf_t dst, const void *src, size_t count)
{
    const uint8_t *u8src = src;
    const uint8_t *bufend = ringbuf_end(dst);
    int overflow = count > ringbuf_bytes_free(dst);
    size_t nread = 0;
}