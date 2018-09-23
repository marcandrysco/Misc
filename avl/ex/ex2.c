#include "../avl.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// the user structure contains the id (used for sorting), the name, and an
// embedded copy of the node structure
struct user_t {
	uint32_t id;
	char *name;

	struct avl_node_t node;
};

// the `user_new` function is used to conveniently create new people; notice
// that the node is initialized with a pointer to the identifier, and it must
// be `&user->id` since that pointer lives in the user; using `&id`
// consumes a pointer to the stack variable `id` and will no longer be valid
// after the function retruns
struct user_t *user_new(uint32_t id, const char *name)
{
	struct user_t *user;

	user = malloc(sizeof(struct user_t));
	user->id = id;
	user->name = strdup(name);

	avl_node_init(&user->node, &user->id);

	return user;
}

// the `user_delete` function completely deletes the user, including name
void user_delete(struct user_t *user)
{
	free(user->name);
	free(user);
}



int main()
{
	// root contains the tree of all people, and user is used as a
	// temporary
	struct avl_root_t root;
	struct user_t *user;

	// we order using the id, a 32-bit unsigned integer (remember, it uses
	// pointers to 32-bit integer, not integers directly)
	root = avl_root_init(avl_cmp_u32);

	// add some people to the tree root
	user = user_new(1, "john");
	avl_root_add(&root, &user->node);

	user = user_new(4, "jane");
	avl_root_add(&root, &user->node);

	user = user_new(2, "alice");
	avl_root_add(&root, &user->node);

	user = user_new(3, "bob");
	avl_root_add(&root, &user->node);


	// lookup a user by id, but don't forget we need a pointer to the id
	uint32_t id;
	struct avl_node_t *node;

	id = 3;
	node = avl_root_get(&root, &id);
	if(node != NULL) {
		//printf("found user '%s'\n", 
	}
	else
		printf("user not found\n");

	return 0;
}
