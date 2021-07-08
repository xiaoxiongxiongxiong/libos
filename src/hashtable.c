#include "libos.h"
#include "uthash/uthash.h"
#include <malloc.h>
#include <string.h>
#include <errno.h>

struct _os_hash_node_t
{
    UT_hash_handle hh;   // 句柄
    void * value;        // 数据指针
    size_t key_len;      // key长度
    char key[0];         // key值
};

struct _os_hash_table_t
{
    os_hash_node_t * head; // 哈希表头
    os_mutex_t * mtx;      // 互斥锁
};

os_hash_table_t * os_hash_table_create()
{
    os_hash_table_t * ht = (os_hash_table_t *)malloc(sizeof(os_hash_table_t));
    if (NULL == ht)
    {
        const int code = errno;
        log_msg_error("malloc os_hash_table_t failed, code: %d, err: %s", code, strerror(code));
        return NULL;
    }

    memset(ht, 0, sizeof(os_hash_table_t));

    ht->mtx = os_mutex_create();
    if (NULL == ht->mtx)
    {
        log_msg_warn("os_mutex_create failed");
        free(ht);
        return NULL;
    }

    return ht;
}

void os_hash_table_destroy(os_hash_table_t ** ht)
{
    if (NULL == ht || NULL == *ht)
    {
        log_msg_warn("Input param is nullptr");
        return;
    }

    // TODO 清空哈希表
    //if (0 != (*ht)->size)
    //    int a = 0;

    if (NULL != (*ht)->mtx)
    {
        os_mutex_destroy(&(*ht)->mtx);
        (*ht)->mtx = NULL;
    }

    free(*ht);
    *ht = NULL;
}

void os_hash_table_clear(os_hash_table_t * ht)
{
    if (NULL == ht)
    {
        log_msg_warn("Input param is nullptr");
        return;
    }

    os_hash_node_t * node = NULL;
    os_hash_node_t * tmp = NULL;

    os_mutex_lock(ht->mtx);
    HASH_ITER(hh, ht->head, node, tmp)
    {
        HASH_DEL(ht->head, node);

        if (node->value != NULL)
        {
            free(node->value);
            node->value = NULL;
        }

        free(node);
        node = NULL;
    }
    os_mutex_unlock(ht->mtx);
}

bool os_hash_table_empty(const os_hash_table_t * ht)
{
    if (NULL == ht)
    {
        log_msg_warn("Input param is nullptr");
        return false;
    }

    bool is_empty = false;
    os_mutex_lock(ht->mtx);
    is_empty = 0 == HASH_COUNT(ht->head);
    os_mutex_unlock(ht->mtx);

    return is_empty;
}

size_t os_hash_table_size(const os_hash_table_t * ht)
{
    if (NULL == ht)
    {
        log_msg_warn("Input param is nullptr");
        return 0;
    }

    size_t size = 0;
    os_mutex_lock(ht->mtx);
    size = HASH_COUNT(ht->head);
    os_mutex_unlock(ht->mtx);

    return size;
}

bool os_hash_table_add(os_hash_table_t * ht, const void * key, const size_t key_len, const void * value, const size_t value_len)
{
    if (NULL == ht || NULL == key || 0 == key_len)
    {
        log_msg_warn("Input param is nullptr or invalid");
        return false;
    }

    os_hash_node_t * node = NULL;
    HASH_FIND(hh, ht->head, key, key_len, node);

    if (node != NULL)
    {
        log_msg_warn("repeated key");
        return false;
    }

    node = (os_hash_node_t *)malloc(sizeof(os_hash_node_t) + key_len);
    if (NULL == node)
    {
        const int code = errno;
        log_msg_error("malloc os_hash_node_t failed, code: %d, err: %s", code, strerror(code));
        return false;
    }
    memset(node, 0, sizeof(os_hash_node_t) + key_len);

    if (NULL != value)
    {
        node->value = malloc(value_len);
        if (NULL == node->value)
        {
            const int code = errno;
            log_msg_error("malloc failed, code: %d, err: %s", code, strerror(code));
            free(node);
            return false;
        }

        memcpy(node->value, value, value_len);
    }

    memcpy(node->key, key, key_len);
    node->key_len = key_len;

    HASH_ADD(hh, ht->head, key, key_len, node);

    return true;
}

bool os_hash_table_delete(os_hash_table_t * ht, os_hash_node_t * hn)
{
    if (NULL == ht || NULL == hn)
    {
        log_msg_warn("input param is nullptr");
        return false;
    }

    HASH_DEL(ht->head, hn);

    if (NULL != hn->value)
    {
        free(hn->value);
        hn->value = NULL;
    }

    free(hn);
    hn = NULL;

    return true;
}

bool os_hash_table_delete_by_key(os_hash_table_t * ht, const void * key, const size_t key_len)
{
    if (NULL == ht || NULL == key || 0 == key_len)
    {
        log_msg_warn("Input param is nullptr or invalid");
        return false;
    }

    os_hash_node_t * hn = NULL;
    HASH_FIND(hh, ht->head, key, key_len, hn);

    if (hn == NULL)
    {
        return true;
    }

    HASH_DEL(ht->head, hn);

    if (hn->value != NULL)
    {
        free(hn->value);
        hn->value = NULL;
    }

    free(hn);
    hn = NULL;

    return true;
}
