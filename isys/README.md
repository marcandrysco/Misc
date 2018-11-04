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

Basic threading support is given by binding to the system APIs. In the case of
Windows, a little extra work is used to create a consistent interface between
platforms. If you require more control over the threading, use the system APIs
instead.

    isys_thread_t isys_thread_new(void *(*func)(void *), void *arg);
    void isys_thread_detach(isys_thread_t thread);
    void *isys_thread_join(isys_thread_t thread);

The `isys_thread_new` function creates a new thread, calling `func` with
`arg`.

The `isys_thread_detach` function stops tracking the `thread`, reaping all
resources whenever the thead exits.

The `isys_thread_join` function waits of the thread to exit and retrieves
the value returned by the thread.


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


### Polling

    struct isys_poll_t {
      isys_fd_t fd;
      uint16_t mask, got;
    };

    bool isys_poll(struct isys_poll_t *fds, unsigned int cnt, int timeout);
    uint16_t isys_poll1(isys_fd_t fd, uint16_t mask, int timeout);

The `isys_poll` function waits on a set of file descriptors for an event. The
file descriptors and event masks are stored in `fds`, containing exactly `cnt`
descriptors. The `timeout` indicates how long to wait for an event in
milliseconds; a value of zero will return immediately, and a negative value
will wait indefinitely until an event occurs.

### Tasks
