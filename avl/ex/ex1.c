#include "../avl.h"
#include <stdio.h>

int main()
{
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

	return 0;
}
