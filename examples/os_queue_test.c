#include "os_queue.h"

int main(void)
{
    os_queue_t * queue = os_queue_create(sizeof(int));
    if (NULL == queue) {
        printf("os_queue_create failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        os_queue_push(queue, &i);
    }

    while (!os_queue_empty(queue)) {
        os_queue_node_t * node = os_queue_front(queue);
        int * data = (int *)os_queue_getdata(node);
        printf("num: %d\n", *data);
        os_queue_pop(queue);
    }

    os_queue_destroy(&queue);

    return 0;
}
