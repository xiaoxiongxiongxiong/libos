#include "os_list.h"

#include <string.h>
#include <errno.h>
#include <malloc.h>
#include <pthread.h>

struct _os_list_node_t
{
    os_list_node_t * next;  // 下一个节点
    os_list_node_t * prev;  // 上一个节点
    char data[0];            // 数据
};

struct _os_list_t
{
    size_t size;             // 链表长度
    size_t node_size;        // 节点数据大小
    os_list_node_t * head;  // 头节点
    os_list_node_t * tail;  // 尾节点
    pthread_mutex_t mtx;        // 互斥锁
};

extern void os_set_error(const char * fmt, ...);

os_list_t * os_list_create(const size_t node_size)
{
    os_list_t * os_lst = (os_list_t *)malloc(sizeof(os_list_t));
    if (NULL == os_lst)
    {
        const int code = errno;
        os_set_error("malloc failed for %s", strerror(code));
        return NULL;
    }

    memset(os_lst, 0, sizeof(os_list_t));
    os_lst->node_size = node_size;

    if (0 != pthread_mutex_init(&os_lst->mtx, NULL))
    {
        os_set_error("os_mutex_create failed");
        free(os_lst);
        return NULL;
    }

    return os_lst;
}

void os_list_destroy(os_list_t ** os_lst)
{
    if (NULL == os_lst || NULL == *os_lst)
    {
        os_set_error("Input param is nullptr!");
        return;
    }

    os_list_clear(*os_lst);

    pthread_mutex_destroy(&(*os_lst)->mtx);

    free(*os_lst);
    *os_lst = NULL;
}

void os_list_clear(os_list_t * os_lst)
{
    if (NULL == os_lst)
    {
        os_set_error("Input param is nullptr!");
        return;
    }

    os_list_node_t * node = NULL;
    pthread_mutex_lock(&os_lst->mtx);
    while (NULL != os_lst->head)
    {
        node = os_lst->head;
        os_lst->head = node->next;
        free(node);
        --os_lst->size;
    }
    os_lst->tail = NULL;
    pthread_mutex_unlock(&os_lst->mtx);
}

bool os_list_empty(os_list_t * os_lst)
{
    bool is_empty = true;
    if (NULL != os_lst)
    {
        pthread_mutex_lock(&os_lst->mtx);
        is_empty = (0 == os_lst->size);
        pthread_mutex_unlock(&os_lst->mtx);
    }

    return is_empty;
}

size_t os_list_size(os_list_t * os_lst)
{
    size_t size = 0;
    if (NULL != os_lst)
    {
        pthread_mutex_lock(&os_lst->mtx);
        size = os_lst->size;
        pthread_mutex_unlock(&os_lst->mtx);
    }

    return size;
}

bool os_list_add(os_list_t * os_lst, void * data)
{
    if (NULL == os_lst || NULL == data)
    {
        os_set_error("Input param is nullptr!");
        return false;
    }

    os_list_node_t * node = (os_list_node_t *)malloc(sizeof(os_list_node_t) + os_lst->node_size);
    if (NULL == node)
    {
        const int code = errno;
        os_set_error("malloc failed for %s", strerror(code));
        return false;
    }
    memcpy(node->data, data, os_lst->node_size);
    node->next = NULL;
    node->prev = NULL;

    pthread_mutex_lock(&os_lst->mtx);
    if (0 == os_lst->size)
    {
        os_lst->head = node;
        os_lst->tail = node;
    }
    else
    {
        node->prev = os_lst->tail;
        os_lst->tail->next = node;
        os_lst->tail = node;
    }
    ++os_lst->size;
    pthread_mutex_unlock(&os_lst->mtx);

    return true;
}

bool os_list_delete(os_list_t * os_lst, os_list_node_t * os_node)
{
    if (NULL == os_lst || NULL == os_node)
    {
        os_set_error("Input param is nullptr!");
        return false;
    }

    pthread_mutex_lock(&os_lst->mtx);
    if (os_node == os_lst->head)
    {
        os_lst->head = os_lst->head->next;
        if (NULL == os_lst->head)
            os_lst->tail = NULL;
        else
            os_lst->head->prev = NULL;
    }
    else if (os_node == os_lst->tail)
    {
        os_lst->tail = os_lst->tail->prev;
        if (NULL != os_lst->tail)
            os_lst->tail->next = NULL;
    }
    else
    {
        os_node->next->prev = os_node->prev;
        os_node->prev->next = os_node->next;
    }
    --os_lst->size;
    pthread_mutex_unlock(&os_lst->mtx);
    free(os_node);

    return true;
}

os_list_node_t * os_list_head(os_list_t * os_lst)
{
    os_list_node_t * head = NULL;
    if (NULL != os_lst)
    {
        pthread_mutex_lock(&os_lst->mtx);
        head = os_lst->head;
        pthread_mutex_unlock(&os_lst->mtx);
    }

    return head;
}

os_list_node_t * os_list_tail(os_list_t * os_lst)
{
    os_list_node_t * tail = NULL;
    if (NULL != os_lst)
    {
        pthread_mutex_lock(&os_lst->mtx);
        tail = os_lst->tail;
        pthread_mutex_unlock(&os_lst->mtx);
    }

    return tail;
}

os_list_node_t * os_list_next(const os_list_node_t * os_node)
{
    return os_node ? os_node->next : NULL;
}

os_list_node_t * os_list_prev(const os_list_node_t * os_node)
{
    return os_node ? os_node->prev : NULL;
}

void * os_list_getdata(const os_list_node_t * node)
{
    return node ? (void *)node->data : NULL;
}
