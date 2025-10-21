#include "os_rbt.h"

#include <string.h>
#include <malloc.h>

typedef enum _OS_RBT_COLOR_TYPE
{
	OS_RBT_COLOR_RED,
	OS_RBT_COLOR_BLACK
} OS_RBT_COLOR_TYPE;

struct _os_rbt_node_t {
	OS_RBT_COLOR_TYPE color;
	struct _os_rbt_node_t * parent;
	struct _os_rbt_node_t * left;
	struct _os_rbt_node_t * right;
	char data[0];
};

struct _os_rbt_t {
	size_t size;
	size_t elem_size;
	size_t node_size;
	os_rbt_node_t * root;
	os_rbt_node_t * nil;
	os_rbt_compare cmp;
};

// 创建一个节点
static os_rbt_node_t * os_rbt_new_node(const os_rbt_t * rbt, void * data);
// 左旋转
static void os_rbt_left_rotate(os_rbt_t * rbt, os_rbt_node_t * node);
// 右旋转
static void os_rbt_right_rotate(os_rbt_t * rbt, os_rbt_node_t * node);
// 插入后修复
static void os_rbt_insert_fixup(os_rbt_t * rbt, os_rbt_node_t * node);

os_rbt_t * os_rbt_create(size_t elem_size, os_rbt_compare cmp)
{
	if (0u == elem_size || NULL == cmp)
		return NULL;

	os_rbt_t * rbt = (os_rbt_t *)calloc(1, sizeof(os_rbt_t));
	if (NULL == rbt)
		return NULL;

	rbt->elem_size = elem_size;
	rbt->node_size = sizeof(os_rbt_node_t) + elem_size;
	rbt->cmp = cmp ? cmp : memcmp;

	rbt->nil = (os_rbt_node_t *)calloc(1, sizeof(os_rbt_node_t));
	if (NULL == rbt->nil) {
		free(rbt);
		return NULL;
	}

	rbt->nil->color = OS_RBT_COLOR_BLACK;
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

bool os_rbt_insert(os_rbt_t * rbt, void * data)
{
	if (NULL == rbt || NULL == data)
		return false;

	os_rbt_node_t * tmp = rbt->nil;
	os_rbt_node_t * root = rbt->root;
	while (rbt->nil != root) {
		tmp = root;
		int ret = rbt->cmp(root->data, data, rbt->elem_size);
		if (ret < 0)
			root = root->right;
		else if (ret > 0)
			root = root->left;
		else
			return true;
	}

	os_rbt_node_t * node = os_rbt_new_node(rbt, data);
	if (NULL == node)
		return false;

	node->parent = tmp;
	int ret = rbt->cmp(tmp->data, data, rbt->elem_size);
	if (rbt->nil == tmp)
		rbt->root = node;
	else if (ret < 0)
		tmp->right = node;
	else
		tmp->left = node;

	os_rbt_insert_fixup(rbt, node);

	++rbt->size;

	return true;
}

bool os_rbt_erase(os_rbt_t * rbt, const void * data)
{
	return true;
}

os_rbt_node_t * os_rbt_find(const os_rbt_t * rbt, const void * data)
{
	if (NULL == rbt || NULL == data)
		return NULL;

	os_rbt_node_t * node = rbt->root;
	while (rbt->nil != node) {
		int ret = rbt->cmp(node->data, data, rbt->elem_size);
		if (ret < 0)
			node = node->right;
		else if (ret > 0)
			node = node->left;
		else
			return node;
	}

	return NULL;
}

void * os_rbt_data(const os_rbt_node_t * node)
{
	return node ? (void *)node->data : NULL;
}

size_t os_rbt_size(const os_rbt_t * rbt)
{
	return rbt ? rbt->size : 0u;
}

bool os_rbt_empty(const os_rbt_t * rbt)
{
	return rbt ? 0 == rbt->size : true;
}

os_rbt_node_t * os_rbt_new_node(const os_rbt_t * rbt, void * data)
{
	os_rbt_node_t * node = (os_rbt_node_t *)malloc(rbt->node_size);
	if (NULL == node)
		return NULL;

	memcpy(node->data, data, rbt->elem_size);
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

void os_rbt_insert_fixup(os_rbt_t * rbt, os_rbt_node_t * node)
{
}
