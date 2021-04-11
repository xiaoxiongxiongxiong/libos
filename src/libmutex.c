#include "libos.h"
#include <malloc.h>
#ifdef __linux__
#include <string.h>
#include <errno.h>
#include <pthread.h>
#else
#include <windows.h>
#include <process.h>
#endif

struct _os_mutex_t
{
#ifdef __linux__
    pthread_mutex_t section;
#else
    CRITICAL_SECTION section;
#endif
};

os_mutex_t * os_mutex_create()
{
    os_mutex_t * lck = (os_mutex_t *)malloc(sizeof(os_mutex_t));
    if (NULL == lck)
    {
        log_msg_error("malloc failed, err:%s", strerror(errno));
        return NULL;
    }

#ifdef __linux__
    if (0 != pthread_mutex_init(&lck->section, NULL))
    {
        log_msg_error("pthread_mutex_init failed, err:%s", strerror(errno));
        free(lck);
        return NULL;
    }
#else
    InitializeCriticalSection(&lck->section);
#endif

    return lck;
}

void os_mutex_destroy(os_mutex_t ** mtx)
{
    if (NULL == mtx || NULL == *mtx)
    {
        log_msg_warn("Input param is nullptr!");
        return;
    }

#ifdef __linux__
    pthread_mutex_destroy(&(*mtx)->section);
#else
    DeleteCriticalSection(&(*mtx)->section);
#endif
    free(*mtx);
    *mtx = NULL;
}

bool os_mutex_lock(os_mutex_t * mtx)
{
    if (NULL == mtx)
    {
        log_msg_warn("Input param is nullptr!");
        return false;
    }

#ifdef __linux__
    pthread_mutex_lock(&mtx->section);
#else
    EnterCriticalSection(&mtx->section);
#endif
    return true;
}

bool os_mutex_unlock(os_mutex_t * mtx)
{
    if (NULL == mtx)
    {
        log_msg_warn("Input param is nullptr!");
        return false;
    }

#ifdef __linux__
    pthread_mutex_unlock(&mtx->section);
#else
    LeaveCriticalSection(&mtx->section);
#endif
    return true;
}