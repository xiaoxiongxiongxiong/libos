#ifndef _OS_LIST_H_
#define _OS_LIST_H_

#include "libos.h"

typedef struct _os_list_t os_list_t;
typedef struct _os_list_node_t os_list_node_t;

OS_API_BEGIN

/*
* os_list_create
* @brief  创建链表
* @param  node_size  节点大小
* @return 链表指针或者为NULL
*/
OS_API os_list_t * os_list_create(const size_t node_size);

/*
* os_list_destroy
* @brief  销毁链表
* @param  os_lst  指向链表指针的指针
*/
OS_API void os_list_destroy(os_list_t ** os_lst);

/*
* os_list_clear
* @brief  清空链表
* @param  os_lst  链表指针
*/
OS_API void os_list_clear(os_list_t * os_lst);

/*
* os_list_empty
* @brief  判断链表是否为空
* @param  os_lst  链表指针
* @return true--成功 false--失败
*/
OS_API bool os_list_empty(os_list_t * os_lst);

/*
* os_list_size
* @brief  获取链表长度
* @param  os_lst  链表指针
* @return 链表长度
*/
OS_API size_t os_list_size(os_list_t * os_lst);

/*
* os_list_add
* @brief  向链表中添加数据
* @param  os_lst   链表指针
* @param  data     数据
* @return true--成功 false--失败
*/
OS_API bool os_list_add(os_list_t * os_lst, void * data);

/*
* os_list_delete
* @brief  从链表中删除数据
* @param  os_lst   链表指针
* @param  os_node  要删除的节点指针
* @return true--成功 false--失败
*/
OS_API bool os_list_delete(os_list_t * os_lst, os_list_node_t * os_node);

/*
* os_list_head
* @brief  获取链表头节点
* @param  os_lst   链表指针
* @return 头节点指针或者NULL
*/
OS_API os_list_node_t * os_list_head(os_list_t * os_lst);

/*
* os_list_tail
* @brief  获取链表尾节点
* @param  os_lst   链表指针
* @return 尾节点指针或者NULL
*/
OS_API os_list_node_t * os_list_tail(os_list_t * os_lst);

/*
* os_list_next
* @brief  获取节点的下一个节点
* @param  os_node   节点指针
* @return 节点下一个节点指针或者NULL
*/
OS_API os_list_node_t * os_list_next(const os_list_node_t * os_node);

/*
* os_list_prev
* @brief  获取节点的上一个节点
* @param  os_node   节点指针
* @return 节点上一个节点指针或者NULL
*/
OS_API os_list_node_t * os_list_prev(const os_list_node_t * os_node);

/*
* os_list_getdata
* @brief  获取节点的数据指针
* @param  os_node   节点指针
* @return 节点数据指针或者NULL
*/
OS_API void * os_list_getdata(const os_list_node_t * node);

OS_API_END

#endif
