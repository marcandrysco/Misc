#define MDBG_NO_RENAME 1

#include "mdbg.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>


void chk(bool val)
{
	if(!val) {
		fprintf(stderr, "heap corruption\n");
		abort();
	}
}


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
#ifndef RELEASE
		abort();
#endif
	}
}


/**
 * Base version of malloc.
 *   @size: The allocation size.
 *   &returns: The allocated pointer.
 */
void *mdbg_malloc(size_t size)
{
#ifdef RELEASE
	return mdbg_malloc_release(size);
#elif TEST
	return mdbg_malloc_test(size);
#elif DEBUG
	return mdbg_malloc_debug(size);
#else
#	error "Must define one of 'RELEASE', 'TEST', or 'DEBUG'."
#endif
}

/**
 * Release version of malloc.
 *   @size: The allocation size.
 *   &returns: The allocated pointer.
 */
void *mdbg_malloc_release(size_t size)
{
	void *ptr;

	ptr = malloc(size ?: 1);
	if(ptr == NULL) {
		fprintf(stderr, "memory allocation failed\n");
		abort();
	}

	return ptr;
}

/**
 * Test version of malloc.
 *   @size: The allocation size.
 *   &returns: The allocated pointer.
 */
void *mdbg_malloc_test(size_t size)
{
	mdbg_cnt++;

	return mdbg_malloc_release(size);
}

/**
 * Debug version of malloc.
 *   @size: The allocation size.
 *   &returns: The allocated pointer.
 */
void *mdbg_malloc_debug(size_t size)
{
	void *ptr;

	ptr = mdbg_malloc_test(32 + size);

	*(uint64_t *)(ptr + 0) = (uint64_t)size;
	*(uint64_t *)(ptr + 8) = (uint64_t)(ptr + 8) * 4221379234814892313ul;
	*(uint64_t *)(ptr + 16 + size + 0) = (uint64_t)(ptr + 16 + 0) * 4221379234814892313ul;
	*(uint64_t *)(ptr + 16 + size + 8) = (uint64_t)(ptr + 16 + 8) * 4221379234814892313ul;

	return ptr + 16;
}


/**
 * Base version of realloc.
 *   @ptr: The pointer.
 *   @size: The size.
 *   &returns: The reallocated pointer.
 */
void *mdbg_realloc(void *ptr, size_t size)
{
#ifdef RELEASE
	return mdbg_realloc_release(ptr, size);
#elif TEST
	return mdbg_realloc_test(ptr, size);
#elif DEBUG
	return mdbg_realloc_debug(ptr, size);
#else
#	error "Must define one of 'RELEASE', 'TEST', or 'DEBUG'."
#endif
}

/**
 * Release version of free.
 *   @ptr: The pointer.
 *   @size: The size.
 *   &returns: The reallocated pointer.
 */
void *mdbg_realloc_release(void *ptr, size_t size)
{
	if(ptr == NULL) {
		fprintf(stderr, "attempted to realloc null pointer\n");
		abort();
	}

	return realloc(ptr, size);
}

/**
 * Test version of realloc.
 *   @ptr: The pointer.
 *   @size: The size.
 *   &returns: The reallocated pointer.
 */
void *mdbg_realloc_test(void *ptr, size_t size)
{
	return mdbg_realloc_release(ptr, size);
}

/**
 * Debug version of realloc.
 *   @ptr: The pointer.
 *   @size: The size.
 *   &returns: The reallocated pointer.
 */
void *mdbg_realloc_debug(void *ptr, size_t size)
{
	size_t chksize;

	ptr -= 16;

	chksize = *(uint64_t *)ptr;
	chk(*(uint64_t *)(ptr + 8) == (uint64_t)(ptr + 8) * 4221379234814892313ul);
	chk(*(uint64_t *)(ptr + 16 + chksize + 0) == (uint64_t)(ptr + 16 + 0) * 4221379234814892313ul);
	chk(*(uint64_t *)(ptr + 16 + chksize + 8) == (uint64_t)(ptr + 16 + 8) * 4221379234814892313ul);

	ptr = mdbg_realloc_test(ptr, 32 + size);

	*(uint64_t *)(ptr + 0) = (uint64_t)size;
	*(uint64_t *)(ptr + 8) = (uint64_t)(ptr + 8) * 4221379234814892313ul;
	*(uint64_t *)(ptr + 16 + size + 0) = (uint64_t)(ptr + 16 + 0) * 4221379234814892313ul;
	*(uint64_t *)(ptr + 16 + size + 8) = (uint64_t)(ptr + 16 + 8) * 4221379234814892313ul;

	return ptr + 16;
}


/**
 * Base version of free.
 *   @ptr: The pointer.
 */
void mdbg_free(void *ptr)
{
#ifdef RELEASE
	return mdbg_free_release(ptr);
#elif TEST
	return mdbg_free_test(ptr);
#elif DEBUG
	return mdbg_free_debug(ptr);
#else
#	error "Must define one of 'RELEASE', 'TEST', or 'DEBUG'."
#endif
}

/**
 * Release version of free.
 *   @ptr: The pointer.
 */
void mdbg_free_release(void *ptr)
{
	if(ptr == NULL) {
		fprintf(stderr, "attempted to free null pointer\n");
		abort();
	}

	free(ptr);
}

/**
 * Test version of free.
 *   @ptr: The pointer.
 */
void mdbg_free_test(void *ptr)
{
	mdbg_free_release(ptr);
	mdbg_cnt--;
}

/**
 * Debug version of free.
 *   @ptr: The pointer.
 */
void mdbg_free_debug(void *ptr)
{
	size_t size;

	ptr -= 16;

	size = *(uint64_t *)ptr;
	chk(*(uint64_t *)(ptr + 8) == (uint64_t)(ptr + 8) * 4221379234814892313ul);
	chk(*(uint64_t *)(ptr + 16 + size + 0) == (uint64_t)(ptr + 16 + 0) * 4221379234814892313ul);
	chk(*(uint64_t *)(ptr + 16 + size + 8) == (uint64_t)(ptr + 16 + 8) * 4221379234814892313ul);

	mdbg_free_test(ptr);
}


/**
 * Run a printf call directly to an allocated string.
 *   @fmt; The printf format.
 *   @...: The printf arguments.
 *   &returns: The allocated string.
 */
char *mdbg_mprintf(const char *restrict fmt, ...)
{
	char *str;
	va_list args;

	va_start(args, fmt);
	str = mdbg_vmprintf(fmt, args);
	va_end(args);

	return str;
}

/**
 * Run a vprintf call directly writing to an allocated string.
 *   @fmt; The printf format.
 *   @args: The printf arguments.
 *   &returns: The allocated string.
 */
char *mdbg_vmprintf(const char *restrict fmt, va_list args)
{
	char *str;
	size_t len;
	va_list copy;

	va_copy(copy, args);
	len = vsnprintf(NULL, 0, fmt, args);
	va_end(copy);

	str = malloc(len + 1);
	vsprintf(str, fmt, args);

	return str;
}

/**
 * Terminate a program with a message.
 *   @fmt; The printf format.
 *   @...: The printf arguments.
 *   &noreturn
 */
__attribute__((noreturn)) void mdbg_fatal(const char *restrict fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	fprintf(stderr, "\n");
	va_end(args);

	abort();
}
