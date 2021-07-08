#ifndef __LIBOS_H__
#define __LIBOS_H__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __linux__
#define OS_PATH_MAX_LEN 4096 // 路径最大长度
#else
#define OS_PATH_MAX_LEN 260  // 路径最大长度
#endif

typedef enum _LOG_MSG_LEVEL  // 日志级别
{
    LOG_LEVEL_INFO = 0,      // 普通打印
    LOG_LEVEL_WARN,          // 警告，程序可以继续运行
    LOG_LEVEL_ERROR,         // 错误，系统函数错误
    LOG_LEVEL_FATAL          // 程序无法继续运行直接退出
} LOG_MSG_LEVEL;

typedef struct _os_mutex_t os_mutex_t;
typedef struct _os_dlist_node_t os_dlist_node_t;
typedef struct _os_dlist_t os_dlist_t;
typedef struct _os_queue_node_t os_queue_node_t;
typedef struct _os_queue_t os_queue_t;
typedef struct _os_hash_node_t os_hash_node_t;
typedef struct _os_hash_table_t os_hash_table_t;

#ifdef __cplusplus
#define OS_API_BEGIN extern "C" {
#define OS_API_END }
#if defined(WIN32) || defined(_WIN32)
#if defined(OS_API_EXPORT)
#define OS_API __declspec(dllexport)
#else
#define OS_API __declspec(dllimport)
#endif
#else
#define OS_API
#endif
#else
#define OS_API_BEGIN
#define OS_API_END
#define OS_API
#endif

OS_API_BEGIN

/*
* os_mutex_create
* @brief  创建锁
* @return 锁指针或者NULL
*/
OS_API os_mutex_t * os_mutex_create();

/*
* os_mutex_destroy
* @brief  销毁锁
* @param  mtx 指向锁指针的指针
*/
OS_API void os_mutex_destroy(os_mutex_t ** mtx);

/*
* os_mutex_lock
* @brief  上锁
* @param  mtx  锁指针
* @return true--成功 false--失败
*/
OS_API bool os_mutex_lock(os_mutex_t * mtx);

/*
* os_mutex_unlock
* @brief  解锁
* @param  mtx  锁指针
* @return true--成功 false--失败
*/
OS_API bool os_mutex_unlock(os_mutex_t * mtx);

/*
* os_dlist_create
* @brief  创建链表
* @param  node_size  节点大小
* @return 链表指针或者为NULL
*/
OS_API os_dlist_t * os_dlist_create(const size_t node_size);

/*
* os_dlist_destroy
* @brief  销毁链表
* @param  os_lst  指向链表指针的指针
*/
OS_API void os_dlist_destroy(os_dlist_t ** os_lst);

/*
* os_dlist_clear
* @brief  清空链表
* @param  os_lst  链表指针
*/
OS_API void os_dlist_clear(os_dlist_t * os_lst);

/*
* os_dlist_empty
* @brief  判断链表是否为空
* @param  os_lst  链表指针
* @return true--成功 false--失败
*/
OS_API bool os_dlist_empty(const os_dlist_t * os_lst);

/*
* os_dlist_size
* @brief  获取链表长度
* @param  os_lst  链表指针
* @return 链表长度
*/
OS_API size_t os_dlist_size(const os_dlist_t * os_lst);

/*
* os_dlist_add
* @brief  向链表中添加数据
* @param  os_lst   链表指针
* @param  os_node  节点指针 可为空，不为空则添加到该节点之后
* @param  data     数据
* @return true--成功 false--失败
*/
OS_API bool os_dlist_add(os_dlist_t * os_lst, os_dlist_node_t * os_node, void * data);

/*
* os_dlist_delete
* @brief  从链表中删除数据
* @param  os_lst   链表指针
* @param  os_node  要删除的节点指针
* @return true--成功 false--失败
*/
OS_API bool os_dlist_delete(os_dlist_t * os_lst, os_dlist_node_t * os_node);

/*
* os_dlist_head
* @brief  获取链表头节点
* @param  os_lst   链表指针
* @return 头节点指针或者NULL
*/
OS_API os_dlist_node_t * os_dlist_head(const os_dlist_t * os_lst);

/*
* os_dlist_tail
* @brief  获取链表尾节点
* @param  os_lst   链表指针
* @return 尾节点指针或者NULL
*/
OS_API os_dlist_node_t * os_dlist_tail(const os_dlist_t * os_lst);

/*
* os_dlist_next
* @brief  获取节点的下一个节点
* @param  os_node   节点指针
* @return 节点下一个节点指针或者NULL
*/
OS_API os_dlist_node_t * os_dlist_next(const os_dlist_node_t * os_node);

/*
* os_dlist_prev
* @brief  获取节点的上一个节点
* @param  os_node   节点指针
* @return 节点上一个节点指针或者NULL
*/
OS_API os_dlist_node_t * os_dlist_prev(const os_dlist_node_t * os_node);

/*
* os_dlist_getdata
* @brief  获取节点的数据指针
* @param  os_node   节点指针
* @return 节点数据指针或者NULL
*/
OS_API void * os_dlist_getdata(const os_dlist_node_t * node);

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
OS_API size_t os_queue_size(const os_queue_t * os_queue);

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

/*
* log_msg_init
* @brief  日志初始化
* @param  file    日志文件
* @param  level   写入文件的日志级别
* @return true--成功 false--失败
*/
OS_API bool log_msg_init(const char * file, LOG_MSG_LEVEL level);

/*
* log_msg_uninit
* @brief  日志销毁
*/
OS_API void log_msg_uninit();

/*
* log_msg
* @brief  打印日志
* @param  level  日志级别
* @param  file   日志消息所在文件
* @param  line   日志消息所在行数
* @param  func   日志消息所在函数
* @param  fmt    日志格式
*/
OS_API int log_msg(LOG_MSG_LEVEL level, const char * file, int line, const char * func, const char * fmt, ...);

#define log_msg_info(...) log_msg(LOG_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_msg_warn(...) log_msg(LOG_LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_msg_error(...) log_msg(LOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_msg_fatal(...) log_msg(LOG_LEVEL_FATAL, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

OS_API_END

#endif
