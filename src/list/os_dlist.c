#include "os_dlist.h"

#include <string.h>
#include <malloc.h>

struct _os_dlist_node_t {
    os_dlist_node_t * next;  // 下一个节点
    os_dlist_node_t * prev;  // 上一个节点
    char data[0];            // 数据
};

struct _os_dlist_t {
    size_t size;            // 链表长度
    size_t elem_size;       // 元素大小
    size_t node_size;       // 节点大小
    os_dlist_node_t * head; // 头节点
    os_dlist_node_t * tail; // 尾节点
};

// 检测是否存在某个节点
static bool os_dlist_exist(const os_dlist_t * lst, const os_dlist_node_t * node);

os_dlist_t * os_dlist_create(const size_t elem_size)
{
    os_dlist_t * lst = (os_dlist_t *)calloc(1, sizeof(os_dlist_t));
    if (NULL == lst)
        return NULL;

    lst->elem_size = elem_size;
    lst->node_size = sizeof(os_dlist_node_t) + elem_size;

    return lst;
}

void os_dlist_destroy(os_dlist_t ** lst)
{
    if (NULL == lst || NULL == *lst)
        return;

    os_dlist_clear(*lst);

    free(*lst);
    *lst = NULL;
}

void os_dlist_clear(os_dlist_t * lst)
{
    if (NULL == lst)
        return;

    os_dlist_node_t * node = NULL;
    while (NULL != lst->head) {
        node = lst->head;
        lst->head = node->next;
        free(node);
        --lst->size;
    }
    lst->tail = NULL;
}

bool os_dlist_empty(os_dlist_t * lst)
{
    if (NULL == lst)
        return true;

    return 0ul == lst->size;
}

size_t os_dlist_size(os_dlist_t * lst)
{
    if (NULL == lst)
        return 0ul;

    return lst->size;
}

bool os_dlist_add(os_dlist_t * lst, void * data)
{
    if (NULL == lst)
        return false;

    os_dlist_node_t * node = (os_dlist_node_t *)calloc(1, lst->node_size);
    if (NULL == node)
        return false;

    memcpy(node->data, data, lst->elem_size);

    if (NULL == lst->head) {
        lst->head = node;
        lst->tail = node;
    } else {
        lst->tail->next = node;
        node->prev = lst->tail;
        lst->tail = node;
    }
    ++lst->size;

    return true;
}

bool os_dlist_insert(os_dlist_t * lst, size_t pos, void * data)
{
    if (NULL == lst || NULL == data)
        return false;

    os_dlist_node_t * node = (os_dlist_node_t *)calloc(1, lst->node_size);
    if (NULL == node)
        return false;

    memcpy(node->data, data, lst->elem_size);

    if (0 == lst->size) {
        lst->head = node;
        lst->tail = node;
    } else if (0 == pos) {
        node->next = lst->head;
        lst->head->prev = node;
        lst->head = node;
    } else if (pos >= lst->size) {
        node->prev = lst->tail;
        lst->tail->next = node;
        lst->tail = node;
    } else {
        os_dlist_node_t * tmp = lst->head;
        for (size_t i = 0; i < pos - 1; i++)
            tmp = tmp->next;
        node->next = tmp->next;
        node->prev = tmp;
        tmp->next->prev = node;
        tmp->next = node;
    }

    lst->size++;

    return true;
}

os_dlist_node_t * os_dlist_delete(os_dlist_t * lst, os_dlist_node_t * node)
{
    if (NULL == lst || NULL == node)
        return NULL;

    os_dlist_node_t * tmp = NULL;
    if (node == lst->head) {
        tmp = lst->head->next;
        lst->head = lst->head->next;
        if (NULL == lst->head)
            lst->tail = NULL;
        else
            lst->head->prev = NULL;
    } else if (node == lst->tail) {
        lst->tail = lst->tail->prev;
        if (NULL != lst->tail)
            lst->tail->next = NULL;
    } else {
        if (!os_dlist_exist(lst, node))
            return NULL;
        tmp = node->next;
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
    --lst->size;

    free(node);

    return tmp;
}

bool os_dlist_delete_ex(os_dlist_t * lst, void * data)
{
    if (NULL == lst || NULL == data)
        return true;

    os_dlist_node_t * node = lst->head;
    while (node) {
        if (0 != memcmp(node->data, data, lst->elem_size)) {
            node = node->next;
            continue;
        }
        node = os_dlist_delete(lst, node);
    }

    return true;
}

os_dlist_node_t * os_dlist_head(os_dlist_t * lst)
{
    return lst ? lst->head : NULL;
}

os_dlist_node_t * os_dlist_tail(os_dlist_t * lst)
{
    return lst ? lst->tail : NULL;
}

os_dlist_node_t * os_dlist_next(const os_dlist_node_t * node)
{
    return node ? node->next : NULL;
}

os_dlist_node_t * os_dlist_prev(const os_dlist_node_t * node)
{
    return node ? node->prev : NULL;
}

void * os_dlist_getdata(const os_dlist_node_t * node)
{
    return node ? (void *)node->data : NULL;
}

bool os_dlist_exist(const os_dlist_t * lst, const os_dlist_node_t * node)
{
    os_dlist_node_t * tmp = lst->head;
    while (tmp) {
        if (tmp == node)
            return true;
        tmp = tmp->next;
    }

    return false;
}
