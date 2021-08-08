#ifndef __RING_BUFFER_H__
#define __RING_BUFFER_H__


#ifdef __cplusplus
extern "C"{
#endif

#define RING_BUFFER_SUCCESS 0x01
#define RING_BUFFER_ERROR   0x00

//定义环形缓冲区结构体
typedef struct {
	uint32_t head;			// 操作头指针
	uint32_t tail;			// 操作尾指针
	uint32_t lenght;		// 已经存储的数据长度
	uint32_t max_lenght;	// 缓冲区最大存储数据量
	uint8_t  *array_addr;	// 缓冲区存储数组基地址
}ring_buffer;


// 自定义操作函数
uint8_t ring_buffer_init(ring_buffer *ring_buffer_handle, );


#ifdef __cplusplus
}
#endif

#endif
