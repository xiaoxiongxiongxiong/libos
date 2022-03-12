#include "os_log.h"
#include <stdlib.h>
#include <pthread.h>

static int g_cnt = 0;

void log_test_thr(void * args)
{
    pthread_mutex_t * mtx = (pthread_mutex_t *)args;
    while (g_cnt < 10000)
    {
        pthread_mutex_lock(mtx);
        g_cnt++;
        pthread_mutex_unlock(mtx);
        int index = g_cnt % 4;
        switch (index)
        {
        case 0:
            log_msg_debug("debug test: %d", g_cnt);
            break;
        case 1:
            log_msg_info("info test: %d", g_cnt);
            break;
        case 2:
            log_msg_warn("warn test: %d", g_cnt);
            break;
        case 3:
            log_msg_error("error test: %d", g_cnt);
            break;
        default:
            break;
        }
    }
}

int main(void)
{
    const char * log_path = "os_log_test.log";
    if (!log_msg_init(log_path, LOG_LEVEL_INFO))
    {
        printf("log_msg_init failed for %s\n", os_last_error());
        return 1;
    }
    //atexit(log_msg_uninit);

    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, NULL);

    pthread_t thr1;
    pthread_t thr2;
    pthread_create(&thr1, NULL, (void *)log_test_thr, &mtx);
    pthread_create(&thr2, NULL, (void *)log_test_thr, &mtx);
    pthread_join(thr1, NULL);
    pthread_join(thr2, NULL);

    log_msg_uninit();

    return 0;
}
