(Platform) Independent System
=============================

The platform indepdent system is a small library for standardizing common
system interfaces.

  * Threading
  * Synchronization (mutex/rwlock)
  * Timing (sleep/wait)


## API

The API consists of a set of datatypes and functions for those datatypes. To
prevent leaked resources, make sure every object gets destroyed once it is no
longer needed.

Notice that there are no errors returned by any function. All error are
handled internally by printing the error and aborting the program. If you need
finer grained control over the errors (e.g. out of resource handling), you
cannot use this library. You are probably better of directly using the system
API if you need that level of control.


### Threads


### Mutexes

Mutexes are simple locking primitives for controlling access to a resource.
ISys only provides basic mutexes; they do not support recursive locking or
priorities.

    typedef struct isys_mutex_t *isys_mutex_t;

The system mutex is hidden behind the opaque type `isys_mutex_t` or you can
use pointers to the underlying structure `isys_mutex_t`

    isys_mutex_t isys_mutex_new(void);
    void isys_mutex_delete(isys_mutex_t mutex);

The `isys_mutex_new` function creates a new mutex. The returned mutex will be
in the unlocked state.

The `isys_mutex_delete` function deletes a mutex. The mutex must be unlocked
when it is deleted.

    void isys_mutex_lock(isys_mutex_t mutex);
    void isys_mutex_unlock(isys_mutex_t mutex);

The `isys_mutex_lock` function locks the mutex.

The `isys_mutex_unlock` function unlocks the mutex.
