#include "os_deque.h"

#include <stdint.h>
#include <string.h>
#include <malloc.h>

struct _os_deque_node_t {
    os_deque_node_t * next;  // 下一个节点
    os_deque_node_t * prev;  // 上一个节点
    char data[0];            // 节点内容
};

struct _os_deque_t {
    size_t size;              // 队列长度
    size_t elem_size;         // 元素大小
    size_t node_size;         // 节点大小
    os_deque_node_t * head;   // 队列头
    os_deque_node_t * tail;   // 队列尾
};

os_deque_t * os_deque_create(const size_t elem_size)
{
    os_deque_t * q = (os_deque_t *)calloc(1, sizeof(os_deque_t));
    if (NULL == q)
        return NULL;

    q->elem_size = elem_size;
    q->node_size = sizeof(os_deque_node_t) + elem_size;

    return q;
}

void os_deque_destroy(os_deque_t ** q)
{
    if (NULL == q || NULL == *q)
        return;

    // 先清空
    os_deque_clear(*q);

    free(*q);
    *q = NULL;
}

void os_deque_clear(os_deque_t * q)
{
    if (NULL == q)
        return;

    while (NULL != q->head) {
        os_deque_node_t * head = q->head;
        q->head = q->head->next;
        free(head);
        head = NULL;
        --q->size;
    }
}

bool os_deque_empty(os_deque_t * q)
{
    return q ? (0u == q->size) : true;
}

size_t os_deque_size(os_deque_t * q)
{
    return q ? q->size : 0u;
}

bool os_deque_push_back(os_deque_t * q, void * data)
{
    if (NULL == q || NULL == data)
        return false;

    os_deque_node_t * node = (os_deque_node_t *)calloc(1, q->node_size);
    if (NULL == node)
        return false;

    memcpy(node->data, data, q->elem_size);

    if (0u == q->size) { // 队列为空
        q->head = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        node->prev = q->tail;
        q->tail = node;
    }
    ++q->size;

    return true;
}

bool os_deque_push_front(os_deque_t * q, void * data)
{
    if (NULL == q || NULL == data)
        return false;

    os_deque_node_t * node = (os_deque_node_t *)calloc(1, q->node_size);
    if (NULL == node)
        return false;

    memcpy(node->data, data, q->elem_size);

    if (0u == q->size) {
        q->head = node;
        q->tail = node;
    } else {
        node->next = q->head;
        q->head->prev = node;
        q->head = node;
    }
    ++q->size;

    return true;
}

bool os_deque_pop_front(os_deque_t * q)
{
    if (NULL == q || 0u == q->size)
        return true;

    os_deque_node_t * head = q->head;
    q->head = q->head->next;
    free(head);
    --q->size;
    if (0u == q->size)
        q->tail = NULL;
    else
        q->head->prev = NULL;

    return true;
}

bool os_deque_pop_back(os_deque_t * q)
{
    if (NULL == q || 0u == q->size)
        return true;

    os_deque_node_t * tail = q->tail;
    q->tail = q->tail->prev;
    free(tail);
    --q->size;
    if (0u == q->size)
        q->head = NULL;
    else
        q->tail->next = NULL;

    return true;
}

os_deque_node_t * os_deque_front(os_deque_t * q)
{
    return q ? q->head : NULL;
}

os_deque_node_t * os_deque_back(os_deque_t * q)
{
    return q ? q->tail : NULL;
}

void * os_deque_getdata(const os_deque_node_t * node)
{
    return  node ? (void *)node->data : NULL;
}
