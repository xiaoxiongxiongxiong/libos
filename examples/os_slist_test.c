#include "os_slist.h"

int main(int argc, char * argv[])
{
    os_slist_t * lst = os_slist_create(sizeof(int));
    if (NULL == lst) {
        fprintf(stderr, "os_slist_create failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++) {
        os_slist_add(lst, (void *)&i);
    }

    os_slist_node_t * node = os_slist_head(lst);
    while (node) {
        int * num = (int *)os_slist_getdata(node);
        printf("%d\t", *num);
        node = os_slist_delete(lst, node);
    }
    printf("\n");

    for (int i = 0; i < 10; i++) {
        os_slist_add(lst, (void *)&i);
    }

    for (int i = 0; i < 10; i++) {
        os_slist_delete_ex(lst, &i);
        node = os_slist_head(lst);
        while (node) {
            int * num = (int *)os_slist_getdata(node);
            printf("%d\t", *num);
            node = os_slist_next(node);
        }
        printf("\n");
    }

    os_slist_destroy(&lst);

    return 0;
}
