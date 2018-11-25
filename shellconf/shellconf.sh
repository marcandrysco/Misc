#!/bin/sh

##
# ShellConf v1.0.0a
#   A portable shell build system.
##

# special characters
nl="`printf '\nX'`" ; nl="${nl%X}"
tab="`printf '\tX'`" ; tab="${tab%X}"

# Check if a string has a space.
#   @str: The string.
#   &returns: Non-zero if space found, zero otherwise.
chk_space()
{
  for __chk_space in "$@" ; do
    test -z "${__chk_space%%* *}" && return 1
  done
  return 0
}


# Setup the project information.
#   @name: The project name.
#   @ver: The project version.
project()
{
  test $# -ne 2 && fail "project function takes 2 arguments"
  test "$_sc_proj" && fail "project must only be called once"

  _sc_proj="$1"
  _sc_ver="$2"

  echo "DIST = configure shellconf.sh" >> "$_sc_mkfile"
  echo "dist:" >> "$_sc_mkfile"
  echo "${tab}rm -rf $_sc_proj-$_sc_ver" >> "$_sc_mkfile"
  echo "${tab}mkdir $_sc_proj-$_sc_ver" >> "$_sc_mkfile"
  echo "${tab}cp --parents \$(DIST) $_sc_proj-$_sc_ver" >> "$_sc_mkfile"
	echo "${tab}tar cf $_sc_proj-$_sc_ver.tar $_sc_proj-$_sc_ver" >> "$_sc_mkfile"
	echo "${tab}tar zcf $_sc_proj-$_sc_ver.tar.gz $_sc_proj-$_sc_ver" >> "$_sc_mkfile"
	echo "${tab}tar jcf $_sc_proj-$_sc_ver.tar.bz2 $_sc_proj-$_sc_ver" >> "$_sc_mkfile"
	echo "${tab}rm -rf $_sc_proj-$_sc_ver" >> "$_sc_mkfile"
  echo ".PHONY: dist" >> "$_sc_mkfile"
  echo "-include Makefile.user" >> "$_sc_mkfile"
}

# Start a new target
#   @path: The target path.
#   @type: The target type.
#   @rule: The rule.
#   ~shared: Flag for shared objects.
#   ~*config.h: Add `config.h` target.
#   ~*inc.h: Add `inc.h` target.
begin_target()
{
  test $# -lt 3 && fail "lib_target function takes at least 3 argument"

  _sc_target=$1 ; shift
  _sc_type=$1 ; shift
  _sc_rule=$1 ; shift
  _sc_hdr=""

  _sc_objs="_SC_OBJS$_sc_n"
  _sc_libs="_SC_LIBS$_sc_n"
  _sc_hdrs="_SC_LIBS$_sc_n"
  _sc_n=$((_sc_n+1))

  _sc_shared=""
  _sc_pic=""

  if [ "$_sc_type" = "lib" ] ; then
    _sc_dir="LIBDIR"
  else
    _sc_dir="BINDIR"
  fi

  for _sc_opt in "$@" ; do
    case "$_sc_opt" in
      shared)    _sc_shared=" -shared" ; _sc_pic=" -fPIC" ;;
      *config.h) cfg_target "$_sc_opt" ;;
      *inc.h)    inc_target "$_sc_opt" ;;
      *)         fail "invalid option '$_sc_opt' to lib_target" ;;
    esac
  done

  echo "$_sc_objs = " >> "$_sc_mkfile"
  echo "$_sc_libs = " >> "$_sc_mkfile"
  echo "$_sc_rule: $_sc_target" >> "$_sc_mkfile"
  echo "$_sc_target: \$($_sc_objs)" >> "$_sc_mkfile"
  echo "${tab}\$(LD)\$($_sc_objs)\$($_sc_libs)$_sc_shared \$(LDFLAGS) -o $_sc_target" >> "$_sc_mkfile"
  echo "CLEAN += $_sc_target" >> "$_sc_mkfile"

  echo "install: install_$_sc_target" >> "$_sc_mkfile"
  echo "install_$_sc_target: $_sc_target" >> "$_sc_mkfile"
  echo "${tab}install --mode 0755 -CD $_sc_target \$($_sc_dir)/$_sc_target" >> "$_sc_mkfile"
}

# Set the binary target.
#   @path: The target path.
bin_target()
{
  test $# -lt 1 && fail "bin_target function takes at least 1 argument"
  chk_space "$1" || fail "bin_target parameter '$1' has spaces"

  _sc_target="$1" ; shift
  begin_target "$_sc_target" "bin" "all" "$@"
}

# Set the library target.
#   @path: The target path.
lib_target()
{
  test $# -lt 1 && fail "lib_target function takes at least 1 argument"
  chk_space "$1" || fail "lib_target parameter '$1' has spaces"

  _sc_target="$1" ; shift
  begin_target "$_sc_target" "lib" "all" "shared" "$@"
}

# Set the test target.
#   @path: The target path.
test_target()
{
  test $# -lt 1 && fail "test_target function takes at least 1 argument"
  chk_space "$1" || fail "test_target parameter '$1' has spaces"

  _sc_target="$1" ; shift
  begin_target "$_sc_target" "test" "test" "$@"
}

# Set the header target
#   @path: The target path.
hdr_target()
{
  test $# -ne 1 && fail "hdr_target function must take 1 argument"
  chk_space "$1" || fail "hdr_target parameter '$1' has spaces"

  _sc_hdr="$1"
  _sc_hdrs="_SC_HDRS$_sc_n"
  _sc_n=$((_sc_n+1))

  echo "$_sc_hdrs = " >> "$_sc_mkfile"
  echo "all: $_sc_hdr" >> "$_sc_mkfile"
  echo "$_sc_hdr: Makefile" >> "$_sc_mkfile"
  echo "${tab}echo \"#pragma once\" > $_sc_hdr" >> "$_sc_mkfile"
  echo "${tab}tail -qn+2\$($_sc_hdrs) >> $_sc_hdr" >> "$_sc_mkfile"
  echo "CLEAN += $_sc_hdr" >> "$_sc_mkfile"

  echo "install: install_$_sc_hdr" >> "$_sc_mkfile"
  echo "install_$_sc_hdr: $_sc_hdr" >> "$_sc_mkfile"
  echo "${tab}install --mode 0755 -CD $_sc_hdr \$(INCDIR)/$_sc_hdr" >> "$_sc_mkfile"
}

# Set the config.h target
#   @path: The target path.
cfg_target()
{
  test $# -ne 1 && fail "cfg_target function must take 1 argument"
  chk_space "$1" || fail "cfg_target parameter '$1' has spaces"
 
  _sc_config="$1"

  rm -f "$_sc_config"
  echo "#pragma once" >> "$_sc_config"
}

# Set the inc.h target
#   @path: The target path.
inc_target()
{
  test $# -ne 1 && fail "inc_target function must take 1 argument"
  chk_space "$1" || fail "inc_target parameter '$1' has spaces"
 
  _sc_inc="$1"

  rm -f "$_sc_inc"
  echo "#pragma once" >> "$_sc_inc"
}


# Add a library as dependency
#   @lib: The library name without prefix 'lib' or postfix '.so'.
dep_lib()
{
  test $# -ne 1 && fail "dep_lib function takes 1 argument"
  chk_space "$1" || fail "dep_lib parameter '$1' has spaces"
  test -z "$_sc_libs" && fail "dep_lib called without a target"

  echo "$_sc_libs += -l$1" >> "$_sc_mkfile"
}

# Add an object as dependency.
#   @obj: The object file name.
dep_obj()
{
  test $# -ne 1 && fail "dep_obj function takes 1 argument"
  chk_space "$1" || fail "dep_obj parameter '$1' has spaces"
  test -z "$_sc_libs" && fail "dep_obj called without a target"

  echo "$_sc_libs += $1" >> "$_sc_mkfile"
  echo "$_sc_target: $1" >> "$_sc_mkfile"
}

# Add a C source file.
#   @path: The source path.
#   ~skip: Do not build, but add to dist.
c_src()
{
  test $# -lt 1 && fail "c_src function must take at least 1 argument"
  chk_space "$1" || fail "c_src parameter '$1' has spaces"
  test ${1##*.} != "c" && fail "c_src argument has invalid extension '.${1##*.}'"

  _sc_src="$1" ; shift
  _sc_skip=""

  for _sc_opt in "$@" ; do
    case "$_sc_opt" in
      "skip") _sc_skip=1 ;;
      *)      fail "invalid option '$_sc_opt' to c_src" ;;
    esac
  done

  if [ -z "$_sc_skip" ] ; then
    _sc_obj="${_sc_src%.*}.o"
    _sc_dep="${_sc_src%.*}.d"

    echo "$_sc_objs += $_sc_obj" >> "$_sc_mkfile"
    echo "$_sc_target: $_sc_obj" >> "$_sc_mkfile"
    echo "$_sc_obj: $_sc_src $_sc_mkfile" >> "$_sc_mkfile"
    echo "${tab}\$(CC) -c -D$_sc_mode$_sc_pic \$(CFLAGS) -I\$(INCDIR) -MD $_sc_src -o $_sc_obj" >> "$_sc_mkfile"
    echo "-include $_sc_dep" >> "$_sc_mkfile"
    echo "CLEAN += $_sc_obj" >> "$_sc_mkfile"
  fi

  test "$_sc_proj" && echo "DIST += $_sc_src" >> "$_sc_mkfile"
  test "$autoinc" && h_src "${_sc_src%.*}.h" "$@"
}

# Add a C header file.
#   @path: The source path.
#   ~skip: Do not build, but add to dist.
h_src()
{
  test $# -lt 1 && fail "h_src function must take at least 1 argument"
  chk_space "$1" || fail "h_src parameter '$1' has spaces"
  test ${1##*.} != "h" && fail "h_src argument has invalid extension '.${1##*.}'"

  _sc_src="$1" ; shift
  _sc_skip=""

  for _sc_opt in "$@" ; do
    case "$_sc_opt" in
      "skip") _sc_skip=1 ;;
      *)      fail "invalid option '$_sc_opt' to h_src" ;;
    esac
  done

  if [ -z "$_sc_skip" ] ; then
    if [ "$_sc_hdr" ] ; then
      echo "$_sc_hdr: $_sc_src" >> "$_sc_mkfile"
      echo "$_sc_hdrs += $_sc_src" >> "$_sc_mkfile"
    fi
    if [ "$_sc_inc" ] ; then
      _sc_path="`pathto "$_sc_inc" "$_sc_src"`"
      echo "#include \"$_sc_path\"" >> "$_sc_inc"
    fi
  fi

  test "$_sc_proj" && echo "DIST += $_sc_src" >> "$_sc_mkfile"
}

# Add a header include file.
#   @path: The source path.
inc_src()
{
  test $# -ne 1 && fail "inc_src function takes 1 argument"
  chk_space "$1" || fail "inc_src parameter '$1' has spaces"
  test ${1##*.} != "h" && fail "inc_src argument has invalid extension '.${1##*.}'"

  path="$1"
  rem="$inc"
  while [ "$path$rem" ] && [ "${path%%/*}" = "${rem%%/*}" ] ; do
    path=${path#*/} ; rem=${rem#*/}
  done

  test "$inc" && printf "#include \"%s\"\n" "$path" >> "$inc"
}

# Compare versions using greater than.
#   @left: The left version.
#   @right: The right version.
#   &returns: True if `left` greater than `right`.
ver_gt()
{
  test $# -ne 2 && fail "ver_gt function takes 2 arguments"

  __ver_gt_left="$1"
  __ver_gt_right="$2"

  while [ "$__ver_gt_left$__ver_gt_right" ] ; do
    __ver_gt_a="${__ver_gt_left%%.*}"
    __ver_gt_b="${__ver_gt_right%%.*}"

    test "$__ver_gt_a" -gt "$__ver_gt_b" && return 0
    test "$__ver_gt_a" -lt "$__ver_gt_b" && return 1

    __ver_gt_left="${__ver_gt_left#$__ver_gt_a}"
    __ver_gt_left="${__ver_gt_left#.}"
    __ver_gt_right="${__ver_gt_right#$__ver_gt_b}"
    __ver_gt_right="${__ver_gt_right#.}"
  done

  return 1
}


# Add an asset to the share directory.
#   @path: The source path.
share_src()
{
  test $# -ne 2 && fail "share_src function takes 2 arguments"
  chk_space "$1" || fail "share_src parameter '$1' has spaces"
  chk_space "$2" || fail "share_src parameter '$2' has spaces"

  dist="$dist $1"
  install="${install}${nl}${tab}install --mode 0644 -CD $1 \$(SHAREDIR)/$2"
}


##
# quote Function
#   Given the input string, it places it within single quotes, making sure that
#   any single quotes within the string are properly escaped.
# Version
#   1.2
# Parameters
#   string input
#     The input text.
# Printed
#   Prints out the quoted string.
#.
quote()
{
	__quote_str="$*"

	while [ 1 ]
	do
		__quote_piece="${__quote_str%%\'*}"
		test "$__quote_piece" = "$__quote_str" && break
		printf "'%s'\\'" "$__quote_piece"
		__quote_str="${__quote_str#*\'}"
	done

	printf %s "'$__quote_str'"
}

##                                                             
# Calculate the depth of a path.                  
#   pathdepth <path>
#.
pathdepth()            
{                           
  test $# -ne 1 && { echo "pathdepth requires one parameter" 1>&2 ; exit 1 ; }
        
  __pathdepth_n=0
  __pathdepth_rem="$1"                                                 
                                                           
  while [ "$__pathdepth_rem" ] ; do                                               
    __pathdepth_part="${__pathdepth_rem%%/*}"
    test "$__pathdepth_part" = "$__pathdepth_rem" && break
              
    if [ "$__pathdepth_part" = ".." ] ; then
      __pathdepth_n=$((__pathdepth_n-1))
    elif [ "$__pathdepth_part" != "." ] ; then
      __pathdepth_n=$((__pathdepth_n+1))
    fi                                                     
         
    __pathdepth_rem="${__pathdepth_rem#*/}"
  done
                               
  printf "%d" "$__pathdepth_n"
}                                                
                                                    
##     
# Compute a relative path from a source to a destination.
#   pathto <src> <dst>                          
#.                                                
pathto()
{    
  test $# -ne 2 && { echo "pathto requires two parameters" 1>&2 ; exit 1 ; }
                                                               
  __pathto_n="`pathdepth $1`"
  __pathto_out="$2"
                             
  while [ $__pathto_n -gt 0 ] ; do
    __pathto_out="../$__pathto_out"
    __pathto_n=$((__pathto_n-1))
  done                                                      
                                                             
  printf "%s\n" "$__pathto_out"                                                     
}

##
# fail Function
#   Print an error message and terminate. The function does not return.
# Version
#   1.0
# Parameters
#   string err
#     The error string.
#.
fail()
{
  printf 'error: %s\n' "$*" >&2
  exit 1
}

# build arguments list
_sc_args=""
for opt in "$@" ; do
  _sc_args="$_sc_args`quote "$opt"` "
done

# append config.args file
test -f config.args && eval set -- "${_sc_args}`cat config.args | tr '\n\t' '  '`"


# initialize options
_sc_release=""
_sc_debug=""
_sc_rpath=""
_sc_windows=""
_sc_cygwin=""
_sc_toolchain=""
_sc_prefix="/usr/local"
_sc_bindir='$(PREFIX)/bin'
_sc_libdir='$(PREFIX)/lib'
_sc_incdir='$(PREFIX)/include'
_sc_sharedir='$(PREFIX)/share'
_sc_cc="gcc"
_sc_ld="gcc"
_sc_cflags='-g -O2 -std=gnu11'
_sc_ldflags='-L$(LIBDIR)'

# parse options
while [ "$#" -gt 0 ] ; do
  case "$1" in 
    --release | --debug | --rpath | --windows | --cygwin )
      eval "_sc_${1#--}=1" ; shift
      ;;

    --toolchain=* | --prefix=* | --cc=* | --ld=* | --cflags=* | --ldflags=* )
      _sc_name="_sc_${1#--}" ; _sc_name="${_sc_name%%=*}" ; _sc_val="${1#*=}" ; shift
      eval "$_sc_name=`quote "$_sc_val"`"
      ;;

    --toolchain+=* | --prefix+=* | --cc+=* | --ld+=* | --cflags+=* | --ldflags+=* )
      _sc_name="_sc_${1#--}" ; _sc_name="${_sc_name%%+=*}" ; _sc_val="${1#*+=}" ; shift
      eval "$_sc_name=\"\$$_sc_name \"`quote "$_sc_val"`"
      ;;

    *)
      printf "unknown option '%s'\n" "$1" >&2 ;
      exit 1 ;
      ;;
  esac
done

# set the mode
if [ "$_sc_debug" ] ; then
  _sc_mode="DEBUG"
elif [ "$_sc_release" ] ; then
  _sc_mode="RELEASE"
else
  _sc_mode="TEST"
fi

# add the toolchain prefix if available
if [ "$_sc_toolchain" ] ; then
  _sc_cc="${_sc_toolchain}-${_sc_cc}"
  _sc_ld="${_sc_toolchain}-${_sc_ld}"
fi

# handle windows definitions
if [ "$_sc_windows" ] ; then
  windows=1
fi

# configure rpath
test "$_sc_rpath" && _sc_ldflags="$_sc_ldflags -Wl,-rpath=\$(LIBDIR)"

# config options
autoinc=""

# initialize settings
_sc_mkfile="Makefile"
_sc_n=1
_sc_proj=""
_sc_type=""
_sc_target=""
_sc_config=""
_sc_inc=""
_sc_objs=""
_sc_libs=""
_sc_install=""

# intialize the makefile
rm -f "$_sc_mkfile"

echo "all:" >> "$_sc_mkfile"
echo "test:" >> "$_sc_mkfile"
echo "clean:" >> "$_sc_mkfile"
echo "${tab}rm -f \$(CLEAN)" >> "$_sc_mkfile"
echo "Makefile: configure shellconf.sh config.args" >> "$_sc_mkfile"
echo "${tab}./configure \$(ARGS)" >> "$_sc_mkfile"
echo ".PHONY: all test clean" >> "$_sc_mkfile"

echo "ARGS     = $_sc_args"     >> "$_sc_mkfile"
echo "PREFIX   = $_sc_prefix"   >> "$_sc_mkfile"
echo "BINDIR   = $_sc_bindir"   >> "$_sc_mkfile"
echo "LIBDIR   = $_sc_libdir"   >> "$_sc_mkfile"
echo "INCDIR   = $_sc_incdir"   >> "$_sc_mkfile"
echo "SHAREDIR = $_sc_sharedir" >> "$_sc_mkfile"
echo "CC       = $_sc_cc"       >> "$_sc_mkfile"
echo "LD       = $_sc_ld"       >> "$_sc_mkfile"
echo "CFLAGS   = $_sc_cflags"   >> "$_sc_mkfile"
echo "LDFLAGS  = $_sc_ldflags"  >> "$_sc_mkfile"
