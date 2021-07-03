#include "libos.h"
#include <malloc.h>
#include <string.h>
#include <errno.h>

struct _os_dlist_node_t
{
    os_dlist_node_t * next;  // 下一个节点
    os_dlist_node_t * prev;  // 上一个节点
    char data[0];            // 数据
};

struct _os_dlist_t
{
    size_t size;             // 链表长度
    size_t node_size;        // 节点数据大小
    os_dlist_node_t * head;  // 头节点
    os_dlist_node_t * tail;  // 尾节点
    os_mutex_t * mtx;        // 互斥锁
};

os_dlist_t * os_dlist_create(const size_t node_size)
{
    os_dlist_t * os_lst = (os_dlist_t *)malloc(sizeof(os_dlist_t));
    if (NULL == os_lst)
    {
        log_msg_error("malloc failed, err:%s", strerror(errno));
        return NULL;
    }

    os_lst->mtx = os_mutex_create();
    if (NULL == os_lst->mtx)
    {
        log_msg_warn("os_mutex_create failed!");
        free(os_lst);
        return NULL;
    }

    os_lst->size = 0;
    os_lst->node_size = node_size;
    os_lst->head = os_lst->tail = NULL;

    return os_lst;
}

void os_dlist_destroy(os_dlist_t ** os_lst)
{
    if (NULL == os_lst || NULL == *os_lst)
    {
        log_msg_warn("Input param is nullptr!");
        return;
    }

    if (!os_dlist_empty(*os_lst))
        os_dlist_clear(*os_lst);

    if (NULL != (*os_lst)->mtx)
    {
        os_mutex_destroy(&(*os_lst)->mtx);
        (*os_lst)->mtx = NULL;
    }

    free(*os_lst);
    *os_lst = NULL;
}

void os_dlist_clear(os_dlist_t * os_lst)
{
    if (NULL == os_lst)
    {
        log_msg_warn("Input param is nullptr!");
        return;
    }

    os_dlist_node_t * node = NULL;
    os_mutex_lock(os_lst->mtx);
    while (os_lst->size--)
    {
        node = os_lst->head;
        os_lst->head = os_lst->head->next;
        os_lst->head->next->prev = NULL;
        free(node);
    }
    os_mutex_unlock(os_lst->mtx);
}

bool os_dlist_empty(const os_dlist_t * os_lst)
{
    if (NULL == os_lst)
    {
        log_msg_warn("Input param is nullptr!");
        return false;
    }

    bool is_empty = false;
    os_mutex_lock(os_lst->mtx);
    is_empty = (0 == os_lst->size);
    os_mutex_unlock(os_lst->mtx);

    return is_empty;
}

size_t os_dlist_size(const os_dlist_t * os_lst)
{
    if (NULL == os_lst)
    {
        log_msg_warn("Input param is nullptr!");
        return false;
    }

    size_t size = 0;
    os_mutex_lock(os_lst->mtx);
    size = os_lst->size;
    os_mutex_unlock(os_lst->mtx);

    return size;
}

bool os_dlist_add(os_dlist_t * os_lst, os_dlist_node_t * os_node, void * data)
{
    if (NULL == os_lst || NULL == data)
    {
        log_msg_warn("Input param is nullptr!");
        return false;
    }

    os_dlist_node_t * node = (os_dlist_node_t *)malloc(sizeof(os_dlist_node_t) + os_lst->node_size);
    if (NULL == node)
    {
        log_msg_error("malloc failed, err:%s", strerror(errno));
        return false;
    }
    memcpy(node->data, data, os_lst->node_size);
    node->next = NULL;
    node->prev = NULL;

    os_mutex_lock(os_lst->mtx);
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
    os_mutex_unlock(os_lst->mtx);

    return true;
}

bool os_dlist_delete(os_dlist_t * os_lst, os_dlist_node_t * os_node)
{
    if (NULL == os_lst || NULL == os_node)
    {
        log_msg_warn("Input param is nullptr!");
        return false;
    }

    os_mutex_lock(os_lst->mtx);
    if (os_node == os_lst->head)
    {
        os_lst->head = os_lst->head->next;
        os_lst->head->next->prev = NULL;
    }
    else if (os_node == os_lst->tail)
    {
        os_lst->tail = os_lst->tail->prev;
        os_lst->tail->prev->next = NULL;
    }
    else
    {
        os_node->next->prev = os_node->prev;
        os_node->prev->next = os_node->next;
    }
    --os_lst->size;
    os_mutex_unlock(os_lst->mtx);
    free(os_node);

    return true;
}

os_dlist_node_t * os_dlist_head(const os_dlist_t * os_lst)
{
    if (NULL == os_lst)
    {
        log_msg_warn("Input param is nullptr!");
        return NULL;
    }

    os_dlist_node_t * head = NULL;
    os_mutex_lock(os_lst->mtx);
    head = os_lst->head;
    os_mutex_unlock(os_lst->mtx);

    return head;
}

os_dlist_node_t * os_dlist_tail(const os_dlist_t * os_lst)
{
    if (NULL == os_lst)
    {
        log_msg_warn("Input param is nullptr!");
        return NULL;
    }

    os_dlist_node_t * tail = NULL;
    os_mutex_lock(os_lst->mtx);
    tail = os_lst->tail;
    os_mutex_unlock(os_lst->mtx);

    return tail;
}

os_dlist_node_t * os_dlist_next(const os_dlist_node_t * os_node)
{
    if (NULL == os_node)
    {
        log_msg_warn("Input param is nullptr!");
        return NULL;
    }

    return os_node->next;
}

os_dlist_node_t * os_dlist_prev(const os_dlist_node_t * os_node)
{
    if (NULL == os_node)
    {
        log_msg_warn("Input param is nullptr!");
        return NULL;
    }

    return os_node->prev;
}

void * os_dlist_getdata(const os_dlist_node_t * node)
{
    if (NULL == node)
    {
        log_msg_warn("Input param is nullptr!");
        return NULL;
    }

    return (void *)node->data;
}
