#ifndef MDBG_H
#define MDBG_H

/*
 * required headers
 */
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * external declarations
 */
extern int mdbg_cnt;


/**
 * Allocate memory.
 *   @size: The size.
 *   &returns: The allocation.
 */
static inline void *mdbg_alloc(size_t size)
{
	void *ptr;

#ifndef RELEASE
	mdbg_cnt++;
#endif

	ptr = malloc(size ?: 1);
	if(ptr == NULL)
		fprintf(stderr, "allocation failed, %s\n", strerror(errno));

	return ptr;
}

/**
 * Reallocate memory.
 *   @ptr: The pointer.
 *   @size: The size.
 *   &returns: The reallocated pointer.
 */
static inline void *mdbg_realloc(void *ptr, size_t size)
{
	ptr = realloc(ptr, size ?: 1);
	if(ptr == NULL)
		fprintf(stderr, "allocation failed, %s\n", strerror(errno));

	return ptr;
}
/**
 * Debugged free memory.
 *   @ptr: The pointer.
 */
static inline void mdbg_free(void *ptr)
{
#ifndef RELEASE
	if(ptr == NULL)
		fprintf(stderr, "attempted to free null pointer\n");

	mdbg_cnt--;
#endif

	free(ptr);
}


/**
 * Debugged duplicate memory.
 *   @ptr: The original pointer.
 *   @size: The size.
 *   &returns: The duplicated memory.
 */
static inline void *mdbg_memdup(const void *ptr, size_t size)
{
	void *alloc;

	alloc = mdbg_alloc(size);
	memcpy(alloc, ptr, size);

	return alloc;
}

/**
 * Debugged duplicate a string.
 *   @str: The string.
 *   &returns: The copy.
 */
static inline char *mdbg_strdup(const char *str)
{
	return mdbg_memdup(str, strlen(str) + 1);
}


/*
 * redefine symbols
 */
#ifndef MDBG_NO_RENAME
#	undef malloc
#	undef realloc
#	undef free
#	undef memdup
#	undef strdup
#	define malloc mdbg_malloc
#	define realloc mdbg_realloc
#	define free mdbg_free
#	define memdup mdbg_memdup
#	define strdup mdbg_strdup
#endif


#endif
