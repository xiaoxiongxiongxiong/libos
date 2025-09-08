#ifndef __OS_DOUBLE_LIST_H__
#define __OS_DOUBLE_LIST_H__

#include "libos.h"

typedef struct _os_dlist_t os_dlist_t;
typedef struct _os_dlist_node_t os_dlist_node_t;

OS_API_BEGIN

/*
* os_dlist_create
* @brief  创建链表
* @param  elem_size  节点大小
* @return 链表指针或者为NULL
*/
OS_API os_dlist_t * os_dlist_create(size_t elem_size);

/*
* os_dlist_destroy
* @brief  销毁链表
* @param  lst  指向链表指针的指针
*/
OS_API void os_dlist_destroy(os_dlist_t ** lst);

/*
* os_dlist_clear
* @brief  清空链表
* @param  lst  链表指针
*/
OS_API void os_dlist_clear(os_dlist_t * lst);

/*
* os_dlist_empty
* @brief  判断链表是否为空
* @param  lst  链表指针
* @return true--成功 false--失败
*/
OS_API bool os_dlist_empty(os_dlist_t * lst);

/*
* os_dlist_size
* @brief  获取链表长度
* @param  lst  链表指针
* @return 链表长度
*/
OS_API size_t os_dlist_size(os_dlist_t * lst);

/*
* os_dlist_add
* @brief  向链表尾部添加数据
* @param  lst   链表指针
* @param  data  数据
* @return true--成功 false--失败
*/
OS_API bool os_dlist_add(os_dlist_t * lst, void * data);

/*
* os_dlist_insert
* @brief  向链表指定位置插入数据
* @param  lst   链表
* @param  pos   位置
* @param  data  数据
* @return true--成功  false--失败
*/
OS_API bool os_dlist_insert(os_dlist_t * lst, size_t pos, void * data);

/*
* os_dlist_delete
* @brief  从链表中删除数据
* @param  lst   链表指针
* @param  node  要删除的节点指针
* @return NULL 或者下一个节点
*/
OS_API os_dlist_node_t * os_dlist_delete(os_dlist_t * lst, os_dlist_node_t * node);

/*
* os_dlist_delete_ex
* @brief  根据数据从链表中删除节点
* @param  lst   链表指针
* @param  data  数据
* @return true--成功 false--失败
*/
OS_API bool os_dlist_delete_ex(os_dlist_t * lst, void * data);

/*
* os_dlist_head
* @brief  获取链表头节点
* @param  lst   链表指针
* @return 头节点指针或者NULL
*/
OS_API os_dlist_node_t * os_dlist_head(os_dlist_t * lst);

/*
* os_dlist_tail
* @brief  获取链表尾节点
* @param  lst   链表指针
* @return 尾节点指针或者NULL
*/
OS_API os_dlist_node_t * os_dlist_tail(os_dlist_t * lst);

/*
* os_dlist_next
* @brief  获取节点的下一个节点
* @param  node   节点指针
* @return 节点下一个节点指针或者NULL
*/
OS_API os_dlist_node_t * os_dlist_next(const os_dlist_node_t * node);

/*
* os_dlist_prev
* @brief  获取节点的上一个节点
* @param  node   节点指针
* @return 节点上一个节点指针或者NULL
*/
OS_API os_dlist_node_t * os_dlist_prev(const os_dlist_node_t * node);

/*
* os_dlist_getdata
* @brief  获取节点的数据指针
* @param  node   节点指针
* @return 节点数据指针或者NULL
*/
OS_API void * os_dlist_getdata(const os_dlist_node_t * node);

OS_API_END

#endif
