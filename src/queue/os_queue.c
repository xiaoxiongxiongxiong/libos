#include "os_queue.h"

#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>

struct _os_queue_node_t {
    os_queue_node_t * next;  // 下一个节点
    char data[0];            // 节点内容
};

struct _os_queue_t {
    size_t size;              // 队列长度
    size_t elem_size;         // 元素大小
    size_t node_size;         // 节点大小
    os_queue_node_t * head;   // 队列头
    os_queue_node_t * tail;   // 队列尾
};

os_queue_t * os_queue_create(const uint32_t elem_size)
{
    os_queue_t * q = (os_queue_t *)calloc(1, sizeof(os_queue_t));
    if (NULL == q)
        return NULL;

    q->elem_size = elem_size;
    q->node_size = sizeof(os_queue_node_t) + elem_size;

    return q;
}

void os_queue_destroy(os_queue_t ** q)
{
    if (NULL == q || NULL == *q)
        return;

    // 先清空
    os_queue_clear(*q);

    free(*q);
    *q = NULL;
}

void os_queue_clear(os_queue_t * q)
{
    if (NULL == q)
        return;

    while (NULL != q->head) {
        os_queue_node_t * head = q->head;
        q->head = q->head->next;
        free(head);
        head = NULL;
        --q->size;
    }
}

bool os_queue_empty(os_queue_t * q)
{
    return q ? (0u == q->size) : true;
}

size_t os_queue_size(os_queue_t * q)
{
    return q ? q->size : 0u;
}

bool os_queue_push(os_queue_t * q, void * data)
{
    if (NULL == q || NULL == data)
        return false;

    os_queue_node_t * node = (os_queue_node_t *)malloc(sizeof(os_queue_node_t) + q->node_size);
    if (NULL == node)
        return false;

    memcpy(node->data, data, q->node_size);
    node->next = NULL;
    if (0 == q->size) { // 队列为空
        q->head = node;
        q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    ++q->size;

    return true;
}

bool os_queue_pop(os_queue_t * q)
{
    if (NULL == q || 0u == q->size)
        return true;

    os_queue_node_t * head = q->head;
    q->head = q->head->next;
    free(head);
    --q->size;
    if (0u == q->size)
        q->tail = NULL;

    return true;
}

os_queue_node_t * os_queue_front(os_queue_t * q)
{
    return q ? q->head : NULL;
}

void * os_queue_getdata(const os_queue_node_t * node)
{
    return  node ? (void *)node->data : NULL;
}
