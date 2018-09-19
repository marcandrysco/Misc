AVL Trees
=========

Tables of Contents
  * 1. API
  * 1a. Tree Root
  * 1b. Tree Node
  * 2. Examples
  * 2a. Raw Structures
  * 2b. 


## 1. API

All

### 1a. Tree Root

    struct avl_root_t avl_root_init(avl_compare_f compare);
    void avl_root_destroy(struct avl_root_t *root, ssize_t offset, avl_delete_f delete);

The `avl_root_init` function initializes a tree root with the comparison
function `compare`.

The `avl_root_destory` function deletes all elements of a tree using the
deletion function `delete`. The `offset` is the offset from the node structure
to the reference root.

    struct avl_node_t *avl_root_first(struct avl_root_t *root);
    struct avl_node_t *avl_root_last(struct avl_root_t *root);

The `avl_root_first` and `avl_root_last` retrieves the first and last nodes
from the tree, respectively.

### 2b. Tree Node


## 2. Examples

### 2a. Ex1: Raw Structures

### 2b. Ex2: 
