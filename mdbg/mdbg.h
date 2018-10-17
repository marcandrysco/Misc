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

/*
 * function declarations
 */
void mdbg_check(void);
__attribute__((noreturn)) void mdbg_fatal(const char *restrict fmt, ...);

void *mdbg_malloc(size_t size);
void *mdbg_malloc_release(size_t size);
void *mdbg_malloc_test(size_t size);
void *mdbg_malloc_debug(size_t size);

void *mdbg_realloc(void *ptr, size_t size);
void *mdbg_realloc_release(void *ptr, size_t size);
void *mdbg_realloc_test(void *ptr, size_t size);
void *mdbg_realloc_debug(void *ptr, size_t size);

void mdbg_free(void *ptr);
void mdbg_free_release(void *ptr);
void mdbg_free_test(void *ptr);
void mdbg_free_debug(void *ptr);

char *mdbg_mprintf(const char *restrict fmt, ...) __attribute__ ((format (printf, 1, 2)));
char *mdbg_vmprintf(const char *restrict fmt, va_list args);


/**
 * Debugged duplicate memory.
 *   @ptr: The original pointer.
 *   @size: The size.
 *   &returns: The duplicated memory.
 */
static inline void *mdbg_memdup(const void *ptr, size_t size)
{
	void *alloc;

	alloc = mdbg_malloc(size);
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
#
#	define mprintf mdbg_mprintf
#	define vmprintf mdbg_vmprintf
#	define fatal mdbg_fatal
#endif


#endif
