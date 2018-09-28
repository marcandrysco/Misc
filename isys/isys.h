#ifndef ISYS_H
#define ISYS_H

/*
 * common headers
 */
#include <stdbool.h>

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

#endif
