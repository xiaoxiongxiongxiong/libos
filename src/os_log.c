#include "os_log.h"
#include "os_queue.h"
#include "os_utils.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#define OS_LOG_DATE_MAX 32       // 日志时间长度 
#define OS_LOG_MSG_MAX  3072     // 日志信息最大长度
#define OS_LOG_LINE_MAX 4096     // 日志每行打印的最大字节数

typedef struct _os_log_t
{
    int line;                      // 日志所在行数
    long tid;                      // 线程id
    LOG_MSG_LEVEL level;           // 日志级别
    char date[OS_LOG_DATE_MAX];    // 打印日志时间
    char file[OS_UTILS_FILE_MAX];  // 日志消息所在文件
    char func[OS_UTILS_FILE_MAX];  // 日志消息所在函数
    char msg[OS_LOG_MSG_MAX];      // 日志消息
} os_log_t;

extern void os_set_error(const char * fmt, ...);

// 是否已经初始化
static bool g_is_init = false;
// 写入文件的日志级别
static LOG_MSG_LEVEL g_log_level = LOG_LEVEL_INFO;
// 文件句柄
static FILE * g_log_fp = NULL;
// 文件路径
static char g_log_path[OS_UTILS_PATH_MAX] = { 0 };
// 日志消息存放队列
static os_queue_t * g_log_queue = NULL;
// 日志级别字符串
static const char * g_log_level_str[] = { "DEBUG", "INFO", "WARN", "ERROR", "FATAL" };

static pthread_t g_log_thr;
static pthread_mutex_t g_log_mtx;
static pthread_cond_t g_log_cond;
static void os_log_msg_thr(void * args);

// 日志初始化
static bool os_log_startup(const char * file);
// 日志销毁
static void os_log_cleanup();

// 将日志写入文件
static void log_msg_write_file(const os_log_t * log_node);

bool log_msg_init(const char * file, const LOG_MSG_LEVEL level)
{
    if (g_is_init)
    {
        os_set_error("log has already initialized");
        return false;
    }

    assert(level <= LOG_LEVEL_FATAL && level >= LOG_LEVEL_DEBUG);

    if (level<LOG_LEVEL_INFO || level>LOG_LEVEL_FATAL)
    {
        os_set_error("log level is out of range");
        return false;
    }
    g_log_level = level;
    g_is_init = true;

    if (NULL == file || 0 == strcmp(file, ""))
    {
        // 只打印不写入文件
        return true;
    }

    return os_log_startup(file);
}

void log_msg_uninit(void)
{
    os_log_cleanup();
}

int log_msg(const LOG_MSG_LEVEL level, const char * file, const int line, const char * func, const char * fmt, ...)
{
    va_list vl;

    va_start(vl, fmt);
    int len = log_msg_doit(level, file, line, func, fmt, vl);
    va_end(vl);

    return len;
}

bool os_log_startup(const char * file)
{
    if (!os_utils_create_directory(file))
    {
        return false;
    }

    memcpy(g_log_path, file, OS_UTILS_PATH_MAX);

    g_log_fp = fopen(g_log_path, "a");
    if (NULL == g_log_fp)
    {
        const int code = errno;
        os_set_error("fopen failed for %s", strerror(code));
        return false;
    }

    // 创建通用消息队列
    g_log_queue = os_queue_create(sizeof(os_log_t));
    if (NULL == g_log_queue)
    {
        fclose(g_log_fp);
        g_log_fp = NULL;
        return false;
    }

    // 启动日志写入线程
    int ret = pthread_mutex_init(&g_log_mtx, NULL);
    if (ret < 0)
    {
        os_set_error("pthread_mutex_init failed with %d", ret);
        os_log_cleanup();
        return false;
    }
    ret = pthread_cond_init(&g_log_cond, NULL);
    if (ret < 0)
    {
        os_set_error("pthread_cond_init failed with %d", ret);
        pthread_mutex_destroy(&g_log_mtx);
        os_log_cleanup();
        return false;
    }

    g_is_init = true;
    ret = pthread_create(&g_log_thr, NULL, (void *)os_log_msg_thr, NULL);
    if (ret < 0)
    {
        os_set_error("pthread_create failed with %d", ret);
        g_is_init = false;
        pthread_cond_destroy(&g_log_cond);
        pthread_mutex_destroy(&g_log_mtx);
        os_log_cleanup();
        return false;
    }

    return true;
}

void os_log_cleanup()
{
    if (g_is_init)
    {
        g_is_init = false;
        pthread_cond_signal(&g_log_cond);
        pthread_join(g_log_thr, NULL);
        pthread_mutex_destroy(&g_log_mtx);
        pthread_cond_destroy(&g_log_cond);
    }

    if (NULL != g_log_fp)
    {
        fclose(g_log_fp);
        g_log_fp = NULL;
    }

    if (NULL != g_log_queue)
    {
        os_queue_destroy(&g_log_queue);
        g_log_queue = NULL;
    }

    memset(g_log_path, 0, OS_UTILS_PATH_MAX);
}

int log_msg_doit(LOG_MSG_LEVEL level, const char * file, const int line, const char * func, const char * fmt, va_list vl)
{
    os_time_t tv = { 0 };
    os_utils_time(&tv);

    os_log_t node = { 0 };
    // 级别修正
    if (level >= LOG_LEVEL_INFO && level <= LOG_LEVEL_FATAL)
        node.level = level;
    else
        node.level = LOG_LEVEL_FATAL;
    node.line = line;
    snprintf(node.date, OS_LOG_DATE_MAX, "%04d/%02d/%02d %02d:%02d:%02d.%03d",
             tv.year, tv.month, tv.day, tv.hour, tv.minute, tv.second, tv.millisecond);

    node.tid = os_utils_tid();
    if (NULL != file)
        os_utils_file_name(file, node.file, OS_UTILS_FILE_MAX);

    // 所在函数
    if (func)
        strncpy(node.func, func, OS_UTILS_FILE_MAX - 1);

    // 格式化日志
    int len = vsnprintf(node.msg, sizeof(node.msg), fmt, vl);
    os_queue_push(g_log_queue, &node);
    pthread_cond_signal(&g_log_cond);

    // 根据级别判定是否退出
    if (level == LOG_LEVEL_FATAL)
    {
        log_msg_uninit();  //日志反初始化并杀掉当前进程
        exit(EXIT_FAILURE);
    }

    return len;
}

void log_msg_write_file(const os_log_t * log_node)
{
    char log_buf[OS_LOG_LINE_MAX] = { 0 };
    snprintf(log_buf, OS_LOG_LINE_MAX, "[%s][%s][%ld][%s:%d][%s] %s\n",
             log_node->date, g_log_level_str[log_node->level], log_node->tid, log_node->file,
             log_node->line, log_node->func, log_node->msg);

#ifndef NDEBUG
    fflush(stdout);
    fputs(log_buf, stderr);
    fflush(stderr);
#endif

    if (log_node->level >= g_log_level)
    {
        if (!os_utils_file_exist(g_log_path))
        {
            g_log_fp = freopen(g_log_path, "a", g_log_fp);
        }

        if (NULL != g_log_fp)
        {
            if (EOF == fputs(log_buf, g_log_fp))  // 写入文件失败时打印到控制台
            {
                fflush(stdout);
                fputs(log_buf, stderr);
                fflush(stderr);
            }
            else
                fflush(g_log_fp);
        }
    }
}

void os_log_msg_thr(void * args)
{
    log_msg_info("log work tid: %ld.", os_utils_tid());
    while (g_is_init)
    {
        pthread_mutex_lock(&g_log_mtx);
        while (os_queue_empty(g_log_queue) && g_is_init)
        {
            pthread_cond_wait(&g_log_cond, &g_log_mtx);
        }

        // 唤醒一次清空一次队列
        while (!os_queue_empty(g_log_queue))
        {
            os_queue_node_t * node = os_queue_front(g_log_queue);
            os_log_t * data = (os_log_t *)os_queue_getdata(node);
            log_msg_write_file(data);
            os_queue_pop(g_log_queue);
        }
        pthread_mutex_unlock(&g_log_mtx);
    }

    // 将未处理的日志信息全部处理掉
    while (!os_queue_empty(g_log_queue))
    {
        os_queue_node_t * node = os_queue_front(g_log_queue);
        os_log_t * data = (os_log_t *)os_queue_getdata(node);
        log_msg_write_file(data);
        os_queue_pop(g_log_queue);
    }
}
