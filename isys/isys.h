#ifndef ISYS_H
#define ISYS_H

/*
 * common headers
 */
#include <stdbool.h>
#include <stdint.h>

/*
 * poll mask definitions
 */
#define ISYS_READ  0x01
#define ISYS_WRITE 0x02
#define ISYS_ERROR 0x04

/**
 * Poll structure.
 *   @fd: The file descriptor.
 *   @mask, got: The mask and gotten events.
 */
struct isys_poll_t {
	isys_fd_t fd;
	uint16_t mask, got;
};


/*
 * thread declarations
 */
typedef struct isys_thread_t *isys_thread_t;

isys_thread_t isys_thread_new(void *(*func)(void *), void *arg);
void isys_thread_detach(isys_thread_t thread);
void *isys_thread_join(isys_thread_t thread);

/*
 * mutex declarations
 */
typedef struct isys_mutex_t *isys_mutex_t;

isys_mutex_t isys_mutex_new(void);
void isys_mutex_delete(isys_mutex_t mutex);

void isys_mutex_lock(isys_mutex_t mutex);
bool isys_mutex_trylock(isys_mutex_t mutex);
void isys_mutex_unlock(isys_mutex_t mutex);

/*
 * event declarations
 */
struct isys_event_t *isys_event_new(void);
void isys_event_delete(struct isys_event_t *event);

isys_fd_t isys_event_fd(struct isys_event_t *event);
void isys_event_signal(struct isys_event_t *event);
void isys_event_reset(struct isys_event_t *event);
void isys_event_wait(struct isys_event_t *event);

/*
 * poll declarations
 */
bool isys_poll(struct isys_poll_t *fds, unsigned int cnt, int timeout);

static inline uint16_t isys_poll1(isys_fd_t fd, uint16_t mask, int timeout)
{
	struct isys_poll_t poll;

	poll.fd = fd;
	poll.mask = mask;

	isys_poll(&poll, 1, timeout);

	return poll.got;
}

/*
 * task declarations
 */
struct isys_task_t *isys_task_new(void (*func)(isys_fd_t, void *), void *arg);
void isys_task_delete(struct isys_task_t *task);

#endif
