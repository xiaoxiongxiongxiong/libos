#ifndef __OS_QUEUE_H__
#define __OS_QUEUE_H__

#include "libos.h"
#include <stdint.h>

typedef struct _os_queue_t os_queue_t;
typedef struct _os_queue_node_t os_queue_node_t;

OS_API_BEGIN

/*
* os_queue_create
* @brief  创建队列
* @param  node_size  节点大小
* @return 队列指针或者为NULL
*/
OS_API os_queue_t * os_queue_create(uint32_t node_size);

/*
* os_queue_destroy
* @brief  销毁队列
* @param  os_queue  指向队列指针的指针
*/
OS_API void os_queue_destroy(os_queue_t ** os_queue);

/*
* os_queue_clear
* @brief  清空队列
* @param  os_queue 队列指针
*/
OS_API void os_queue_clear(os_queue_t * os_queue);

/*
* os_queue_empty
* @brief  判断队列是否为空
* @param  os_queue  队列指针
* @return true--成功 false--失败
*/
OS_API bool os_queue_empty(os_queue_t * os_queue);

/*
* os_queue_size
* @brief  获取队列长度
* @param  os_queue  队列指针
* @return 队列长度
*/
OS_API size_t os_queue_size(os_queue_t * os_queue);

/*
* os_queue_push
* @brief  插入数据
* @param  os_queue  队列指针
* @param  data  数据指针
* @return true--成功 false--失败
*/
OS_API bool os_queue_push(os_queue_t * os_queue, void * data);

/*
* os_queue_pop
* @brief  删除队列头
* @param  os_queue  队列指针
* @return true--成功 false--失败
*/
OS_API bool os_queue_pop(os_queue_t * os_queue);

/*
* os_queue_front
* @brief  获取队列头
* @param  os_queue  队列指针
* @return 头结点指针或者NULL
*/
OS_API os_queue_node_t * os_queue_front(os_queue_t * os_queue);

/*
* os_queue_getdata
* @brief  获取节点数据
* @param  node    节点指针
* @return 数据指针
*/
OS_API void * os_queue_getdata(const os_queue_node_t * node);

OS_API_END

#endif
