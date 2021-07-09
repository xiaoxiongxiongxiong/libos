#include "libos.h"
#include <stdlib.h>

int main(int argc, char * argv[])
{
    log_msg_init(NULL, LOG_LEVEL_INFO);
    atexit(log_msg_uninit);

    auto ht = os_hash_table_create();
    if (nullptr == ht)
    {
        log_msg_warn("os_hash_table_create failed");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 10; i++)
    {
        if (!os_hash_table_add(ht, &i, sizeof(int), &i, sizeof(int)))
        {
            log_msg_warn("os_hash_table_add failed");
            break;
        }
    }

    for (int i = 0; i < 10; i++)
    {
        auto hn = os_hash_table_find(ht, &i, sizeof(int));
        if (nullptr != hn)
        {
            int value = *(int *)os_hash_table_get_value(hn);
            log_msg_info("value:%d", value);
        }
    }

    auto head = os_hash_table_head(ht);
    while (nullptr != head)
    {
        int value = *(int *)os_hash_table_get_value(head);
        log_msg_info("value:%d", value);
        head = os_hash_table_next(head);
    }

    return EXIT_SUCCESS;
}
