#ifndef INLINE_H
#define INLINE_H

/*
 * required headers
 */
#include <stddef.h>
#include <string.h>


/**
 * Retrive the parent data structure from a member pointer.
 *   @ptr: The member poitner.
 *   @type: The parent type.
 *   @member: The member identifier.
 *   &returns: The parent pointer.
 */
#define getparent(ptr, type, member) ((type *)((void *)(ptr) - offsetof(type, member)))

static inline void memzero(void *ptr, size_t size)
{
	memset(ptr, 0x00, size);
}

#endif
