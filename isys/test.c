#ifdef WINDOWS
#  include "windows.h"
#else
#  include "posix.h"
#endif
#include "isys.h"

#include "../test/test.h"


/*
 * local declarations
 */
static uint32_t nfails;

static void *threadproc(void *arg);


/**
 * Test the Independent System.
 *   &returns: The number of failures..
 */
uint32_t isys_test(void)
{
	isys_thread_t thread;

	nfails = 0;

	thread = isys_thread_new(threadproc, (void *)12);
	check(isys_thread_join(thread) == (void *)31, &nfails);

	return nfails;
}

/**
 * Thread processing.
 *   @arg: The argument.
 *   &returns: The exit value.
 */
static void *threadproc(void *arg)
{
	check(arg == (void *)12, &nfails);

	return (void *)31;
}
