#include "os_rbt.h"

#include <string.h>
#include <malloc.h>

typedef enum _OS_RBT_COLOR_TYPE
{
	OS_RBT_COLOR_RED,
	OS_RBT_COLOR_BLACK
} OS_RBT_COLOR_TYPE;

struct _os_rbt_node_t {
	void * key;
	void * val;
	OS_RBT_COLOR_TYPE color;
	struct _os_rbt_node_t * parent;
	struct _os_rbt_node_t * left;
	struct _os_rbt_node_t * right;
};

struct _os_rbt_t {
	size_t size;
	os_rbt_node_t * root;
	os_rbt_node_t * nil;
	os_rbt_compare cmp;
	os_rbt_free key_free;
	os_rbt_free val_free;
};

// 创建一个节点
static os_rbt_node_t * os_rbt_new_node(const os_rbt_node_t * parent, const os_rbt_node_t * left, const os_rbt_node_t * right,
									   OS_RBT_COLOR_TYPE color, const void * key, const void * val);
// 左旋转
static void os_rbt_left_rotate(os_rbt_t * rbt);
// 右旋转
static void os_rbt_right_rotate(os_rbt_t * rbt);

os_rbt_t * os_rbt_create(os_rbt_compare cmp, os_rbt_free key, os_rbt_free val)
{
	os_rbt_t * rbt = (os_rbt_t *)calloc(1, sizeof(os_rbt_t));
	if (NULL == rbt)
		return NULL;

	rbt->cmp = cmp;
	rbt->key_free = key;
	rbt->val_free = val;

	return rbt;
}

void os_rbt_destroy(os_rbt_t ** rbt)
{

}

void os_rbt_clear(os_rbt_t * rbt)
{

}

bool os_rbt_insert(os_rbt_t * rbt, void * key, void * val)
{
	return true;
}

bool os_rbt_erase(os_rbt_t * rbt, const void * key)
{
	return true;
}

os_rbt_node_t * os_rbt_find(const os_rbt_t * rbt, const void * key)
{
	return NULL;
}

void * os_rbt_key(const os_rbt_node_t * node)
{
	return node ? node->key : NULL;
}

void * os_rbt_val(const os_rbt_node_t * node)
{
	return node ? node->val : NULL;
}

size_t os_rbt_size(const os_rbt_t * rbt)
{
	return rbt ? rbt->size : 0u;
}

bool os_rbt_empty(const os_rbt_t * rbt)
{
	return rbt ? 0 == rbt->size : true;
}

os_rbt_node_t * os_rbt_new_node(const os_rbt_node_t * parent, const os_rbt_node_t * left, const os_rbt_node_t * right,
								OS_RBT_COLOR_TYPE color, const void * key, const void * val)
{
	os_rbt_node_t * node = (os_rbt_node_t *)malloc(sizeof(os_rbt_node_t));
	if (NULL == node)
		return NULL;

	node->key = key;
	node->val = val;
	node->color = color;
	node->parent = parent;
	node->left = left;
	node->right = right;

	return node;
}

void os_rbt_left_rotate(os_rbt_t * rbt)
{

}

void os_rbt_right_rotate(os_rbt_t * rbt)
{

}
