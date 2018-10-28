Miscellaneous Code
==================

This repository is a collection of miscellaneous bits of code for use in other
projects. Each mini-project is versioned so hopefully downstream projects can
be updated in a sane way. Furthermore, the entire code base may be compiled
into a single library and header for use in a project.


## Components

All components listed below alphabetically. Any dependencies are explicitly
noted. Any versions below 1.0 are provisional; expect breaking changes.

 * [AnyFS (v0.1)](afs/README.md) -- Platform independent filesystem access.
     Depends: AVL Tree, Error Mechanism, Memory Debugger.
 * [AVL Tree (v0.1)](avl/README.md) -- Intrusive AVL tree implementation.
 * [Error Mechanism (v0.1)](emech/README.md) -- Error reporting and
     propagation library.
 * [Inlines Header](inline/README.md) -- Header of small, inlined functions
     and macros.
 * [Linear Algebra](lin/README.md) -- Linear algebra library.
 * [Lexigraphical Parser](lex/README.md) -- Parser for converting an input
     stream to lexemes. Depends: AVL Tree, Error Mechanism, Memory Buffer,
     Memory Debugger.
 * [Memory Buffer](membuf/README.md) -- Memory (or string) buffering library.
     Depends: Memory Debugger.
 * [Memory Debugger (v0.1)](mdbg/README.md) -- Debugging memory bugs (e.g.
     leaked memory or double free). Depends: AVL Tree.
 * [Shell Configure (v1.0.0a)](shellconf/README.md) -- A portable shell
     configure script aimed at C programs.


## Versioning

Each version is in the form `major.minor.revision` where each component is an
integer. This means that version `1.10.0` is a newer version that `1.2.0`. The
meaning of the components follows

  * Major: Any major change will probably break code that relies on the
    library. There is not real "limit" to how much is different. Be prepared
    to fix code if you update through a major change.
  * Minor: A minor change at involves adding features to the existing code.
    Code that depends on the library *should not* break; of course, extreme
    cases exist, such as a client program accessing private or undocumented
    features.
  * Revision: A revision occurs when no APIs are added, removed, or modified.
    Typically, this is reserved for bug fixes, refactoring, or documentation
    changes.

An additional 'a', 'b', or 'rc#' may be added to the end to denote alpha,
beta, or release candidate. These are not full releases so they do not need to
obey the above rules.
