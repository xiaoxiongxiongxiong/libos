#ifndef _OS_SINGLE_LIST_H_
#define _OS_SINGLE_LIST_H_

#include "libos.h"

typedef struct _os_slist_t os_slist_t;
typedef struct _os_slist_node_t os_slist_node_t;

OS_API_BEGIN

/*
* os_slist_create
* @brief  ��������
* @param  elem_size  �ڵ��С
* @return ����ָ�����ΪNULL
*/
OS_API os_slist_t * os_slist_create(const size_t elem_size);

/*
* os_slist_destroy
* @brief  ��������
* @param  lst  ָ������ָ���ָ��
*/
OS_API void os_slist_destroy(os_slist_t ** lst);

/*
* os_slist_clear
* @brief  �������
* @param  lst  ����ָ��
*/
OS_API void os_slist_clear(os_slist_t * lst);

/*
* os_slist_empty
* @brief  �ж������Ƿ�Ϊ��
* @param  lst  ����ָ��
* @return true--�ɹ� false--ʧ��
*/
OS_API bool os_slist_empty(os_slist_t * lst);

/*
* os_slist_size
* @brief  ��ȡ������
* @param  lst  ����ָ��
* @return ������
*/
OS_API size_t os_slist_size(os_slist_t * lst);

/*
* os_slist_add
* @brief  ������β���������
* @param  lst   ����ָ��
* @param  data  ����
* @return true--�ɹ� false--ʧ��
*/
OS_API bool os_slist_add(os_slist_t * lst, void * data);

/*
* os_slist_insert
* @brief  ������ָ��λ�ò�������
* @param  lst   ����
* @param  pos   λ��
* @param  data  ����
* @return true--�ɹ�  false--ʧ��
*/
OS_API bool os_slist_insert(os_slist_t * lst, size_t pos, void * data);

/*
* os_slist_delete
* @brief  ��������ɾ������
* @param  lst   ����ָ��
* @param  node  Ҫɾ���Ľڵ�ָ��
* @return NULL ������һ���ڵ�
*/
OS_API os_slist_node_t * os_slist_delete(os_slist_t * lst, os_slist_node_t * node);

/*
* os_slist_delete_ex
* @brief  �������ݴ�������ɾ���ڵ�
* @param  lst   ����ָ��
* @param  data  ����
* @return true--�ɹ� false--ʧ��
*/
OS_API bool os_slist_delete_ex(os_slist_t * lst, void * data);

/*
* os_slist_head
* @brief  ��ȡ����ͷ�ڵ�
* @param  lst   ����ָ��
* @return ͷ�ڵ�ָ�����NULL
*/
OS_API os_slist_node_t * os_slist_head(os_slist_t * lst);

/*
* os_slist_next
* @brief  ��ȡ�ڵ����һ���ڵ�
* @param  node   �ڵ�ָ��
* @return �ڵ���һ���ڵ�ָ�����NULL
*/
OS_API os_slist_node_t * os_slist_next(const os_slist_node_t * node);

/*
* os_slist_getdata
* @brief  ��ȡ�ڵ������ָ��
* @param  node   �ڵ�ָ��
* @return �ڵ�����ָ�����NULL
*/
OS_API void * os_slist_getdata(const os_slist_node_t * node);

OS_API_END

#endif
