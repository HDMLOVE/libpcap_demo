#include "ring_buffer.h"

// 初始化缓冲区
uint8_t ring_buffer_init(ring_bffer *ring_buffer_handle, uint8_t *buffer_addr, uint32_t buffer_size)
{
	ring_buffer_handle->head = 0;
	ring_buffer_handle->tail = 0;
	ring_buffer_handle->lenght = 0;
	ring_buffer_handle->array_addr = buffer_addr;
	ring_buffer_handle->max_lenght = buffer_size;

	if(ring_buffer_handle->max_lenght < 2)
		return RING_BUFFER_ERROR;
	else
		return RING_BUFFER_SUCCESS;
}

// 删除缓冲区中的数据
uint8_t ring_buffer_delete(ring_buffer *ring_buffer_handle, uint32_t lenght)
{
	if(ring_buffer_handle->lenght < lenght)
		return RING_BUFFER_ERROR;
	if((ring_buffer_handle->head + lenght) >= ring_buffer_handle->max_lenght)
		ring_buffer_handle->head =  lenght - (ring_buffer_handle->max_lenght - ring_buffer_handle->head)
	else
		ring_buffer_handle->head += lenght;
	ring_buffer_handle->lenght -= lenght;

	return RING_BUFFER_SUCCESS;
}

uint8_t ring_buffer_write_byte(ring_buffer *ring_buffer_handle, uint32_t lenght)
{
	if(ring_buffer_handle->max_lenght - ring_buffer_
}

