#include "libos.h"
#include <stdlib.h>

int main(int argc, char * argv[])
{
    log_msg_init(NULL, LOG_LEVEL_INFO);
    atexit(log_msg_uninit);

    os_queue_t * os_queue = os_queue_create(sizeof(int));
    if (nullptr == os_queue)
    {
        log_msg_warn("os_queue_create failed!");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 10; i++)
    {
        os_queue_push(os_queue, (void *)&i);
    }

    log_msg_info("queue size:%ld", os_queue_size(os_queue));
    
    for (int i = 0; i < 10; i++)
    {
        os_queue_node_t * node = os_queue_front(os_queue);
        if (nullptr != node)
        {
            int * node_data = (int *)os_queue_getdata(node);
            log_msg_info("%d", *node_data);
            os_queue_pop(os_queue);
        }
    }

    log_msg_info("queue size:%ld", os_queue_size(os_queue));

    for (int i = 0; i < 10; i++)
        log_msg_info("libos test");

    return EXIT_SUCCESS;
}