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
	os_rbt_malloc key_malloc;
	os_rbt_malloc val_malloc;
	os_rbt_free key_free;
	os_rbt_free val_free;
};

// 创建一个节点
static os_rbt_node_t * os_rbt_new_node(const os_rbt_t * rbt, void * key, void * val);
// 左旋转
static void os_rbt_left_rotate(os_rbt_t * rbt, os_rbt_node_t * node);
// 右旋转
static void os_rbt_right_rotate(os_rbt_t * rbt, os_rbt_node_t * node);

os_rbt_t * os_rbt_create(os_rbt_compare cmp, os_rbt_malloc km, os_rbt_malloc vm, os_rbt_free kf, os_rbt_free vf)
{
	if (NULL == km || NULL == vm || NULL == kf || NULL == vf)
		return NULL;

	os_rbt_t * rbt = (os_rbt_t *)calloc(1, sizeof(os_rbt_t));
	if (NULL == rbt)
		return NULL;

	rbt->cmp = cmp;
	rbt->key_malloc = km;
	rbt->val_malloc = vm;
	rbt->key_free = kf;
	rbt->val_free = vf;

	rbt->nil = (os_rbt_t *)calloc(1, sizeof(os_rbt_node_t));
	if (NULL == rbt->nil) {
		free(rbt);
		return NULL;
	}

	rbt->nil = OS_RBT_COLOR_BLACK;
	rbt->root = rbt->nil;

	return rbt;
}

void os_rbt_destroy(os_rbt_t ** rbt)
{
	if (NULL == rbt || NULL == *rbt)
		return;

	os_rbt_clear(*rbt);
	free(*rbt);
	*rbt = NULL;
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

os_rbt_node_t * os_rbt_new_node(const os_rbt_t * rbt, void * key, void * val)
{
	os_rbt_node_t * node = (os_rbt_node_t *)malloc(sizeof(os_rbt_node_t));
	if (NULL == node)
		return NULL;

	node->key = rbt->key_malloc(key);
	node->val = rbt->val_malloc(val);
	node->color = OS_RBT_COLOR_RED;
	node->parent = rbt->nil;
	node->left = rbt->nil;
	node->right = rbt->nil;

	return node;
}

void os_rbt_left_rotate(os_rbt_t * rbt, os_rbt_node_t * node)
{
	os_rbt_node_t * right = node->right;
	node->right = right->left;

	if (right->left != rbt->nil)
		right->left->parent = node;
	
	right->parent = node->parent;

	if (node->parent == rbt->nil) // 无父节点
		rbt->root = right;
	else if (node == node->parent->left) // 父节点左孩子
		node->parent->left = right;
	else
		node->parent->right = right;  // 父节点右孩子

	right->left = node;
	node->parent = right;
}

void os_rbt_right_rotate(os_rbt_t * rbt, os_rbt_node_t * node)
{
	os_rbt_node_t * left = node->left;
	node->left = left->right;

	if (left->right != rbt->nil)
		left->right->parent = node;

	left->parent = node->parent;
	// 无父节点
	if (node->parent == rbt->nil)
		rbt->root = left;
	else if (node == node->parent->left)
		node->parent->left = left;
	else
		node->parent->right = left;

	left->right = node;
	node->parent = left;
}
