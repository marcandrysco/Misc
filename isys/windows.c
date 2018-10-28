#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include "windows.h"
#include "isys.h"
#include "../mdbg/mdbg.h"

/*
 * local declarations
 */
static DWORD thread_proc(LPVOID param);

/**
 * Thread structure.
 *   @func: The function.
 *   @arg, ptr: The argument and return values.
 *   @handle: The handle
 *   @lock: The lock.
 *   @detach: Detach flag.
 */
struct isys_thread_t {
	void *(*func)(void *);
	void *arg, *ret;
	HANDLE handle;
	CRITICAL_SECTION lock;
	bool detach;
};

/**
 * Create a thread.
 *   @flags: The flags.
 *   @func: The callback function.
 *   @arg: The arguments.
 */
struct isys_thread_t *isys_thread_create(void *(*func)(void *), void *arg)
{
	struct isys_thread_t *thread;

	thread = malloc(sizeof(struct isys_thread_t));
	thread->func = func;
	thread->arg = arg;
	thread->detach = false;
	InitializeCriticalSection(&thread->lock);

	thread->handle = CreateThread(NULL, 0, thread_proc, thread, 0, NULL);
	if(thread->handle == NULL)
		fatal("Failed to create thread.");

	return thread;
}

/**
 * Join a thread.
 *   @thread: The thread.
 *   &returns: The returned value.
 */
void *isys_thread_join(struct isys_thread_t *thread)
{
	void *ret;

	WaitForSingleObject(thread->handle, INFINITE);
	ret = thread->ret;

	DeleteCriticalSection(&thread->lock);
	CloseHandle(thread->handle);
	free(thread);

	return ret;
}

/**
 * Detach from a thread. The detach flag tells us if the thread was already
 * completed.
 *   @thread: The thread.
 */
void isys_thread_detach(struct isys_thread_t *thread)
{
	bool detach;

	EnterCriticalSection(&thread->lock);
	detach = thread->detach;
	thread->detach = true;
	LeaveCriticalSection(&thread->lock);

	if(detach) {
		DeleteCriticalSection(&thread->lock);
		free(thread);
	}

	CloseHandle(thread->handle);
}

/**
 * Thread procedure.
 *   @param: The parameter.
 *   &returns: Always zero.
 */
static DWORD thread_proc(LPVOID param)
{
	bool detach;
	struct isys_thread_t *thread = param;

	thread->ret = thread->func(thread->arg);

	EnterCriticalSection(&thread->lock);
	detach = thread->detach;
	thread->detach = true;
	LeaveCriticalSection(&thread->lock);

	if(detach) {
		DeleteCriticalSection(&thread->lock);
		free(thread);
	}

	return 0;
}


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
