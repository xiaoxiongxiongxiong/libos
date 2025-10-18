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
    os_slist_node_t * tail; // 尾节点
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

bool os_slist_empty(const os_slist_t * lst)
{
    if (NULL == lst)
        return true;

    return (0 == lst->size);
}

size_t os_slist_size(const os_slist_t * lst)
{
    return lst ? lst->size : 0ul;
}

bool os_slist_add(os_slist_t * lst, void * data)
{
    if (NULL == lst)
        return false;

    os_slist_node_t * node = (os_slist_node_t *)calloc(1, lst->node_size);
    if (NULL == node)
        return false;

    memcpy(node->data, data, lst->elem_size);

    if (NULL == lst->head) {
        lst->head = node;
        lst->tail = node;
    } else {
        lst->tail->next = node;
        lst->tail = node;
    }

    ++lst->size;

    return true;
}

bool os_slist_insert(os_slist_t * lst, size_t pos, void * data)
{
    if (NULL == lst || NULL == data)
        return false;

    if (pos >= lst->size)
        return os_slist_add(lst, data);

    os_slist_node_t * node = (os_slist_node_t *)calloc(1, lst->node_size);
    if (NULL == node)
        return false;

    memcpy(node->data, data, lst->elem_size);

    if (0ul == pos) {
        node->next = lst->head;
        lst->head = node;
    } else {
        os_slist_node_t * tmp = lst->head;
        while (pos > 1) {
            tmp = tmp->next;
            pos--;
        }
        node->next = tmp->next;
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

    os_slist_node_t * res = NULL;
    while (tmp->next) {
        if (tmp->next != node) {
            tmp = tmp->next;
            continue;
        }

        res = tmp->next->next;
        tmp->next = res;
        --lst->size;
        free(node);
        break;
    }

    return res;
}

bool os_slist_delete_ex(os_slist_t * lst, void * data, os_slist_compare compare)
{
    if (NULL == lst || NULL == data)
        return true;

    os_slist_node_t * node = lst->head;
    while (node) {
        if (!compare(node->data, data)) {
            node = node->next;
            continue;
        }
        node = os_slist_delete(lst, node);
    }

    return true;
}

os_slist_node_t * os_slist_head(const os_slist_t * lst)
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

os_slist_t * os_slist_reverse_order(os_slist_t * lst)
{
    if (NULL == lst)
        return NULL;

    os_slist_node_t * tmp = NULL;
    os_slist_node_t * node = NULL;
    os_slist_node_t * head = lst->head;
    while (head) {
        tmp = head->next;
        head->next = node;
        node = head;
        head = tmp;
    }

    lst->tail = lst->head;
    lst->head = node;
    return lst;
}

os_slist_t * os_slist_merge(os_slist_t * lst1, os_slist_t * lst2, os_slist_compare compare)
{
    if (lst1 && lst2 && lst1->elem_size != lst2->elem_size)
        return NULL;

    if (NULL == lst1 || 0u == lst1->size)
        return lst2;

    if (NULL == lst2 || 0u == lst2->size)
        return lst1;

    if (NULL == compare) {
        lst1->tail->next = lst2->head;
        lst1->tail = lst2->tail;
        lst1->size += lst2->size;
        return lst1;
    }

    os_slist_node_t * head = NULL;
    os_slist_node_t * head1 = lst1->head;
    os_slist_node_t * head2 = lst2->head;

    os_slist_node_t * tmp = NULL;
    os_slist_node_t * node = NULL;
    while (head1 && head2) {
        if (compare(head1->data, head2->data)) {
            node = head1;
            head1 = head1->next;
        } else {
            node = head2;
            head2 = head2->next;
        }
        node->next = NULL;
        if (head) {
            tmp->next = node;
            tmp = tmp->next;
        } else {
            head = node;
            tmp = head;
        }
    }

    if (head1)
        tmp->next = head1;

    if (head2) {
        tmp->next = head2;
        lst1->tail = lst2->tail;
    }

    lst1->head = head;
    lst1->size += lst2->size;

    return lst1;
}
