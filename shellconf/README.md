Shell Configure
===============

The Shell Configure script is a minimal configure script for building C
projects. It is written only using POSIX shell and common utilities for
maximum protability.


## Running `configure`

The `configure` command is used to generate the `Makefile` for building the
target project. The project may be customized using the options passed into
configure, either directly from the command-line or read from `config.args`.


### Configure Options

  * `--cygwin`: Build for Cygwin.
  * `--debug`: Denote a debug build.
  * `--prefix`: Prefix for installation. Set to `/usr/local` by default.
  * `--release`: Denote a release build.
  * `--repath`: Build using an rpath. This is useful for local builds.
  * `--toolchain`: Specifies the toolchain to use for compilation. The value
    is prepended to the build command for selecting the given tooclhain. For
    example, use `--toolchain=x86_64-w64-mingw32`. to build with the mingw-w64
    toolchain.
  * `--windows`: Build for windows.


### Reading Options from `config.args`

To reduce the burden of having to reenter the option for every call to
`configure`, options can be specified in a file called `config.args`. These
options are prepended to whatever is passed directly into configure; i.e., you
can specify options both in `config.args` and on the command-line. An example
`config.args` follows -- note that you can use newlines to seperate options in
the file.

    --prefix="$HOME/build"
    --rpathe="$HOME/build"


### Local builds

It is often useful to build and install to the user's local directory so that
root access is not required. The easiest way to achieve this is to pass a
local directory to `--prefix` and `--rpath`.

    ./configure --prefix="$HOME/build" --rpath="$HOME/build"

The `--prefix` parameter tells the build system to install all the built files
to the local path. Then, the `--rpath` parameter tells any built binaries to
first look for libraries in the local path. Using both parameters a binary
plus all of its dependencies will effectively create a local installation of
the built environment.


### Cross-Compile to Windows

In order to cross-compile a target to windows, you need to specify the option
`--windows` to the build system that you want a Windows target and
`--toolchain` to specify the compiler toolchain. Mingw-w64 is a common
toolchain using the prefix `x86_64-w64-mingw32`. The follow snippet shows a
complete configure call for Mingw-w64.

    ./configure --windows --toolchain="x86_64-w64-mingw32"
