#include "libos.h"
#include <stdint.h>
#include <string.h>
#include <malloc.h>
#include <errno.h>

struct _os_queue_node_t
{
    os_queue_node_t * next;  // 下一个节点
    char data[0];            // 节点内容
};

struct _os_queue_t
{
    size_t size;              // 队列长度
    uint32_t node_size;       // 节点数据大小
    os_queue_node_t * head;   // 队列头
    os_queue_node_t * tail;   // 队列尾
    os_mutex_t * mtx;
};

os_queue_t * os_queue_create(const uint32_t node_size)
{
    os_queue_t * os_queue = (os_queue_t *)malloc(sizeof(os_queue_t));
    if (NULL == os_queue)
    {
        log_msg_error("malloc failed, err:%s", strerror(errno));
        return NULL;
    }
    os_queue->size = 0;
    os_queue->node_size = node_size;
    os_queue->head = NULL;
    os_queue->tail = NULL;
    os_queue->mtx = os_mutex_create();
    if (NULL == os_queue->mtx)
    {
        log_msg_warn("os_mutex_create failed!");
        free(os_queue);
        return NULL;
    }

    return os_queue;
}

void os_queue_destroy(os_queue_t ** os_queue)
{
    if (NULL == os_queue || NULL == *os_queue)
    {
        log_msg_warn("Input param is nullptr!");
        return;
    }

    // 不为空则清空
    if (!os_queue_empty(*os_queue))
    {
        os_queue_clear(*os_queue);
    }

    os_mutex_destroy(&(*os_queue)->mtx);
    free(*os_queue);
    *os_queue = NULL;
}

void os_queue_clear(os_queue_t * os_queue)
{
    if (NULL == os_queue)
    {
        log_msg_warn("Input param is nullptr!");
        return;
    }

    os_mutex_lock(os_queue->mtx);
    while (os_queue->size)
    {
        os_queue_node_t * head = os_queue->head;
        os_queue->head = os_queue->head->next;
        free(head);
        head = NULL;
        --os_queue->size;
    }
    os_mutex_unlock(os_queue->mtx);
}

bool os_queue_empty(os_queue_t * os_queue)
{
    if (NULL == os_queue)
    {
        log_msg_warn("Input param is nullptr!");
        return true;
    }

    bool is_empty = false;
    os_mutex_lock(os_queue->mtx);
    is_empty = (os_queue->size == 0);
    os_mutex_unlock(os_queue->mtx);

    return is_empty;
}

size_t os_queue_size(const os_queue_t * os_queue)
{
    if (NULL == os_queue)
    {
        log_msg_warn("Input param is nullptr!");
        return 0;
    }

    size_t size = 0;
    os_mutex_lock(os_queue->mtx);
    size = os_queue->size;
    os_mutex_unlock(os_queue->mtx);

    return size;
}

bool os_queue_push(os_queue_t * os_queue, void * data)
{
    if (NULL == os_queue || NULL == data)
    {
        log_msg_warn("Input param is nullptr!");
        return false;
    }

    os_queue_node_t * os_node = (os_queue_node_t *)malloc(sizeof(os_queue_node_t) + os_queue->node_size);
    if (NULL == os_node)
    {
        log_msg_error("malloc failed, err:%s", strerror(errno));
        return false;
    }

    memcpy(os_node->data, data, os_queue->node_size);
    os_node->next = NULL;
    os_mutex_lock(os_queue->mtx);
    if (0 == os_queue->size) // 队列为空
    {
        os_queue->head = os_node;
        os_queue->tail = os_node;
    }
    else
    {
        os_queue->tail->next = os_node;
        os_queue->tail = os_node;
    }
    ++os_queue->size;
    os_mutex_unlock(os_queue->mtx);

    return true;
}

bool os_queue_pop(os_queue_t * os_queue)
{
    if (NULL == os_queue)
    {
        log_msg_warn("Input param is nullptr!");
        return false;
    }

    // 队列已为空
    os_mutex_lock(os_queue->mtx);
    if (0 == os_queue->size)
    {
        log_msg_warn("queue has already empty!");
        os_mutex_unlock(os_queue->mtx);
        return true;
    }

    os_queue_node_t * head = os_queue->head;
    os_queue->head = os_queue->head->next;
    free(head);
    head = NULL;
    --os_queue->size;
    os_mutex_unlock(os_queue->mtx);

    return true;
}

os_queue_node_t * os_queue_front(os_queue_t * os_queue)
{
    if (NULL == os_queue || 0 == os_queue->size)
    {
        log_msg_warn("Input param is nullptr or queue empty!");
        return NULL;
    }

    os_queue_node_t * node = NULL;
    os_mutex_lock(os_queue->mtx);
    node = os_queue->head;
    os_mutex_unlock(os_queue->mtx);
    return node;
}

void * os_queue_getdata(const os_queue_node_t * node)
{
    if (NULL == node)
    {
        log_msg_warn("Input param is nullptr!");
        return 0;
    }

    return (void *)node->data;
}
