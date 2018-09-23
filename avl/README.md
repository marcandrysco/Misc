AVL Tree
========

Tables of Contents
  * 1. API
  * 1a. Tree Root
  * 1b. Tree Node
  * 1c. Comparison Functions
  * 2. Examples
  * 2a. Ex1: Raw Structures
  * 2b. Ex2: Roster the Hard Way
  * 2c. Ex3: Roster the Easy Way 


## 1. API



### 1a. Tree Root

The tree root structure tracks the root node and comparison function.

    struct avl_root_t avl_root_init(avl_compare_f compare);
    void avl_root_destroy(struct avl_root_t *root, ssize_t offset, avl_delete_f delete);

The `avl_root_init` function initializes a tree root with the comparison
function `compare`.

The `avl_root_destroy` function deletes all elements of a tree using the
deletion function `delete`. The `offset` is the offset from the node structure
to the reference root.

    struct avl_node_t *avl_root_first(struct avl_root_t *root);
    struct avl_node_t *avl_root_last(struct avl_root_t *root);

The `avl_root_first` and `avl_root_last` retrieves the first and last nodes
from the tree, respectively.

    struct avl_node_t *avl_root_get(struct avl_root_t *root, const void *ref);
    void avl_root_add(struct avl_root_t *root, struct avl_node_t *node);
    struct avl_node_t *avl_root_remove(struct avl_root_t *root, const void *ref);

The `avl_root_get` function finds a node matching the reference if found. If
`ref` is not found, the function returns `NULL`.

The `avl_root_add` function adds a node to the tree. A node may only be added
to a single tree, although multiple nodes with matching references can be
added to a tree.

The `avl_root_remove` function removes a node that matches the reference. If
no such node exists, the function returns `NULL`.


### 1b. Tree Node

    void avl_node_init(struct avl_node_t *node, const void *ref)

The `avl_node_init` function initializes the node with a given reference. The
reference is used on tree root operations for ordering nodes.

    struct avl_node_t *avl_node_prev(struct avl_node_t *node);
    struct avl_node_t *avl_node_next(struct avl_node_t *node);

The `avl_node_prev` and `avl_node_next` retrieves the previous and next nodes,
respectively.


### 1c. Comparison Functions

There are a set of comparison functions that may be passed to the root
initializer `avl_root_init`. These cover the basic types so you may need to
write your own comparison function for other types.

    int avl_cmp_ptr(const void *left, const void *right);
    int avl_cmp_str(const void *left, const void *right);

The `avl_cmp_ptr` compares the raw pointer values.

The `avl_cmp_str` compares two strings using the `strcmp` function.

    int avl_cmp_int(const void *left, const void *right);
    int avl_cmp_uint(const void *left, const void *right);
    int avl_cmp_char(const void *left, const void *right);
    int avl_cmp_short(const void *left, const void *right);
    int avl_cmp_long(const void *left, const void *right);
    int avl_cmp_size(const void *left, const void *right);
    int avl_cmp_i8(const void *left, const void *right);
    int avl_cmp_u8(const void *left, const void *right);
    int avl_cmp_i16(const void *left, const void *right);
    int avl_cmp_u16(const void *left, const void *right);
    int avl_cmp_i32(const void *left, const void *right);
    int avl_cmp_u32(const void *left, const void *right);
    int avl_cmp_i64(const void *left, const void *right);
    int avl_cmp_u64(const void *left, const void *right);
    int avl_cmp_float(const void *left, const void *right);
    int avl_cmp_double(const void *left, const void *right);

The above `avl_cmp_#TYPE#` functions compares values of a given type. The
`left` and `right` parameters are cast to the appropriate pointer type and
dereferences for comparison (e.g. `*(int *)left < *(int *)right`).


## 2. Examples

### 2a. Ex1: Raw Structures

Although not super useful, the AVL tree structures may be used to directly
create trees. Still, it serves as a good beginning example. The second example
will cover how to use the node structures embedded within another data
structure.

    // raw node structures
    struct avl_root_t root;
    struct avl_node_t node[3];
    
    // we are going to insert them, ordered by strings
    root = avl_root_init(avl_cmp_str);
    
    // insert all the nodes
    avl_node_init(&node[0], "foo");
    avl_root_add(&root, &node[0]);
    
    avl_node_init(&node[1], "bar");
    avl_root_add(&root, &node[1]);
    
    avl_node_init(&node[2], "baz");
    avl_root_add(&root, &node[2]);
    
    // first node is "bar"
    printf("%s\n", (char *)avl_root_first(&root)->ref);
    
    // last node is "baz"
    printf("%s\n", (char *)avl_root_last(&root)->ref);

It is important to node that the tree operates on pointers to nodes. This
means that the tree only remains valid while the `node` variables are on the
stack. After the function exists, they go out of scope and the pointers become
stale. For this reason, you typically want nodes to be on the heap so that
they can outlive the current functions. This is better covered in the next
example.

### 2b. Ex2: Roster the Hard Way


### 2c. Ex3: Roster the Easy Way
