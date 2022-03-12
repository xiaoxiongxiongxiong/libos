#include "libos.h"
#include <string.h>
#include <stdarg.h>
#include <pthread.h>

static pthread_mutex_t g_err_mtx = PTHREAD_MUTEX_INITIALIZER;
// 错误信息
static char g_err_msg[OS_ERR_MAX_LEN] = { 0 };

void os_set_error(const char * fmt, ...);

const char * os_last_error()
{
    char * msg = NULL;
    pthread_mutex_lock(&g_err_mtx);
    msg = g_err_msg;
    pthread_mutex_unlock(&g_err_mtx);
    return msg;
}

void os_set_error(const char * fmt, ...)
{
    va_list vl;
    pthread_mutex_lock(&g_err_mtx);
    va_start(vl, fmt);
    vsnprintf(g_err_msg, OS_ERR_MAX_LEN, fmt, vl);
    va_end(vl);
    pthread_mutex_unlock(&g_err_mtx);
}
