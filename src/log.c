#include "libos.h"
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>
#ifdef __linux__
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/time.h>
#include <sys/syscall.h>
#else
#include <windows.h>
#include <io.h>
#endif

#define OS_LOG_DATE_MAX_LEN 64       // 日志时间长度 
#define OS_LOG_FILE_MAX_LEN 256      // 文件名最大长度为255
#define OS_LOG_MSG_MAX_BUFF 3072     // 日志信息最大长度
#define OS_LOG_LINE_MAX_LEN 4096     // 日志每行打印的最大字节数

typedef struct _os_log_t
{
    int line;                        // 日志所在行数
    long thr_id;                     // 线程id
    LOG_MSG_LEVEL level;             // 日志级别
    char date[OS_LOG_DATE_MAX_LEN];  // 打印日志时间
    char file[OS_LOG_FILE_MAX_LEN];  // 日志消息所在文件
    char func[OS_LOG_FILE_MAX_LEN];  // 日志消息所在函数
    char msg[OS_LOG_MSG_MAX_BUFF];   // 日志消息
} os_log_t;

// 是否已经初始化
static bool g_is_init = false;
// 写入文件的日志级别
static LOG_MSG_LEVEL g_log_level = LOG_LEVEL_INFO;
// 文件句柄
static FILE * g_log_fp = NULL;
// 文件路径
static char g_log_path[OS_PATH_MAX_LEN] = { 0 };
// 日志级别字符串
static const char * g_log_str[] = { "INFO", "WARN", "ERROR", "FATAL" };

// 处理日志函数
static int log_msg_doit(LOG_MSG_LEVEL level, const char * file, const int line, const char * func, const char * fmt, va_list vl);

bool log_msg_init(const char * file, const LOG_MSG_LEVEL level)
{
    if (g_is_init)
    {
        log_msg_warn("log has already initialized!");
        return false;
    }

    assert(level <= LOG_LEVEL_FATAL && level >= LOG_LEVEL_INFO);

    if (level<LOG_LEVEL_INFO || level>LOG_LEVEL_FATAL)
    {
        fputs("log level is out of range!\n", stderr);
        return false;
    }
    g_log_level = level;
    g_is_init = true;

    if (file)
    {
        strncpy(g_log_path, file, OS_PATH_MAX_LEN - 1);
        // TODO 获取绝对路径
        g_log_fp = fopen(g_log_path, "a+");
        if (NULL == g_log_fp)
        {
            log_msg_error("fopen %s failed, err:%s", g_log_path, strerror(errno));
            g_is_init = false;
            return false;
        }
    }

    return true;
}

void log_msg_uninit()
{
    if (!g_is_init)
        return;

    if (g_log_fp)
    {
        fclose(g_log_fp);
        g_log_fp = NULL;
    }

    g_is_init = false;
}

int log_msg(const LOG_MSG_LEVEL level, const char * file, const int line, const char * func, const char * fmt, ...)
{
    va_list vl;

    va_start(vl, fmt);
    int len = log_msg_doit(level, file, line, func, fmt, vl);
    va_end(vl);

    return len;
}

int log_msg_doit(LOG_MSG_LEVEL level, const char * file, const int line, const char * func, const char * fmt, va_list vl)
{
    os_log_t node = { 0 };

#ifdef __linux__
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm * sys_tv = localtime(&tv.tv_sec);
#else
    SYSTEMTIME sys_tv;
    GetLocalTime(&sys_tv);
#endif

    // 级别修正
    if (level >= LOG_LEVEL_INFO && level <= LOG_LEVEL_FATAL)
        node.level = level;
    else
        node.level = LOG_LEVEL_FATAL;
    node.line = line;

#if __linux__
    if (file)
    {
        char * file_name = basename((char *)file);
        strncpy(node.file, file_name, OS_LOG_FILE_MAX_LEN);
    }
    snprintf(node.date, OS_LOG_DATE_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d.%03d", sys_tv->tm_year + 1900, sys_tv->tm_mon + 1,
             sys_tv->tm_mday, sys_tv->tm_hour, sys_tv->tm_min, sys_tv->tm_sec, tv.tv_usec / 1000);
    node.thr_id = syscall(SYS_gettid);
#else
    // 打印日志时间
    snprintf(node.date, OS_LOG_DATE_MAX_LEN, "%04d/%02d/%02d %02d:%02d:%02d.%03d",
             sys_tv.wYear, sys_tv.wMonth, sys_tv.wDay, sys_tv.wHour,
             sys_tv.wMinute, sys_tv.wSecond, sys_tv.wMilliseconds);
    node.thr_id = (long)GetCurrentThreadId();
    if (file)
    {
        char tmp[OS_LOG_FILE_MAX_LEN] = { 0 };
        char ext[OS_LOG_DATE_MAX_LEN] = { 0 };
        _splitpath(file, NULL, NULL, tmp, ext);
        snprintf(node.file, OS_LOG_FILE_MAX_LEN, "%s%s", tmp, ext);
    }
#endif

    // 所在函数
    if (func)
        strncpy(node.func, func, OS_LOG_FILE_MAX_LEN - 1);

    // 格式化日志
    int len = vsnprintf(node.msg, sizeof(node.msg), fmt, vl);
    char log_buf[OS_LOG_LINE_MAX_LEN] = { 0 };
    snprintf(log_buf, OS_LOG_LINE_MAX_LEN, "[%s][%s][%ld][%s:%d][%s] %s\n",
             node.date, g_log_str[node.level], node.thr_id, node.file,
             node.line, node.func, node.msg);

    fflush(stdout);
    fputs(log_buf, stderr);
    fflush(stderr);

    if (node.level >= g_log_level && g_log_fp)
    {
        bool exist = false;
#ifdef __linux__
        exist = (0 == access(g_log_path, F_OK));
#else
        exist = (0 == _access(g_log_path, 0));
#endif
        if (!exist)
        {
            g_log_fp = freopen(g_log_path, "a+", g_log_fp);
        }

        if (g_log_fp)
        {
            fputs(log_buf, g_log_fp);
            fflush(g_log_fp);
        }
    }

    // 根据级别判定是否退出
    if (level == LOG_LEVEL_FATAL)
    {
        log_msg_uninit();  //日志反初始化并杀掉当前进程
        exit(EXIT_FAILURE);
    }
    return len;
}
