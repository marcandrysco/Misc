#include "inline.h"


static void test_strprefix()
{
	const char *str1 = "test string one";

	assert(strprefix(str1, "test") == (str1 + 4));
	assert(strprefix(str1, "foo") == NULL);
	assert(strprefix(str1, str1) == (str1 + strlen(str1)));
}

/**
 * Test the inline header.
 */
void inline_test()
{
	test_strprefix();
}
