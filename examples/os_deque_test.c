#include "os_deque.h"

int main(int argc, char * argv[])
{
    os_deque_t * q = os_deque_create(sizeof(int));
    if (NULL == q) {
        fprintf(stderr, "os_deque_create failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        os_deque_push_back(q, &i);
    }

    while (!os_deque_empty(q)) {
        os_deque_node_t * node = os_deque_front(q);
        int * data = (int *)os_deque_getdata(node);
        printf("num: %d\n", *data);
        os_deque_pop_front(q);
    }

    for (int i = 0; i < 10; i++) {
        os_deque_push_front(q, &i);
    }

    while (!os_deque_empty(q)) {
        os_deque_node_t * node = os_deque_back(q);
        int * data = (int *)os_deque_getdata(node);
        printf("num: %d\n", *data);
        os_deque_pop_back(q);
    }

    os_deque_destroy(&q);

    return 0;
}
