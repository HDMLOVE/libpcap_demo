/******************************************
 * Filename : ringbuffer.c
 * Time     : 2021-08-18 06:11
 * Author   : 小骆
 * Dcription: 
*******************************************/

#include <stdlib.h>
#include <assert.h>

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

static uint8_t *ringbuf_nextp(const struct ringbuf_t *rb)
{
    assert( (p>= rb->buf) && (p < ringbuf_end(rb)) );
    return rb->buf + ((++p - rb->buf) % ringbuf_buffer_size(rb));
}