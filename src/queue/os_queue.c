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
    uint32_t node_size;       // 节点数据大小
    os_queue_node_t * head;   // 队列头
    os_queue_node_t * tail;   // 队列尾
};

os_queue_t * os_queue_create(const uint32_t node_size)
{
    os_queue_t * os_queue = (os_queue_t *)malloc(sizeof(os_queue_t));
    if (NULL == os_queue)
        return NULL;

    os_queue->size = 0;
    os_queue->node_size = node_size;
    os_queue->head = NULL;
    os_queue->tail = NULL;

    return os_queue;
}

void os_queue_destroy(os_queue_t ** os_queue)
{
    if (NULL == os_queue || NULL == *os_queue)
        return;

    // 先清空
    os_queue_clear(*os_queue);

    free(*os_queue);
    *os_queue = NULL;
}

void os_queue_clear(os_queue_t * os_queue)
{
    if (NULL == os_queue)
        return;

    while (NULL != os_queue->head) {
        os_queue_node_t * head = os_queue->head;
        os_queue->head = os_queue->head->next;
        free(head);
        head = NULL;
        --os_queue->size;
    }
}

bool os_queue_empty(os_queue_t * os_queue)
{
    if (NULL == os_queue)
        return false;

    return (0 == os_queue->size);
}

size_t os_queue_size(os_queue_t * os_queue)
{
    if (NULL == os_queue)
        return 0;

    return os_queue->size;
}

bool os_queue_push(os_queue_t * os_queue, void * data)
{
    if (NULL == os_queue || NULL == data)
        return false;

    os_queue_node_t * os_node = (os_queue_node_t *)malloc(sizeof(os_queue_node_t) + os_queue->node_size);
    if (NULL == os_node)
        return false;

    memcpy(os_node->data, data, os_queue->node_size);
    os_node->next = NULL;
    if (0 == os_queue->size) { // 队列为空
        os_queue->head = os_node;
        os_queue->tail = os_node;
    } else {
        os_queue->tail->next = os_node;
        os_queue->tail = os_node;
    }
    ++os_queue->size;

    return true;
}

bool os_queue_pop(os_queue_t * os_queue)
{
    if (NULL == os_queue)
        return false;

    // 队列已为空
    if (0 == os_queue->size)
        return true;

    os_queue_node_t * head = os_queue->head;
    os_queue->head = os_queue->head->next;
    free(head);
    head = NULL;
    --os_queue->size;
    if (os_queue->size == 0)
        os_queue->tail = NULL;

    return true;
}

os_queue_node_t * os_queue_front(os_queue_t * os_queue)
{
    if (NULL == os_queue)
        return NULL;

    return os_queue->head;
}

void * os_queue_getdata(const os_queue_node_t * node)
{
    return  node ? (void *)node->data : NULL;
}
