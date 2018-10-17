#include "../mdbg/mdbg.h"
#include "membuf.h"


/**
 * Initialize a memory buffer.
 *   @init: The initial length.
 *   &returns: The buffer.
 */
struct membuf_t membuf_init(uint64_t init)
{
	return (struct membuf_t){ malloc(init), 0, init };
}

/**
 * Destroy a memory buffer.
 *   @buf: The buffer.
 */
void membuf_destroy(struct membuf_t buf)
{
	free(buf.arr);
}

/**
 * Finish a memory buffer, returning the allocated memory.
 *   @buf: The buffer.
 *   @len: Optional. The length of the buffer.
 *   &returns: The allocated memory. Must be freed to prevent memory leaks.
 */
void *membuf_done(struct membuf_t *buf, uint64_t *len)
{
	if(len != NULL)
		*len = buf->len;

	return realloc(buf->arr, buf->len);
}


/**
 * Add an 8-bit, unsigned integer to a memory buffer.
 *   @buf: The buffer.
 *   @val: The val.
 */
void membuf_addu8(struct membuf_t *buf, uint8_t val)
{
	if(buf->len == buf->max)
		buf->arr = realloc(buf->arr, buf->len *= 2);

	buf->arr[buf->len++] = val;
}

/**
 * Add some data to the buffer.
 *   @buf: The buffer.
 *   @data: The data.
 *   @nbytes: The nummber of bytes to add.
 */
void membuf_addbuf(struct membuf_t *restrict buf, const void *restrict data, uint64_t nbytes)
{
	uint64_t i;
	const uint8_t *ptr = data;

	for(i = 0; i < nbytes; i++)
		membuf_addu8(buf, ptr[i]);
}


/**
 * Add a character to a memory buffer.
 *   @buf: The buffer.
 *   @ch: The character.
 */
void membuf_addch(struct membuf_t *buf, char ch)
{
	membuf_addch(buf, ch);

}

/**
 * Add a string to a memory buffer, excluding the final null byte.
 *   @buf: The buffer.
 *   @str: The string.
 */
void membuf_addstr(struct membuf_t *restrict buf, const char *restrict str)
{
	while(*str != '\0')
		membuf_addch(buf, *str++);
}
