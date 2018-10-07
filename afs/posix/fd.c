#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../afs.h"
#include "../../emech/emech.h"


typedef int io_fd_t;


/**
 * Open a file.
 *   @fd: The file descriptor.
 *   @path: The path.
 *   @flags: The flags.
 *   &returns: Error.
 */
char *io_fd_open(io_fd_t *fd, const char *path, uint32_t flags, uint32_t mode)
{
	uint32_t conv = 0;

	onerr();

	if((flags & IO_RDWR) == IO_READ)
		conv |= O_RDONLY;
	else if((flags & IO_RDWR) == IO_WRITE)
		conv |= O_WRONLY;
	else if((flags & IO_RDWR) == IO_RDWR)
		conv |= O_RDWR;

	if(open(path, conv, mode) < 0)
		fail("Cannot open '%s' for writing. %s.", path, strerror(errno));

	return NULL;
}

/**
 * Close a file.
 *   @fd: The file descriptor.
 */
void io_fd_close(io_fd_t fd)
{
	close(fd);
}


struct io_file_t {
	io_fd_t fd;
};

typedef struct io_file_t io_file_t;

char *io_file_open(io_file_t *file, const char *path, uint32_t mode)
{
	onerr( free(file); );

	fail("hi");

	return NULL;
}
