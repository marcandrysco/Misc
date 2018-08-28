#ifndef AVL_H
#define AVL_H

/**
 * Delete function.
 *   @ptr: The pointer.
 */
typedef void (*avl_delete_f)(void *ptr);

/**
 * Comparison function.
 *   @lhs: The left-hand side.
 *   @rhs: The right-hand side.
 *   &returns: Their order.
 */
typedef int (*avl_compare_f)(const void *lhs, const void *rhs);


/**
 * AVL tree root structure.
 *   @count: The number of nodes.
 *   @node: The root node.
 *   @compare: The comparison callback.
 */
struct avl_root_t {
	unsigned int count;
	struct avl_node_t *node;

	avl_compare_f compare;
};

/**
 * AVL tree node storage.
 *   @bal: The balance factor.
 *   @ref: The reference.
 *   @parent, left, right: The parent, left, and right children.
 */
struct avl_node_t {
	int_fast8_t bal;

	const void *ref;
	struct avl_root_t *root;
	struct avl_node_t *parent, *left, *right;
};

/*
 * avltree declarations
 */
struct avl_root_t avl_root_init(avl_compare_f compare);
void avl_root_destroy(struct avl_root_t *root, ssize_t offset, avl_delete_f delete);

struct avl_node_t *avl_root_first(struct avl_root_t *root);
struct avl_node_t *avl_root_last(struct avl_root_t *root);

struct avl_node_t *avl_node_prev(struct avl_node_t *node);
struct avl_node_t *avl_node_next(struct avl_node_t *node);

struct avl_node_t *avl_root_lookup(struct avl_root_t *root, const void *ref);
void avl_root_insert(struct avl_root_t *root, struct avl_node_t *ins);
struct avl_node_t *avl_root_remove(struct avl_root_t *root, const void *ref);

#endif
