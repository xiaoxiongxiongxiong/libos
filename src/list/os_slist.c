#include "os_slist.h"

#include <string.h>
#include <malloc.h>

struct _os_slist_node_t {
    os_slist_node_t * next;  // 下一个节点
    char data[0];            // 数据
};

struct _os_slist_t {
    size_t size;            // 链表长度
    size_t node_size;       // 节点大小
    size_t elem_size;       // 元素大小
    os_slist_node_t * head; // 头节点
};

os_slist_t * os_slist_create(const size_t elem_size)
{
    os_slist_t * lst = (os_slist_t *)calloc(1, sizeof(os_slist_t));
    if (NULL == lst)
        return NULL;

    lst->elem_size = elem_size;
    lst->node_size = sizeof(os_slist_node_t) + elem_size;

    return lst;
}

void os_slist_destroy(os_slist_t ** lst)
{
    if (NULL == lst || NULL == *lst)
        return;

    os_slist_clear(*lst);

    free(*lst);
    *lst = NULL;
}

void os_slist_clear(os_slist_t * lst)
{
    if (NULL == lst)
        return;

    os_slist_node_t * node = NULL;
    while (NULL != lst->head) {
        node = lst->head;
        lst->head = node->next;
        free(node);
        --lst->size;
    }
}

bool os_slist_empty(os_slist_t * lst)
{
    if (NULL == lst)
        return true;

    return (0 == lst->size);
}

size_t os_slist_size(os_slist_t * lst)
{
    if (NULL == lst)
        return 0ul;

    return lst->size;
}

bool os_slist_add(os_slist_t * lst, void * data)
{
    if (NULL == lst)
        return false;

    os_slist_node_t * node = (os_slist_node_t *)calloc(1, lst->node_size);
    if (NULL == node)
        return false;

    memcpy(node->data, data, lst->elem_size);

    os_slist_node_t * tmp = lst->head;
    while (tmp && tmp->next)
        tmp = tmp->next;
    if (NULL == tmp)
        lst->head = node;
    else
        tmp->next = node;
    ++lst->size;

    return true;
}

bool os_slist_insert(os_slist_t * lst, size_t pos, void * data)
{
    if (NULL == lst || NULL == data)
        return false;

    os_slist_node_t * node = (os_slist_node_t *)calloc(1, lst->node_size);
    if (NULL == node)
        return false;

    memcpy(node->data, data, lst->elem_size);

    // 位置无效最后面插入
    os_slist_node_t * tmp = lst->head;
    if (0 == lst->size) {
        lst->head = node;
    } else if (pos < 0 || pos > lst->size) {
        while (NULL != tmp->next)
            tmp = tmp->next;
        tmp->next = node;
    } else {
        while (pos > 0) {
            tmp = tmp->next;
            pos--;
        }
        tmp->next = node;
    }

    ++lst->size;

    return true;
}

os_slist_node_t * os_slist_delete(os_slist_t * lst, os_slist_node_t * node)
{
    if (NULL == lst || NULL == node || 0u == lst->size)
        return NULL;

    os_slist_node_t * tmp = lst->head;
    if (node == tmp) {
        lst->head = tmp->next;
        free(node);
        --lst->size;
        return lst->head;
    }

    while (tmp && tmp->next) {
        if (node != tmp->next) {
            tmp = tmp->next;
            continue;
        }

        tmp->next = tmp->next->next;
        free(node);
        --lst->size;
        return tmp->next;
    }

    return NULL;
}

bool os_slist_delete_ex(os_slist_t * lst, void * data)
{
    if (NULL == lst || NULL == data)
        return true;

    os_slist_node_t * node = lst->head;
    while (node) {
        if (0 != memcmp(node->data, data, lst->elem_size)) {
            node = node->next;
            continue;
        }
        node = os_slist_delete(lst, node);
    }

    return true;
}

os_slist_node_t * os_slist_head(os_slist_t * lst)
{
    return lst ? lst->head : NULL;
}

os_slist_node_t * os_slist_next(const os_slist_node_t * node)
{
    return node ? node->next : NULL;
}

void * os_slist_getdata(const os_slist_node_t * node)
{
    return node ? (void *)node->data : NULL;
}
