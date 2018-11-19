#pragma once

#include <stdint.h>

/**
 * File descriptor structure.
 *   @handle: The handle.
 *   @socket; The socket.
 */
struct isys_fd_t {
	void *handle;
	int64_t socket;
};

static inline struct isys_fd_t isys_fd_handle(void *handle)
{
	return (struct isys_fd_t){ handle, INVALID_SOCKET };
}

static inline struct isys_fd_t isys_fd_socket(int64_t socket, void *handle)
{
	return (struct isys_fd_t){ handle, socket };
}


/*
 * file descriptor type
 */
typedef struct isys_fd_t isys_fd_t;
