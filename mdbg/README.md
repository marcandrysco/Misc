Memory Debugging
================

The memory debugging tools replace all memory allocation calls (e.g. `malloc`
and `free`) with calls to the debugger's verions (e.g. `mdbg_malloc` and
`mdbg_free`). The debugger functions provide mechanisms discovering memory
leaks, detecting use-after-free, and heap buffer overflows/underflows.


## Debugging Levels

The memory debugger provides three levels of debugging: release, test, and
debug. The level is selected using by checking for a definition (`#define`) of
`RELEASE`, `TEST`, or `DEBUG`. The debugging level is encoded into the shared
library, so replacing the library will change the debug level of all programs
that consumed it.

  * Release Mode -- All allocation calls are directly forwarded to the
    internal allocator. Absolutely no checks are performed.
  * Test Mode -- The total number of outstanding allocations are tracked to
    determine if memory leaks occurred.
  * Debug Mode -- Every allocation is explicitly tracked so that any leaked
    memory can report where it was allocation. Protections are added to check
    for buffer overflows or underflows.


## API

The API consists of functions for managing memory (allocation and freeing) and
checking the current state of resource usage.


### Memory Management

The memory management functions correspond to functions already found in the
standard library.

    void *mdbg_alloc(size_t size)
    void *mdbg_realloc(void *ptr, size_t size)
    void mdbg_free(void *ptr)

The `mdbg_alloc`, `mdbg_realloc`, and `mdbg_free` functions correspond to the
the ones found in the standard library.

    void *mdbg_memdup(const void *ptr, size_t size)
    char *mdbg_strdup(const char *str)

The `mdbg_memdup` function duplicates the data pointed to by `ptr`, creating
an allocation of `size` bytes.

The `mdbg_strdup` function corresponds to the standard library function
`strdup`.


### Internal Functions

Internally, all debugging levels are implemented as individual functions that
may be called directly.

    void *mdbg_malloc_release(size_t size);
    void *mdbg_malloc_test(size_t size);
    void *mdbg_malloc_debug(size_t size);

The `mdbg_malloc_#LEVEL#` functions directly call the allocation function for
the specific debugging level. Calling `mdbg_malloc` (or `malloc` after
redefinition) will automatically call the appropriate internal function.

    void mdbg_free_release(void *ptr);
    void mdbg_free_test(void *ptr);
    void mdbg_free_debug(void *ptr);

The `mdbg_free_#LEVEL#` functions directly call the free function for the
specific debugging level. Calling `mdbg_free` (or `free` after redefinition)
will automatically call the appropriate internal function.


### Bonus APIs

These are interfaces that are beyond those given by the standard library.

    char *mdbg_mprintf(const char *restrict fmt, ...);
    char *mdbg_vmprintf(const char *restrict fmt, va_list args);

The `mdbg_mprintf` and `mdbg_vmprintf` functions are printf-style functions
that print to the string that is returned. The returned string must be freed.

    void mdbg_fatal(const char *restrict fmt, ...);

The `mdbg_fatal` function writes a formatted message to `stderr` and
terminates the program with `abort`.


### Redefining Symbols

For ease of use, the symbols to the standard library calls (e.g. `malloc`) are
redefined to call the debugged versions with the `mdbg_` prefix. This behavior
is disabled by defining `MDBG_NO_RENAME`.
