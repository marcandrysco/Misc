#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "isys.h"
#include "../mdbg/mdbg.h"


/*
 * local declarations
 */
static inline void chk(int err, const char *msg);


/**
 * Thread structure.
 */
struct isys_thread_t {
	pthread_t pthrd;
};

/**
 * Create a mutex.
 *   @func: The function.
 *   @arg: The argument.
 *   &returns: The mutex.
 */
struct isys_thread_t *isys_thread_new(void *(*func)(void *), void *arg)
{
	struct isys_thread_t *thread;
	
	thread = malloc(sizeof(struct isys_thread_t));
	//chk(pthread_thread_init(&thread->pthrd, NULL), "Failed to create thread.");

	return thread;
}

void isys_thread_detach(struct isys_thread_t *thread)
{
	free(thread);
}

void *isys_thread_join(struct isys_thread_t *thread)
{
	free(thread);

	return NULL;
}


/**
 * Mutex structure.
 *   @pthrd: The pthread mutex.
 */
struct isys_mutex_t {
	pthread_mutex_t pthrd;
};


/**
 * Create a mutex.
 *   &returns: The mutex.
 */
struct isys_mutex_t *isys_mutex_new(void)
{
	struct isys_mutex_t *mutex;
	
	mutex = malloc(sizeof(struct isys_mutex_t));
	chk(pthread_mutex_init(&mutex->pthrd, NULL), "Failed to create mutex.");

	return mutex;
}

/**
 * Delete a mutex.
 *   @mutex: The mutex.
 */
void isys_mutex_delete(struct isys_mutex_t *mutex)
{
	chk(pthread_mutex_destroy(&mutex->pthrd), "Failed to destroy mutex.");
	free(mutex);
}


/**
 * Lock a mutex.
 *   @mutex: The mutex.
 */
void isys_mutex_lock(struct isys_mutex_t *mutex)
{
	chk(pthread_mutex_lock(&mutex->pthrd), "Failed to lock mutex.");
}

/**
 * Try to lock a mutex.
 *   @mutex: The mutex.
 *   &returns: True if successful, false if unable to lock.
 */
bool isys_mutex_trylock(struct isys_mutex_t *mutex)
{
	int err;

	err = pthread_mutex_trylock(&mutex->pthrd);
	if(err == EBUSY)
		return false;

	chk(err, "Failed attempt to lock mutex.");

	return true;
}

/**
 * Unlock a mutex.
 *   @mutex: The mutex.
 */
void isys_mutex_unlock(struct isys_mutex_t *mutex)
{
	chk(pthread_mutex_unlock(&mutex->pthrd), "Failed to unlock mutex.");
}


/**
 * Check an error, printing a message and terminating on failure.
 */
static inline void chk(int err, const char *msg)
{
	if(err != 0) {
		fprintf(stderr, "%s. %s.\n", msg, strerror(err));
		abort();
	}
}
