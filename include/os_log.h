#ifndef _OS_LOG_H_
#define _OS_LOG_H_

#include "libos.h"
#include <stdarg.h>
#include <inttypes.h>

typedef enum _LOG_MSG_LEVEL  // 日志级别
{
    LOG_LEVEL_DEBUG = 0,     // 调试信息
    LOG_LEVEL_INFO,          // 普通打印
    LOG_LEVEL_WARN,          // 警告，程序可以继续运行
    LOG_LEVEL_ERROR,         // 错误，系统函数错误
    LOG_LEVEL_FATAL          // 程序无法继续运行直接退出
} LOG_MSG_LEVEL;

OS_API_BEGIN

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
OS_API void log_msg_uninit(void);

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

/*
* log_msg_doit
* @brief  打印日志
* @param  level  日志级别
* @param  file   日志消息所在文件
* @param  line   日志消息所在行数
* @param  func   日志消息所在函数
* @param  fmt    日志格式
*/
OS_API int log_msg_doit(LOG_MSG_LEVEL level, const char * file, int line, const char * func, const char * fmt, va_list vl);

#define log_msg_debug(...) log_msg(LOG_LEVEL_DEBUG, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_msg_info(...) log_msg(LOG_LEVEL_INFO, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_msg_warn(...) log_msg(LOG_LEVEL_WARN, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_msg_error(...) log_msg(LOG_LEVEL_ERROR, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#define log_msg_fatal(...) log_msg(LOG_LEVEL_FATAL, __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

OS_API_END

#endif
