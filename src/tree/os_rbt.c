#include "os_rbt.h"

#include <string.h>
#include <malloc.h>

typedef enum _OS_RBT_COLOR_TYPE {
	OS_RBT_COLOR_RED,
	OS_RBT_COLOR_BLACK
} OS_RBT_COLOR_TYPE;

typedef enum _OS_RBT_TRAVERSAL_TYPE
{
	OS_RBT_PREORDER_TRAVERSAL,   // 先序遍历
	OS_RBT_INORDER_TRAVERSAL,    // 中序遍历
	OS_RBT_POSTORDER_TRAVERSAL,  // 后序遍历
} OS_RBT_TRAVERSAL_TYPE;

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
// 后序遍历删除子树
static void os_rbt_erase_subtree(os_rbt_t * rbt, os_rbt_node_t * node);
// 左旋转
static void os_rbt_left_rotate(os_rbt_t * rbt, os_rbt_node_t * node);
// 右旋转
static void os_rbt_right_rotate(os_rbt_t * rbt, os_rbt_node_t * node);
// 插入后修复
static void os_rbt_insert_fixup(os_rbt_t * rbt, os_rbt_node_t * node);
// 节点互换父辈
static void os_rbt_replace(os_rbt_t * rbt, os_rbt_node_t * src, os_rbt_node_t * dst);
// 获取最小节点
static os_rbt_node_t * os_rbt_min(os_rbt_t * rbt, os_rbt_node_t * node);
// 删除后修复
static void os_rbt_delete_fixup(os_rbt_t * rbt, os_rbt_node_t * node);
// 遍历
static void os_rbt_traversal(const os_rbt_t * rbt, os_rbt_node_t * node, 
							 enum OS_RBT_TRAVERSAL_TYPE tt, os_rbt_traversal_cb cb, void * user_data);

os_rbt_t * os_rbt_create(size_t elem_size, os_rbt_compare cmp)
{
	if (0u == elem_size)
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

	free((*rbt)->nil);
	(*rbt)->nil = NULL;

	free(*rbt);
	*rbt = NULL;
}

void os_rbt_clear(os_rbt_t * rbt)
{
	if (NULL == rbt || 0u == rbt->size)
		return;

	os_rbt_erase_subtree(rbt, rbt->root);
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

    ++rbt->size;
	node->parent = tmp;
	if (rbt->nil == tmp) {
		rbt->root = node;
		rbt->root->color = OS_RBT_COLOR_BLACK;
		return true;
	}

	if (rbt->cmp(tmp->data, data, rbt->elem_size) < 0)
		tmp->right = node;
	else
		tmp->left = node;

	os_rbt_insert_fixup(rbt, node);

	return true;
}

bool os_rbt_erase(os_rbt_t * rbt, const void * data)
{
	os_rbt_node_t * node = os_rbt_find(rbt, data);
	if (NULL == node)
		return false;

	os_rbt_node_t * tmp = NULL;
	os_rbt_node_t * dst = node;
	OS_RBT_COLOR_TYPE node_color = node->color;

    if (rbt->nil == node->left) { 	// 只有右孩子
		tmp = node->right;
		os_rbt_replace(rbt, node, node->right);
	} else if (rbt->nil == node->right) { // 只有左孩子
		tmp = node->left;
		os_rbt_replace(rbt, node, node->left);
	} else {
		dst = os_rbt_min(rbt, node->right);
		tmp = dst->right;
		node_color = dst->color;
		if (dst->parent == node)
			tmp->parent = dst; // 针对tmp为NIL情况时(dst无子节点)在fixup中仍可用
		else { // 此种情况下只可能存在右孩子
			os_rbt_replace(rbt, dst, dst->right);
			dst->right = node->right;
			dst->right->parent = dst;
		}
		os_rbt_replace(rbt, node, dst);
		dst->left = node->left;
		dst->left->parent = dst;
		dst->color = node->color;
	}

	free(node);

	if (OS_RBT_COLOR_BLACK == node_color)
		os_rbt_delete_fixup(rbt, tmp);
	rbt->size--;

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

void os_rbt_preorder_traversel(const os_rbt_t * rbt, os_rbt_traversal_cb cb, void * user_data)
{
	if (NULL == rbt || NULL == cb)
		return;

	os_rbt_traversal(rbt, rbt->root, OS_RBT_PREORDER_TRAVERSAL, cb, user_data);
}

void os_rbt_inorder_traversel(const os_rbt_t * rbt, os_rbt_traversal_cb cb, void * user_data)
{
    if (NULL == rbt || NULL == cb)
        return;

	os_rbt_traversal(rbt, rbt->root, OS_RBT_INORDER_TRAVERSAL, cb, user_data);
}

void os_rbt_postorder_traversel(const os_rbt_t * rbt, os_rbt_traversal_cb cb, void * user_data)
{
    if (NULL == rbt || NULL == cb)
        return;

    os_rbt_traversal(rbt, rbt->root, OS_RBT_POSTORDER_TRAVERSAL, cb, user_data);
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

void os_rbt_erase_subtree(os_rbt_t * rbt, os_rbt_node_t * node)
{
	if (node == rbt->nil)
		return;

	os_rbt_erase_subtree(rbt, node->left);
	os_rbt_erase_subtree(rbt, node->right);
    free(node);
	rbt->size--;
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

    if (node->parent == rbt->nil) 	// 无父节点
		rbt->root = left;
	else if (node == node->parent->left)  // 父节点左孩子
		node->parent->left = left;
	else
		node->parent->right = left;  // 父节点右孩子

	left->right = node;
	node->parent = left;
}

void os_rbt_insert_fixup(os_rbt_t * rbt, os_rbt_node_t * node)
{
	while (OS_RBT_COLOR_RED == node->parent->color) {
		os_rbt_node_t * tmp = node->parent->parent;
		if (node->parent == tmp->left) {
			os_rbt_node_t * uncle = tmp->right;
			if (OS_RBT_COLOR_RED == uncle->color) { // 叔叔节点红色
				tmp->color = OS_RBT_COLOR_RED;
				node->parent->color = OS_RBT_COLOR_BLACK;
				uncle->color = OS_RBT_COLOR_BLACK;
				node = tmp;
			} else {
				// 叔叔是黑色，且为右孩子 LR型
				if (node == node->parent->right) {
					node = node->parent;
					os_rbt_left_rotate(rbt, node);
				}
				// 叔叔是黑色，且为左孩子
                os_rbt_right_rotate(rbt, tmp);
                node->parent->color = OS_RBT_COLOR_BLACK;
                tmp->color = OS_RBT_COLOR_RED;
			}
		} else {
			os_rbt_node_t * uncle = tmp->left;
			if (OS_RBT_COLOR_RED == uncle->color) {
				tmp->color = OS_RBT_COLOR_RED;
				node->parent->color = OS_RBT_COLOR_BLACK;
				uncle->color = OS_RBT_COLOR_BLACK;
				node = tmp;
			} else {
				// 叔叔是黑色,且为左孩子 RL型
				if (node == node->parent->left) {
					node = node->parent;
					os_rbt_right_rotate(rbt, node);
				}
				// 叔叔是黑色，且为右孩子
				os_rbt_left_rotate(rbt, tmp);
				node->parent->color = OS_RBT_COLOR_BLACK;
				tmp->color = OS_RBT_COLOR_RED;
			}
		}
    }
	rbt->root->color = OS_RBT_COLOR_BLACK;
}

void os_rbt_replace(os_rbt_t * rbt, os_rbt_node_t * src, os_rbt_node_t * dst)
{
	// 根节点
	if (rbt->nil == src->parent)
		rbt->root = dst;
	else if (src->parent->left == src)
		src->parent->left = dst;
	else
		src->parent->right = dst;
	dst->parent = src->parent;
}

os_rbt_node_t * os_rbt_min(os_rbt_t * rbt, os_rbt_node_t * node)
{
	while (node->left != rbt->nil) {
		node = node->left;
	}
	return node;
}

void os_rbt_delete_fixup(os_rbt_t * rbt, os_rbt_node_t * node)
{
	while (rbt->root != node && OS_RBT_COLOR_BLACK == node->color) {
		if (node == node->parent->left) { // 左节点
			os_rbt_node_t * brother = node->parent->right; // 兄弟节点
			if (OS_RBT_COLOR_RED == brother->color) {
                brother->color = OS_RBT_COLOR_BLACK;
				node->parent->color = OS_RBT_COLOR_RED;
				os_rbt_left_rotate(rbt, node->parent);
				brother = node->parent->right;
			}

			// 兄弟节点至少一个红孩
			if (OS_RBT_COLOR_RED == brother->left->color || OS_RBT_COLOR_RED == brother->right->color) {
				// 左孩红，右孩黑
				if (OS_RBT_COLOR_BLACK == brother->right->color) {
					brother->left->color = OS_RBT_COLOR_BLACK;
					brother->color = OS_RBT_COLOR_RED;
					os_rbt_right_rotate(rbt, brother);
					brother = node->parent->right;
				}

				brother->color = node->parent->color;
				node->parent->color = OS_RBT_COLOR_BLACK;
				brother->right->color = OS_RBT_COLOR_BLACK;
				os_rbt_left_rotate(rbt, node->parent);
				node = rbt->root;
			} else {
				brother->color = OS_RBT_COLOR_RED;
				node = node->parent;
			}
		} else {
			os_rbt_node_t * brother = node->parent->left;
			if (OS_RBT_COLOR_RED == brother->color) {
				brother->color = OS_RBT_COLOR_BLACK;
				node->parent->color = OS_RBT_COLOR_RED;
				os_rbt_right_rotate(rbt, node->parent);
				brother = node->parent->left;
			}

			if (OS_RBT_COLOR_RED == brother->left->color || OS_RBT_COLOR_RED == brother->right->color) {
				if (OS_RBT_COLOR_BLACK == brother->left->color) {
					brother->right->color = OS_RBT_COLOR_BLACK;
					brother->color = OS_RBT_COLOR_RED;
					os_rbt_left_rotate(rbt, brother);
					brother = node->parent->left;
				}
				brother->color = node->parent->color;
				node->parent->color = OS_RBT_COLOR_BLACK;
				brother->left->color = OS_RBT_COLOR_BLACK;
				os_rbt_right_rotate(rbt, node->parent);
				node = rbt->root;
			} else {
				brother->color = OS_RBT_COLOR_RED;
				node = node->parent;
			}
		}
	}
	node->color = OS_RBT_COLOR_BLACK;
}

void os_rbt_traversal(const os_rbt_t * rbt, os_rbt_node_t * node,
                      enum OS_RBT_TRAVERSAL_TYPE tt, os_rbt_traversal_cb cb, void * user_data)
{
    if (node == rbt->nil)
        return;

	switch (tt)
	{
	case OS_RBT_PREORDER_TRAVERSAL:
    {
        cb(user_data, node->data);
        os_rbt_traversal(rbt, node->left, tt, cb, user_data);
        os_rbt_traversal(rbt, node->right, tt, cb, user_data);
    }
		break;
	case OS_RBT_INORDER_TRAVERSAL:
    {
        os_rbt_traversal(rbt, node->left, tt, cb, user_data);
        cb(user_data, node->data);
        os_rbt_traversal(rbt, node->right, tt, cb, user_data);
    }
		break;
	case OS_RBT_POSTORDER_TRAVERSAL:
	{
        os_rbt_traversal(rbt, node->left, tt, cb, user_data);
        os_rbt_traversal(rbt, node->right, tt, cb, user_data);
        cb(user_data, node->data);
	}
		break;
	default:
		break;
	}
}
