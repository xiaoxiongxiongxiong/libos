#include "os_list.h"

int main(void)
{
    os_list_t * lst = os_list_create(sizeof(int));
    if (NULL == lst)
    {
        printf("os_list_create failed for %s\n", os_last_error());
        return 1;
    }

    for (int i = 0; i < 10; i++)
    {
        os_list_add(lst, &i);
    }

    os_list_node_t * head = os_list_head(lst);
    while (NULL != head)
    {
        int * data = (int *)os_list_getdata(head);
        printf("num: %d\n", *data);
        head = os_list_next(head);
    }

    os_list_clear(lst);
    os_list_destroy(&lst);

    return 0;
}
