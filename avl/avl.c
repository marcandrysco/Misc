#include "avl.h"


/*
 * local declarations
 */
static void node_destroy(struct avl_node_t *node, ssize_t offset, avl_delete_f delete);

static struct avl_node_t *node_first(struct avl_node_t *node);
static struct avl_node_t *node_last(struct avl_node_t *node);

static struct avl_node_t *rotate_right(struct avl_node_t *node);
static struct avl_node_t *rotate_left(struct avl_node_t *node);

static struct avl_node_t **getref(struct avl_node_t *node, struct avl_root_t *root);
static int_fast8_t getdir(struct avl_node_t *node);


/**
 * Create an empty AVL tree root.
 *   @compare: The comparison.
 *   &returns: The empty root.
 */
struct avl_root_t avl_root_init(avl_compare_f compare)
{
	return (struct avl_root_t){ 0, NULL, compare };
}

/**
 * Destroy an AVL tree root.
 *   @root: The root.
 *   @offset; The offset.
 *   @delete: Deletion callack.
 */
void avl_root_destroy(struct avl_root_t *root, ssize_t offset, avl_delete_f delete)
{
	node_destroy(root->node, offset, delete);
}
static void node_destroy(struct avl_node_t *node, ssize_t offset, avl_delete_f delete)
{
	if(node == NULL)
		return;

	node_destroy(node->left, offset, delete);
	node_destroy(node->right, offset, delete);
	delete((void *)node - offset);
}


/**
 * Retrieve the first node from an AVL tree root.
 *   @root: The root.
 *   &returns: The first node or null.
 */
struct avl_node_t *avl_root_first(struct avl_root_t *root)
{
	return root->node ? node_first(root->node) : NULL;
}
static struct avl_node_t *node_first(struct avl_node_t *node)
{
	while(node->left != NULL)
		node = node->left;

	return node;
}

/**
 * Retrieve the last node from an AVL tree root.
 *   @root: The root.
 *   &returns: The last node or null.
 */
struct avl_node_t *avl_root_last(struct avl_root_t *root)
{
	return root->node ? node_last(root->node) : NULL;
}
static struct avl_node_t *node_last(struct avl_node_t *node)
{
	while(node->right != NULL)
		node = node->right;

	return node;
}


/**
 * Lookup a node by reference.
 *   @root; The root.
 *   @ref: The reference.
 *   &returns: The node or null.
 */
struct avl_node_t *avl_root_lookup(struct avl_root_t *root, const void *ref)
{
	int cmp;
	struct avl_node_t *node;

	node = root->node;
	while(node != NULL) {
		cmp = root->compare(node->ref, ref);
		if(cmp < 0)
			node = node->right;
		else if(cmp > 0)
			node = node->left;
		else
			return node;
	}

	return NULL;
}


/**
 * Insert a node onto the root.
 *   @root; The root.
 *   @node: The node.
 */
void avl_root_insert(struct avl_root_t *root, struct avl_node_t *ins)
{
	int cmp;
	struct avl_node_t **cur, *node, *parent = NULL;

	cur = &root->node;

	while(*cur != NULL) {
		parent = *cur;
		cmp = root->compare((*cur)->ref, ins->ref);
		if(cmp <= 0)
			cur = &(*cur)->right;
		else
			cur = &(*cur)->left;
	}

	*cur = ins;
	ins->bal = 0;
	ins->parent = parent;
	ins->left = ins->right = NULL;
	node = ins;
	root->count++;

	while((parent = node->parent) != NULL) {
		parent->bal += getdir(node);
		node = node->parent;
		if(node->bal > 1) {
			if(node->right->bal == -1)
				node->right = rotate_right(node->right);

			node = *getref(node, root) = rotate_left(node);
			break;
		}
		else if(node->bal < -1) {
			if(node->left->bal == 1)
				node->left = rotate_left(node->left);

			node = *getref(node, root) = rotate_right(node);
			break;
		}
		else if(node->bal == 0)
			break;
	}
}

/**
 * Remove a node from the root.
 *   @root; The root.
 *   @ref: The reference.
 *   &returns: The node.
 *   @node: The removed node if the reference is found.
 */
struct avl_node_t *avl_root_remove(struct avl_root_t *root, const void *ref)
{
	int_fast8_t bal;
	struct avl_node_t *rem, *node;

	rem = avl_root_lookup(root, ref);
	if(rem == NULL)
		return NULL;

	if(rem->left != NULL)
		node = node_last(rem->left), bal = getdir(node);
	else if(rem->right != NULL)
		node = node_first(rem->right), bal = getdir(node);
	else
		node = NULL, bal = rem->parent ? getdir(rem) : 0;

	root->count--;
	*getref(rem, root) = node;

	if(node != NULL) {
		struct avl_node_t *parent;

		if(node->left) {
			node->left->parent = node->parent;
			*getref(node, root) = node->left;
		}
		else if(node->right) {
			node->left->parent = node->parent;
			*getref(node, root) = node->left;
		}
		else {
			*getref(node, root) = NULL;
		}
		parent = (node->parent == rem) ? node : node->parent;

		node->bal = rem->bal;
		node->parent = rem->parent;
		node->left = rem->left;
		node->right = rem->right;

		if(node->left != NULL)
			node->left->parent = node;

		if(node->right != NULL)
			node->right->parent = node;

		node = parent;
	}
	else
		node = rem->parent;

	if(node != NULL) {
		while(true) {
			node->bal -= bal;

			if(node->bal > 1) {
				if(node->right->bal == -1)
					node->right = rotate_right(node->right);

				node = *getref(node, root) = rotate_left(node);

			}
			else if(node->bal < -1) {
				if(node->left->bal == 1)
					node->left = rotate_left(node->left);

				node = *getref(node, root) = rotate_right(node);

			}

			if(node->bal != 0)
				break;

			if(node->parent == NULL)
				break;

			bal = getdir(node);
			node = node->parent;
		}
	}

	return rem;
}


/**
 * Retrieve the previous node.
 *   @node: The current node.
 *   &returns: The previous node or null.
 */
struct avl_node_t *avl_node_prev(struct avl_node_t *node)
{
	if(node->left != NULL) {
		node = node->left;

		while(node->right != NULL)
			node = node->right;

		return node;
	}
	else {
		while(node->parent != NULL) {
			if(node->parent->right == node)
				break;

			node = node->parent;
		}

		return node->parent;
	}
}

/**
 * Retrieve the next node.
 *   @node: The current node.
 *   &returns: The next node or null.
 */
struct avl_node_t *avl_node_next(struct avl_node_t *node)
{
	if(node->right != NULL) {
		node = node->right;

		while(node->left != NULL)
			node = node->left;

		return node;
	}
	else {
		while(node->parent != NULL) {
			if(node->parent->left == node)
				break;

			node = node->parent;
		}

		return node->parent;
	}
}


/**
 * Rotate right on a given node.
 *   @node: The base node.
 *   &returns: The new base node.
 */
static struct avl_node_t *rotate_right(struct avl_node_t *node)
{
	struct avl_node_t *tmp;

	tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;

	node->bal++;
	if(tmp->bal < 0)
		node->bal -= tmp->bal;

	tmp->bal++;
	if(node->bal > 0)
		tmp->bal += node->bal;

	tmp->parent = node->parent;
	node->parent = tmp;

	if(node->left != NULL)
		node->left->parent = node;

	return tmp;
}

/**
 * Rotate left on a given node.
 *   @node; The base node.
 *   &returns: The new base node.
 */
static struct avl_node_t *rotate_left(struct avl_node_t *node)
{
	struct avl_node_t *tmp;

	tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;

	node->bal--;
	if(tmp->bal > 0)
		node->bal -= tmp->bal;

	tmp->bal--;
	if(node->bal < 0)
		tmp->bal += node->bal;

	tmp->parent = node->parent;
	node->parent = tmp;

	if(node->right != NULL)
		node->right->parent = node;

	return tmp;
}


/**
 * Get a reference to the node from either the parent or root.
 *   @node: The node.
 *   @root: The root.
 *   &returns: The reference.
 */
static struct avl_node_t **getref(struct avl_node_t *node, struct avl_root_t *root)
{
	if(node->parent == NULL)
		return &root->node;
	else if(node->parent->left == node)
		return &node->parent->left;
	else
		return &node->parent->right;
}

/**
 * Retrieve the direction of a node relative to its parent.
 *   @node: The node.
 *   &returns: The direction as '1' or '-1'.
 */
static int_fast8_t getdir(struct avl_node_t *node)
{
	return (node->parent->right == node) ? 1 : -1;
}
