#ifndef __OS_DEQUE_H__
#define __OS_DEQUE_H__

#include "libos.h"

typedef struct _os_deque_t os_deque_t;
typedef struct _os_deque_node_t os_deque_node_t;

OS_API_BEGIN

/*
* os_deque_create
* @brief  创建队列
* @param  elem_size  元素大小
* @return 队列指针或者为NULL
*/
OS_API os_deque_t * os_deque_create(size_t elem_size);

/*
* os_deque_destroy
* @brief  销毁队列
* @param  q 指向队列指针的指针
*/
OS_API void os_deque_destroy(os_deque_t ** q);

/*
* os_deque_clear
* @brief  清空队列
* @param  q 队列指针
*/
OS_API void os_deque_clear(os_deque_t * q);

/*
* os_deque_empty
* @brief  判断队列是否为空
* @param  q  队列指针
* @return true--空 false--非空
*/
OS_API bool os_deque_empty(os_deque_t * q);

/*
* os_deque_size
* @brief  获取队列长度
* @param  q  队列指针
* @return 队列长度
*/
OS_API size_t os_deque_size(os_deque_t * q);

/*
* os_deque_push_back
* @brief  队列尾插入数据
* @param  q     队列指针
* @param  data  数据指针
* @return true--成功 false--失败
*/
OS_API bool os_deque_push_back(os_deque_t * q, void * data);

/*
* os_deque_push_front
* @brief  队列头插入数据
* @param  q  队列指针
* @param  data  数据指针
* @return true--成功 false--失败
*/
OS_API bool os_deque_push_front(os_deque_t * q, void * data);

/*
* os_deque_pop
* @brief  删除队列头
* @param  q  队列指针
* @return true--成功 false--失败
*/
OS_API bool os_deque_pop_front(os_deque_t * q);

/*
* os_deque_pop_back
* @brief  删除队列尾
* @param  q  队列指针
* @return true--成功 false--失败
*/
OS_API bool os_deque_pop_back(os_deque_t * q);

/*
* os_deque_front
* @brief  获取队列头
* @param  q  队列指针
* @return 头结点指针或者NULL
*/
OS_API os_deque_node_t * os_deque_front(os_deque_t * q);

/*
* os_deque_back
* @brief  获取队列头
* @param  q  队列指针
* @return 尾结点指针或者NULL
*/
OS_API os_deque_node_t * os_deque_back(os_deque_t * q);

/*
* os_deque_getdata
* @brief  获取节点数据
* @param  node  节点指针
* @return 数据指针
*/
OS_API void * os_deque_getdata(const os_deque_node_t * node);

OS_API_END

#endif
