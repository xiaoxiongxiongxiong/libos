#ifndef __OS_RBT_H__
#define __OS_RBT_H__

#include "libos.h"

typedef struct _os_rbt_t os_rbt_t;
typedef struct _os_rbt_node_t os_rbt_node_t;

OS_API_BEGIN

/*
* @brief  键值比较回调函数
* @param  key2
* @param  key1
* @return -1,0,1
*/
typedef int(*os_rbt_compare)(const void * key1, const void * key2);

/*
* @brief  key/val释放函数
* @param  data
*/
typedef void(*os_rbt_free)(void * data);

/*
* os_rbt_create
* @brief  创建红黑树
* @param  cmp  键值比较函数
* @param  key  key释放函数
* @param  val  val释放函数
* @return NULL/实例
*/
os_rbt_t * os_rbt_create(os_rbt_compare cmp, os_rbt_free key, os_rbt_free val);

/*
* os_rbt_destroy
* @brief  销毁红黑树
* @param  rbt  树实例
*/
void os_rbt_destroy(os_rbt_t ** rbt);

/*
* os_rbt_clear
* @brief  清空树
* @param  rbt  树实例
*/
void os_rbt_clear(os_rbt_t * rbt);

/*
* os_rbt_insert
* @brief  插入数据
* @param  rbt  树实例
* @param  key  要插入的键
* @param  val  要插入的值
* @return true/false
*/
bool os_rbt_insert(os_rbt_t * rbt, void * key, void * val);

/*
* os_rbt_erase
* @brief  删除某个节点
* @param  rbt  树实例
* @param  key  要删除的键
* @return true/false
*/
bool os_rbt_erase(os_rbt_t * rbt, const void * key);

/*
* os_rbt_find
* @brief  查找某个节点
* @param  rbt  树实例
* @param  key  要查找的键
* @return NULL/节点
*/
os_rbt_node_t * os_rbt_find(const os_rbt_t * rbt, const void * key);

/*
* os_rbt_key
* @brief  获取kek值
* @param  node  节点
* @return NULL/key
*/
void * os_rbt_key(const os_rbt_node_t * node);

/*
* os_rbt_val
* @brief  获取val值
* @param  node  节点
* @return NULL/val
*/
void * os_rbt_val(const os_rbt_node_t * node);

/*
* os_rbt_size
* @brief  获取树元素个数
* @param  rbt  树实例
* @return 元素个数
*/
size_t os_rbt_size(const os_rbt_t * rbt);

/*
* os_rbt_empty
* @brief  判断树是否为空
* @param  rbt  树实例
* @return true/false
*/
bool os_rbt_empty(const os_rbt_t * rbt);

OS_API_END

#endif
