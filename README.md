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
     propagation library. Depends: Memory Debugger.
 * [Linear Algebra](lin/README.md) -- Linear algebra library.
 * [Memory Debugger (v0.1)](mdbg/README.md) -- Debugging memory bugs (e.g.
     leaked memory or double free). Depends: AVL Tree.
