#include "inline.h"
#include "../test/test.h"


/*
 * local declarations
 */
static uint32_t test_memzero(void);
static uint32_t test_strprefix(void);


/**
 * Test the inline header.
 *   &returns: The number of failures.
 */
uint32_t inline_test()
{
	uint32_t nfails = 0;

	nfails += test_memzero();
	nfails += test_strprefix();

	return nfails;
}


/**
 * Test memory zeroing.
 *   &returns: The number of errors.
 */
static uint32_t test_memzero(void)
{
	uint32_t nfails = 0;
	uint64_t v = -1;

	memzero(&v, sizeof(uint64_t));
	check(v == 0, &nfails);

	return nfails;
}

/**
 * Test string prefix.
 *   &returns: The number of errors.
 */
static uint32_t test_strprefix(void)
{
	uint32_t nfails = 0;
	const char *str1 = "test string one";

	check(strprefix(str1, "test") == (str1 + 4), &nfails);
	check(strprefix(str1, "foo") == NULL, &nfails);
	check(strprefix(str1, str1) == (str1 + strlen(str1)), &nfails);

	return nfails;
}
