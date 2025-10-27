#include "os_rbt.h"

int main()
{
    os_rbt_t * rbt = os_rbt_create(sizeof(int), NULL);
    if (NULL == rbt) {
        fprintf(stderr, "os_rbt_create failed\n");
        return 1;
    }

    for (int i = 1; i <= 10; i++) {
        os_rbt_insert(rbt, (void *)&i);
    }

    os_rbt_destroy(&rbt);

    return 0;
}