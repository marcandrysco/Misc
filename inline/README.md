Inlines Header
==============

The inlines header provides an ecletic set of common functions that are small
and directly inlined into the user code.


## API

The API consists of either macros or static inline functions, both of which
are inlined directly to the call site.

    #define getparent(ptr, type, member) ((type *)((void *)(ptr) - offsetof(type, member)))

The `getparent` macro takes a pointer to a member pointer `ptr` and computes a
pointer to the parent structure. The `member` parameter is the name of the
member in the structure, and `type` is the type name of the parent structure.
The parameter `type` is unsafe and will be evaluated multiple times; the other
parameters `ptr` and `member` are safe and only evaluated once.

    void memzero(void *ptr, size_t size)

The `memzero` function writes zeros to a piece of memory. This function
interally calls `memset`.

    static inline char *strprefix(const char *str, const char *pre)

The `strprefix` function determines if `pre` is a prefix of `str`. If so, the
function returns a pointer after the prefix in `str`. If not found, the
function returns `NULL`.

## Examples


### Ex1. Pointers using `getparent`

The following example demonstrates how `getparent` is used to take a pointer
to a member element and retrieves a pointer to the parent structure.

    // object with two members: a, b
    struct obj_t {
      int a, b;
    };

    // we have a pointer to the object
    struct obj_t *obj = ...;

    // ptr points to the member b of the object
    int *ptr = &obj->b;

    // getparent computes a pointer to the object from the pointer to member b
    struct obj_t *parent = getparent(ptr, struct obj_t, b);

    // the two pointers must be the same
    assert(obj == parent);

It is important to be careful to use the correct member. In the above code,
passing `a` as the last parameter to `getparent` would produce an invalid
pointer.
