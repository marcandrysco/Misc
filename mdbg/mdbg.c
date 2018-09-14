#define MDBG_NO_RENAME 1

#include "mdbg.h"
#include <stdio.h>
#include <stdint.h>


/*
 * memory counter variable
 */
int mdbg_cnt = 0;


/**
 * Check for any memory leaks.
 */
void mdbg_check(void)
{
	if(mdbg_cnt != 0) {
		fprintf(stderr, "missed %d allocations\n", mdbg_cnt);
		abort();
	}
}


void *mdbg_test_malloc(size_t size)
{
	mdbg_cnt++;

	return malloc(size);
}


void *mdbg_debug_malloc(size_t size)
{
	void *ptr;

	ptr = mdbg_test_malloc(32 + size);

	*(uint64_t *)(ptr + 0) = (uint64_t)(ptr + 0) * 4221379234814892313ul;
	*(uint64_t *)(ptr + 8) = (uint64_t)(ptr + 8) * 4221379234814892313ul;

	return ptr + 16;
}
