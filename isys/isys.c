#ifdef WINDOWS
#	include "windows.h"
#else
#	include "posix.h"
#endif

#include "isys.h"
#include "../mdbg/mdbg.h"


/**
 * Task structure.
 *   @func: The task function.
 *   @arg: The argument.
 */
struct isys_task_t {
	struct isys_thread_t *thread;
	struct isys_event_t *event;

	void (*func)(isys_fd_t, void *);
	void *arg;
};

/*
 * local declarations
 */
static void *task_proc(void *arg);


/**
 * Create a new task.
 *   @func: The task function.
 *   @arg: The argument.
 *   &returns: The task.
 */
struct isys_task_t *isys_task_new(void (*func)(isys_fd_t, void *), void *arg)
{
	struct isys_task_t *task;

	task = malloc(sizeof(struct isys_task_t));
	task->func = func;
	task->arg = arg;
	task->event = isys_event_new();
	task->thread = isys_thread_new(task_proc, task);

	return task;
}

/**
 * Delete a task.
 *   @task: The task.
 */
void isys_task_delete(struct isys_task_t *task)
{
	isys_event_signal(task->event);
	isys_thread_join(task->thread);
	isys_event_delete(task->event);
	free(task);
}

/**
 * Process a task thread.
 *   @arg: The argument.
 *   &returns: Always zero.
 */
static void *task_proc(void *arg)
{
	struct isys_task_t *task = arg;

	task->func(isys_event_fd(task->event), task->arg);

	return NULL;
}
