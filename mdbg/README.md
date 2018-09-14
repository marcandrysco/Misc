Memory Debugging
================

The memory debugging tools replace all memory allocation calls (e.g. `malloc`
and `free`) with calls to the debugger's verions (e.g. `mdbg_malloc` and
`mdbg_free`). The debugger functions provide mechanisms discovering memory
leaks, detecting use-after-free, and heap buffer overflows/underflows.


## Debugging Levels

The memory debugger provides three levels of debugging: release, test, and
debug. The level is selected using by checking for a definition (`#define`) of
`RELEASE`, `TEST`, or `DEBUG`.

### Release Mode

All allocation calls are directly forwarded to the internal allocator.
Absolutely no checks are performed.

### Test Mode

The total number of outstanding allocations are tracked to determine if memory
leaks occurred.

### Debug Mode

Every allocation is explicitly tracked so that any leaked memory can report
where it was allocation. Protections are added to check for buffer
overflows or underflows.


## API

    void *mdbg_alloc(size_t size)
    void *mdbg_realloc(void *ptr, size_t size)
    void mdbg_free(void *ptr)

The `mdbg_alloc`, `mdbg_realloc`, and `mdbg_free` functions correspond to the
the ones found in the standard library.

    void *mdbg_memdup(const void *ptr, size_t size)
    char *mdbg_strdup(const char *str)

The `mdbg_memdup` function duplicates the data pointed to by `ptr`, creating
an allocation of `size` bytes.

THe `mdbg_stdup` function corresponds to the standard library function
`strdup`.

### Redefining Symbols

For ease of use, the symbols to the standard library calls (e.g. `malloc`) are
redefined to call the debugged versions with the `mdbg_` prefix. This behavior
is disabled by defining `MDBG_NO_RENAME`.
