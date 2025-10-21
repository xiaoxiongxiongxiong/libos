#ifndef _OS_SINGLE_LIST_H_
#define _OS_SINGLE_LIST_H_

#include "libos.h"

typedef struct _os_slist_t os_slist_t;
typedef struct _os_slist_node_t os_slist_node_t;
typedef bool (*os_slist_compare)(const void * data1, const void * data2);

OS_API_BEGIN

/*
* os_slist_create
* @brief  创建链表
* @param  elem_size  节点大小
* @return 链表指针或者为NULL
*/
OS_API os_slist_t * os_slist_create(const size_t elem_size);

/*
* os_slist_destroy
* @brief  销毁链表
* @param  lst  指向链表指针的指针
*/
OS_API void os_slist_destroy(os_slist_t ** lst);

/*
* os_slist_clear
* @brief  清空链表
* @param  lst  链表指针
*/
OS_API void os_slist_clear(os_slist_t * lst);

/*
* os_slist_empty
* @brief  判断链表是否为空
* @param  lst  链表指针
* @return true--成功 false--失败
*/
OS_API bool os_slist_empty(const os_slist_t * lst);

/*
* os_slist_size
* @brief  获取链表长度
* @param  lst  链表指针
* @return 链表长度
*/
OS_API size_t os_slist_size(const os_slist_t * lst);

/*
* os_slist_add
* @brief  向链表尾部添加数据
* @param  lst   链表指针
* @param  data  数据
* @return true--成功 false--失败
*/
OS_API bool os_slist_add(os_slist_t * lst, void * data);

/*
* os_slist_insert
* @brief  向链表指定位置插入数据
* @param  lst   链表
* @param  pos   位置
* @param  data  数据
* @return true--成功  false--失败
*/
OS_API bool os_slist_insert(os_slist_t * lst, size_t pos, void * data);

/*
* os_slist_delete
* @brief  从链表中删除数据
* @param  lst   链表指针
* @param  node  要删除的节点指针
* @return NULL 或者下一个节点
*/
OS_API os_slist_node_t * os_slist_delete(os_slist_t * lst, os_slist_node_t * node);

/*
* os_slist_delete_ex
* @brief  根据数据从链表中删除节点
* @param  lst   链表指针
* @param  data  数据
* @param  compare 数据删除条件
* @return true--成功 false--失败
*/
OS_API bool os_slist_delete_ex(os_slist_t * lst, void * data, os_slist_compare compare);

/*
* os_slist_head
* @brief  获取链表头节点
* @param  lst   链表指针
* @return 头节点指针或者NULL
*/
OS_API os_slist_node_t * os_slist_head(const os_slist_t * lst);

/*
* os_slist_next
* @brief  获取节点的下一个节点
* @param  node   节点指针
* @return 节点下一个节点指针或者NULL
*/
OS_API os_slist_node_t * os_slist_next(const os_slist_node_t * node);

/*
* os_slist_getdata
* @brief  获取节点的数据指针
* @param  node   节点指针
* @return 节点数据指针或者NULL
*/
OS_API void * os_slist_getdata(const os_slist_node_t * node);

/*
* os_slist_reverse_order
* @brief  逆序整个链表
* @param  lst
* @return 逆序后的链表
*/
OS_API os_slist_t * os_slist_reverse_order(os_slist_t * lst);

/*
* os_slist_merge
* @brief  合并链表
* @param  lst1  链表1
* @param  lst2  链表2
* @param  compare 自实现data1与data2比较函数
* @return 合并后的链表
*/
OS_API os_slist_t * os_slist_merge(os_slist_t * lst1, os_slist_t * lst2, os_slist_compare compare);

OS_API_END

#endif
