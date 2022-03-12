#ifndef _OS_UTILS_H_
#define _OS_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __linux__
#define OS_UTILS_FILE_MAX 256   // 文件名最大长度
#define OS_UTILS_PATH_MAX 4096  // 文件路径最大长度
#define OS_UTILS_DISK_SEP '/'   // 路径分隔符
#else
#define OS_UTILS_FILE_MAX 260   // 文件名最大长度
#define OS_UTILS_PATH_MAX 260   // 文件路径最大长度
#define OS_UTILS_DISK_SEP '\\'  // 路径分隔符
#endif

// 系统时间
typedef struct _os_time_t
{
    int year;        // 年
    int month;       // 月
    int day;         // 日
    int hour;        // 时
    int minute;      // 分
    int second;      // 秒
    int millisecond; // 毫秒
} os_time_t;

// 获取当前工作路径
bool os_utils_work_path(char * path, size_t len);

// 创建文件夹
bool os_utils_create_directory(const char * path);

// 检查文件是否存在
bool os_utils_file_exist(const char * path);

// 获取文件名
bool os_utils_file_name(const char * path, char * file, size_t len);

// 获取当前线程id
long os_utils_tid();

// 获取当前时间
os_time_t * os_utils_time(os_time_t * tv);

#endif
