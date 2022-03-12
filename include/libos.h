#ifndef _LIBOS_H_
#define _LIBOS_H_

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
#define OS_API_BEGIN extern "C" {
#define OS_API_END }
#else
#define OS_API_BEGIN
#define OS_API_END
#endif

#if defined(WIN32) || defined(_WIN32)
#if defined(OS_API_EXPORT)
#define OS_API __declspec(dllexport)
#else
#define OS_API __declspec(dllimport)
#endif
#else
#define OS_API
#endif

#define OS_ERR_MAX_LEN 260  // 错误信息最大长度

OS_API_BEGIN

/*
* os_last_error
* @brief 获取最后一次错误信息
* @return 错误信息
*/
OS_API const char * os_last_error();

OS_API_END

#endif
