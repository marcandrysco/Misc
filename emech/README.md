Error Mechanism
===============

The error mechanism header provides a set of macros for handling and reporting
string errors. To ease resource management, the macros leverage user-defined
values to automatically insert resource freeing code on errors.


## Error Architecture

The Error Mechanism header intended only for string-based error messages. Each
message is allocated on the heap so they must be `free`d in order to prevent
memory leaks.

Errors are expected to be returned by functions, and the return type of each
function is `char *`. Success is indicated by a `NULL` value. Errors are
created by the `fail` macro that executes the `onerr` code and returns from
the function. The caller is expected to handle any possible errors by using
the `chk#action#` macros.


## Macro API

All functionality is provided through safe macros -- all arguments will only
be executed once.

    #define onerr(CODE)

The `onerr` macro prepares the failure handler. All code given by `CODE` will
be run in event of an error that is generated via `fail` or propagated via
`chkret`.

    #define fail(fmt, ...)

The `fail` macro executes the cleanup code given by `onerr` and returns the
formatted error message as an allocated string.

    #define chkret(val)
    #define chkexit(val)
    #define chkabort(val)
    #define chkbool(val)
    #define chkwarn(val)

The checking macros check a value for an error (i.e. non-null) and performs
the stated action.

The `chkret` macro calls the current fucntions `onerr` code and then returns
the error.

The `chkexit` macro prints the errors message and exits with an exit status of
`1`.

The `chkabort` macro prints the errors message and aborts the program.

The `chkbool` macro converts the error message into a simple boolean where
true indicates success and false indicates failure. Any error message is
automatically freed.

The `chkwarn` macro is identical to `chkbool` by returning a boolean that
indicates success, except that an error message will be printed. Any error
message is automatically freed.

    void unreachable()

The `unreachable` macro is used to mark code that should never be reached. If
`RELEASE` is defined, the compiler will optimize accordingly and therefore any
code that calls `unreachable` will execute undefined. If `RELEASE` is not
defined, an error will be printed and the program will be aborted.


## Examples

