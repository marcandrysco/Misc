#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "windows.h"
#include "isys.h"
#include "../mdbg/mdbg.h"


/**
 * Mutex structure.
 *   @crit: The critical section object
 */
struct isys_mutex_t {
	CRITICAL_SECTION crit;
};


/**
 * Create a mutex.
 *   &returns: The mutex.
 */
struct isys_mutex_t *isys_mutex_new(void)
{
	struct isys_mutex_t *mutex;
	
	mutex = malloc(sizeof(struct isys_mutex_t));
	InitializeCriticalSection(&mutex->crit);

	return mutex;
}

/**
 * Delete a mutex.
 *   @mutex: The mutex.
 */
void isys_mutex_delete(struct isys_mutex_t *mutex)
{
	DeleteCriticalSection(&mutex->crit);
	free(mutex);
}


/**
 * Lock a mutex.
 *   @mutex: The mutex.
 */
void isys_mutex_lock(struct isys_mutex_t *mutex)
{
	EnterCriticalSection(&mutex->crit);
}

/**
 * Try to lock a mutex.
 *   @mutex: The mutex.
 *   &returns: True if successful, false if unable to lock.
 */
bool isys_mutex_trylock(struct isys_mutex_t *mutex)
{
	return TryEnterCriticalSection(&mutex->crit);
}

/**
 * Unlock a mutex.
 *   @mutex: The mutex.
 */
void isys_mutex_unlock(struct isys_mutex_t *mutex)
{
	LeaveCriticalSection(&mutex->crit);
}


/**
 * Event structure.
 *   @handle: The handle.
 */
struct isys_event_t {
	HANDLE handle;
};

/**
 * Create an event.
 *   &returns: The event.
 */
struct isys_event_t *isys_event_new(void)
{
	struct isys_event_t *event;

	event = malloc(sizeof(struct isys_event_t));
	event->handle = CreateEvent(NULL, FALSE, FALSE, NULL);

	return event;
}

/**
 * Delete an event.
 *   @event: The event.
 */
void isys_event_delete(struct isys_event_t *event)
{
	CloseHandle(event->handle);
	free(event);
}
