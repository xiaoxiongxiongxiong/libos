#ifndef __OS_QUEUE_H__
#define __OS_QUEUE_H__

#include "libos.h"
#include <stdint.h>

typedef struct _os_queue_t os_queue_t;
typedef struct _os_queue_node_t os_queue_node_t;

OS_API_BEGIN

/*
* os_queue_create
* @brief  ��������
* @param  node_size  �ڵ��С
* @return ����ָ�����ΪNULL
*/
OS_API os_queue_t * os_queue_create(uint32_t node_size);

/*
* os_queue_destroy
* @brief  ���ٶ���
* @param  os_queue  ָ�����ָ���ָ��
*/
OS_API void os_queue_destroy(os_queue_t ** os_queue);

/*
* os_queue_clear
* @brief  ��ն���
* @param  os_queue ����ָ��
*/
OS_API void os_queue_clear(os_queue_t * os_queue);

/*
* os_queue_empty
* @brief  �ж϶����Ƿ�Ϊ��
* @param  os_queue  ����ָ��
* @return true--�ɹ� false--ʧ��
*/
OS_API bool os_queue_empty(os_queue_t * os_queue);

/*
* os_queue_size
* @brief  ��ȡ���г���
* @param  os_queue  ����ָ��
* @return ���г���
*/
OS_API size_t os_queue_size(os_queue_t * os_queue);

/*
* os_queue_push
* @brief  ��������
* @param  os_queue  ����ָ��
* @param  data  ����ָ��
* @return true--�ɹ� false--ʧ��
*/
OS_API bool os_queue_push(os_queue_t * os_queue, void * data);

/*
* os_queue_pop
* @brief  ɾ������ͷ
* @param  os_queue  ����ָ��
* @return true--�ɹ� false--ʧ��
*/
OS_API bool os_queue_pop(os_queue_t * os_queue);

/*
* os_queue_front
* @brief  ��ȡ����ͷ
* @param  os_queue  ����ָ��
* @return ͷ���ָ�����NULL
*/
OS_API os_queue_node_t * os_queue_front(os_queue_t * os_queue);

/*
* os_queue_getdata
* @brief  ��ȡ�ڵ�����
* @param  node    �ڵ�ָ��
* @return ����ָ��
*/
OS_API void * os_queue_getdata(const os_queue_node_t * node);

OS_API_END

#endif
