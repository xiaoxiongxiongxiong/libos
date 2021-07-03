#include "libos.h"
#include <stdlib.h>

typedef struct _test_node_t
{
    int data;
} test_node_t;

int main(int argc, char * argv[])
{
    log_msg_init(NULL, LOG_LEVEL_INFO);
    atexit(log_msg_uninit);

    auto lst = os_dlist_create(sizeof(int));
    if (nullptr == lst)
    {
        log_msg_warn("os_dlist_create failed!");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < 10; i++)
    {
        test_node_t node = { 0 };
        node.data = i;
        os_dlist_add(lst, nullptr, &node);
    }

    auto head = os_dlist_head(lst);
    while (nullptr != head)
    {
        auto node = (test_node_t *)os_dlist_getdata(head);
        log_msg_info("%d", node->data);
        head = os_dlist_next(head);
    }

    return EXIT_SUCCESS;
}
