#ifndef __LIBOS_H__
#define __LIBOS_H__

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

#endif
