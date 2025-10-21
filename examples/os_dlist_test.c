#include "os_dlist.h"
#include <stdlib.h>
#include <time.h>

int main(void)
{
    srand((unsigned int)time(NULL));  // 用当前时间作为种子

    os_dlist_t * lst = os_dlist_create(sizeof(int));
    if (NULL == lst)
    {
        printf("os_list_create failed\n");
        return 1;
    }

    for (int i = 0; i < 10; i++)
    {
        os_dlist_add(lst, &i);
    }
    int cnt = 10;
    os_dlist_add(lst, &cnt);
    cnt++;
    os_dlist_insert(lst, 0, &cnt);
    cnt++;
    os_dlist_insert(lst, 1, &cnt);
    cnt++;
    os_dlist_insert(lst, os_dlist_size(lst), &cnt);

    os_dlist_delete(lst, (os_dlist_node_t*)100);
    //for (int i = 0; i < 10; i++)
    //{
    //    os_dlist_add(lst, &i);
    //}

    //for (int i = 11; i < 20; i++)
    //{
    //    size_t pos = rand() % os_dlist_size(lst);
    //    os_dlist_insert(lst, pos, &i);
    //}

    //for (int i = 0; i < 5; i++)
    //{
    //    int num = rand() % 10;
    //    printf("delete %d\n", num);
    //    os_dlist_delete_ex(lst, &num);
    //}

    os_dlist_node_t * head = os_dlist_head(lst);
    while (NULL != head) {
        int * data = (int *)os_dlist_getdata(head);
        printf("num: %d\n", *data);
        head = os_dlist_next(head);
    }

    os_dlist_clear(lst);
    os_dlist_destroy(&lst);

    return 0;
}
