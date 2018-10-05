#include "isys.h"

#if 0
/**
 * Task structure.
 *   @sync, thread: The synchronization and thread handles.
 *   @func: The task function.
 *   @arg: The argument.
 */
struct isys_task_t {
	sync, thread;

	sys_task_f func;
	void *arg;
};


/**
 * Create a new task.
 *   @func: The task function.
 *   @arg: The argument.
 *   &returns: The task.
 */
struct sys_task_t *sys_task_new(sys_task_f func, void *arg)
{
	struct sys_task_t *task;

	task = malloc(sizeof(struct sys_task_t));
	task->func = func;
	task->arg = arg;
	task->sync = CreateEvent(NULL, TRUE, FALSE, NULL);
	task->thread = CreateThread(NULL, 0, task_proc, task, 0, NULL);

	return task;
}

/**
 * Delete a task.
 *   @task: The task.
 */
void sys_task_delete(struct sys_task_t *task)
{
	SetEvent(task->sync);
	WaitForSingleObject(task->thread, INFINITE);
	CloseHandle(task->sync);
	CloseHandle(task->thread);
	free(task);
}

/**
 * Process a task thread.
 *   @arg: The argument.
 *   &returns: Always zero.
 */
static DWORD task_proc(LPVOID arg)
{
	struct sys_task_t *task = arg;

	task->func(w32_fd(task->sync), task->arg);

	return 0;
}
#endif
