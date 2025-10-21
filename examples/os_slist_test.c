#include "os_slist.h"

static void os_slist_print(const os_slist_t * lst)
{
    int cnt = 0;
    os_slist_node_t * head = os_slist_head(lst);
    while (head) {
        int * num = (int *)os_slist_getdata(head);
        fprintf(stdout, "%d\t", *num);
        cnt++;
        if (cnt >= 10) {
            fprintf(stdout, "\n");
            cnt = 0;
        }
        head = os_slist_next(head);
    }
}

static bool os_merge_compare(const void * data1, const void * data2)
{
    int * num1 = (int *)data1;
    int * num2 = (int *)data2;
    return *num1 < *num2;
}

static bool os_delete_compare(const void * data1, const void * data2)
{
    int * num1 = (int *)data1;
    int * num2 = (int *)data2;
    return *num1 == *num2;
}

int main(int argc, char * argv[])
{
    os_slist_t * lst1 = os_slist_create(sizeof(int));
    if (NULL == lst1) {
        fprintf(stderr, "os_slist_create failed\n");
        return 1;
    }

    os_slist_t * lst2 = os_slist_create(sizeof(int));
    if (NULL == lst2) {
        fprintf(stderr, "os_slist_create failed\n");
        return 1;
    }

    for (int i = 0; i < 20;) {
        os_slist_add(lst1, (void *)&i);
        i++;
        os_slist_add(lst2, (void *)&i);
        i++;
    }
    printf("lst1: \n");
    os_slist_print(lst1);
    printf("lst2: \n");
    os_slist_print(lst2);

    os_slist_t * lst = os_slist_merge(lst1, lst2, os_merge_compare);
    printf("after merge: \n");
    os_slist_print(lst);
    lst = os_slist_reverse_order(lst);
    printf("after reorder: \n");
    os_slist_print(lst);
    int cnt = 10;
    os_slist_delete_ex(lst, &cnt, os_delete_compare);
    cnt += 5;
    os_slist_delete_ex(lst, &cnt, os_delete_compare);
    printf("after delete: \n");
    os_slist_print(lst);

    os_slist_destroy(&lst1);

    return 0;
}
