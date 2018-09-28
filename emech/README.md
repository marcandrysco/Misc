Error Mechanism
===============

The error mechanism header provides a set of macros for handling and reporting
string errors. To ease resource management, the macros leverage user-defined
values to automatically insert resource freeing code on errors.


## Macro API

    #define onerr(CODE)

The `onerr` macro prepares the failure handler. All code given by `CODE` will
be run in event of an error that is thrown via `throw` or propagated via
`chkret`.

    #define throw(fmt, ...)

The `throw` macro executes the cleanup code given by `onerr` and returns the
formatted error message as an allocated string.

    #define chkret(val)
    #define chkexit(val)
    #define chkabort(val)

The checking macros check a value for an error (i.e. non-null) and performs
the stated action.

The `chkret` macro calls the current fucntions `onerr` code and then returns
the error.

The `chkexit` macro prints the errors message and exits with an exit status of
`1`.

The `chkabort` macro prints the errors message and aborts the program.

    void unreachable()

The `unreachable` macro is used to mark code that should never be reached. If
`RELEASE` is defined, the compiler will optimize accordingly and therefore any
code that calls `unreachable` will execute undefined. If `RELEASE` is not
defined, an error will be printed and the program will be aborted.
