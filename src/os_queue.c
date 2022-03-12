#include "os_queue.h"

#include <pthread.h>
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
    pthread_mutex_t mtx;
};

extern void os_set_error(const char * fmt, ...);

os_queue_t * os_queue_create(const uint32_t node_size)
{
    os_queue_t * os_queue = (os_queue_t *)malloc(sizeof(os_queue_t));
    if (NULL == os_queue)
    {
        const int code = errno;
        os_set_error("malloc failed for %s", strerror(code));
        return NULL;
    }
    os_queue->size = 0;
    os_queue->node_size = node_size;
    os_queue->head = NULL;
    os_queue->tail = NULL;

    if (0 != pthread_mutex_init(&os_queue->mtx, NULL))
    {
        os_set_error("pthread_mutex_init failed");
        free(os_queue);
        os_queue = NULL;
    }

    return os_queue;
}

void os_queue_destroy(os_queue_t ** os_queue)
{
    if (NULL == os_queue || NULL == *os_queue)
    {
        os_set_error("Input param is nullptr!");
        return;
    }

    // 先清空
    os_queue_clear(*os_queue);

    pthread_mutex_destroy(&(*os_queue)->mtx);
    free(*os_queue);
    *os_queue = NULL;
}

void os_queue_clear(os_queue_t * os_queue)
{
    if (NULL == os_queue)
    {
        os_set_error("Input param is nullptr!");
        return;
    }

    pthread_mutex_lock(&os_queue->mtx);
    while (NULL != os_queue->head)
    {
        os_queue_node_t * head = os_queue->head;
        os_queue->head = os_queue->head->next;
        free(head);
        head = NULL;
        --os_queue->size;
    }
    pthread_mutex_unlock(&os_queue->mtx);
}

bool os_queue_empty(os_queue_t * os_queue)
{
    bool is_empty = true;
    if (NULL != os_queue)
    {
        pthread_mutex_lock(&os_queue->mtx);
        is_empty = (os_queue->size == 0);
        pthread_mutex_unlock(&os_queue->mtx);
    }

    return is_empty;
}

size_t os_queue_size(os_queue_t * os_queue)
{
    size_t size = 0;
    if (NULL != os_queue)
    {
        pthread_mutex_lock(&os_queue->mtx);
        size = os_queue->size;
        pthread_mutex_unlock(&os_queue->mtx);
    }

    return size;
}

bool os_queue_push(os_queue_t * os_queue, void * data)
{
    if (NULL == os_queue || NULL == data)
    {
        os_set_error("Input param is nullptr!");
        return false;
    }

    os_queue_node_t * os_node = (os_queue_node_t *)malloc(sizeof(os_queue_node_t) + os_queue->node_size);
    if (NULL == os_node)
    {
        const int code = errno;
        os_set_error("malloc failed for %s", strerror(code));
        return false;
    }

    memcpy(os_node->data, data, os_queue->node_size);
    os_node->next = NULL;
    pthread_mutex_lock(&os_queue->mtx);
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
    pthread_mutex_unlock(&os_queue->mtx);

    return true;
}

bool os_queue_pop(os_queue_t * os_queue)
{
    if (NULL == os_queue)
    {
        os_set_error("Input param is nullptr!");
        return false;
    }

    // 队列已为空
    pthread_mutex_lock(&os_queue->mtx);
    if (0 == os_queue->size)
    {
        pthread_mutex_unlock(&os_queue->mtx);
        return true;
    }

    os_queue_node_t * head = os_queue->head;
    os_queue->head = os_queue->head->next;
    free(head);
    head = NULL;
    --os_queue->size;
    if (os_queue->size == 0)
        os_queue->tail = NULL;
    pthread_mutex_unlock(&os_queue->mtx);

    return true;
}

os_queue_node_t * os_queue_front(os_queue_t * os_queue)
{
    os_queue_node_t * node = NULL;
    if (NULL != os_queue)
    {
        pthread_mutex_lock(&os_queue->mtx);
        node = os_queue->head;
        pthread_mutex_unlock(&os_queue->mtx);
    }
    return node;
}

void * os_queue_getdata(const os_queue_node_t * node)
{
    return  node ? (void *)node->data : NULL;
}
