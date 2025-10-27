#ifndef __OS_RBT_H__
#define __OS_RBT_H__

#include "libos.h"

typedef struct _os_rbt_t os_rbt_t;
typedef struct _os_rbt_node_t os_rbt_node_t;

OS_API_BEGIN

/*
* @brief  键值比较回调函数
* @param  data1
* @param  data2
* @param  size
* @return -1(data1<data2) 0(data1==data2) 1(data1>data2)
*/
typedef int(*os_rbt_compare)(const void * data1, const void * data2, size_t size);

/*
* os_rbt_create
* @brief  创建红黑树
* @param  elem_size  元素类型大小
* @param  cmp  键值比较函数
* @return NULL/实例
*/
OS_API os_rbt_t * os_rbt_create(size_t elem_size, os_rbt_compare cmp);

/*
* os_rbt_destroy
* @brief  销毁红黑树
* @param  rbt  树实例
*/
OS_API void os_rbt_destroy(os_rbt_t ** rbt);

/*
* os_rbt_clear
* @brief  清空树
* @param  rbt  树实例
*/
OS_API void os_rbt_clear(os_rbt_t * rbt);

/*
* os_rbt_insert
* @brief  插入数据
* @param  rbt  树实例
* @param  data 要插入的数据
* @return true/false
*/
OS_API bool os_rbt_insert(os_rbt_t * rbt, void * data);

/*
* os_rbt_erase
* @brief  删除某个节点
* @param  rbt  树实例
* @param  data 要删除的数据
* @return true/false
*/
OS_API bool os_rbt_erase(os_rbt_t * rbt, void * data);

/*
* os_rbt_find
* @brief  查找某个节点
* @param  rbt  树实例
* @param  data 要查找的数据
* @return NULL/节点
*/
OS_API os_rbt_node_t * os_rbt_find(const os_rbt_t * rbt, void * data);

/*
* os_rbt_data
* @brief  获取val值
* @param  node  节点
* @return NULL/val
*/
OS_API void * os_rbt_data(const os_rbt_node_t * node);

/*
* os_rbt_size
* @brief  获取树元素个数
* @param  rbt  树实例
* @return 元素个数
*/
OS_API size_t os_rbt_size(const os_rbt_t * rbt);

/*
* os_rbt_empty
* @brief  判断树是否为空
* @param  rbt  树实例
* @return true/false
*/
OS_API bool os_rbt_empty(const os_rbt_t * rbt);

OS_API_END

#endif
