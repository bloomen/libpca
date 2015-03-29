dnl
dnl Copyright 2007,2008 Free Software Foundation, Inc.
dnl
dnl This file is part of GNU Radio
dnl
dnl GNU Radio is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; either version 3, or (at your option)
dnl any later version.
dnl
dnl GNU Radio is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
dnl GNU General Public License for more details.
dnl
dnl You should have received a copy of the GNU General Public License
dnl along with GNU Radio; see the file COPYING. If not, write to
dnl the Free Software Foundation, Inc., 51 Franklin Street,
dnl Boston, MA 02110-1301, USA.
dnl
dnl Configure paths for library armadillo.
dnl
dnl GR_ARMADILLO([ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND]])
dnl
dnl Test for library armadillo, set ARMADILLO_CFLAGS and ARMADILLO_LIBS if found.
dnl
AC_DEFUN([GR_ARMADILLO],
[
dnl ARMADILLO Library Version
ARMADILLO_LIBRARY1=-larmadillo
dnl Save the environment
AC_LANG_PUSH(C++)
armadillo_save_CPPFLAGS="$CPPFLAGS"
armadillo_save_LIBS="$LIBS"
libarmadillo_ok=yes
dnl ARMADILLO Info
dnl Allow user to specify where armadillo files are
AC_ARG_WITH([armadillo-libdir],
[ --with-armadillo-libdir=path Prefix where armadillo library is installed (optional)],
[armadillo_libdir="$withval"], [armadillo_libdir=""])
AC_ARG_WITH([armadillo-incdir],
[ --with-armadillo-incdir=path Prefix where armadillo include files are (optional)],
[armadillo_incdir="$withval"], [armadillo_incdir=""])
AC_ARG_WITH([armadillo-lib],
[ --with-armadillo-lib=library armadillo library name (optional)],
[armadillo_lib="$withval"], [armadillo_lib=""])
dnl Check for presence of header files
dnl if not user-specified, try the first include dir (Ubuntu), then
dnl try the second include dir (Fedora)
CPPFLAGS="$CPPFLAGS"
dnl if not set by user
if test "$armadillo_incdir" = "" ; then
dnl check <armadillo> (as in Ubuntu)
AC_CHECK_HEADER(
[armadillo],
[armadillo_armadillo_h=yes],
[armadillo_armadillo_h=no]
)
dnl If it was found, set the flags and move on
if test "$armadillo_armadillo_h" = "yes" ; then
ARMADILLO_CFLAGS="$ARMADILLO_CFLAGS -I/usr/include"
else
dnl otherwise, armadillo.h wasn't found, so set the flag to no
libarmadillo_ok=no
fi
else
dnl Using the user-specified include directory
ARMADILLO_CFLAGS="$ARMADILLO_CFLAGS -I$armadillo_incdir"
AC_CHECK_HEADER(
[$armadillo_incdir/armadillo],
[],
[libarmadillo_ok=no])
fi
dnl Don't bother going on if we can't find the headers
if test "$libarmadillo_ok" = "yes" ; then
dnl Check for armadillo library (armadillo)
dnl User-defined ARMADILLO library path
if test "$armadillo_libdir" != "" ; then
ARMADILLO_LIBS="-L$armadillo_libdir $ARMADILLO_LIBS"
fi
dnl temporarily set these so the AC_CHECK_LIB works
CPPFLAGS="$CPPFLAGS $ARMADILLO_CFLAGS"
LIBS="$armadillo_save_LIBS $ARMADILLO_LIBS -larmadillo"
dnl If the user specified a armadillo library name, use it here
if test "$armadillo_lib" != "" ; then
AC_CHECK_LIB([$armadillo_lib], [main], [libarmadillo_ok=yes], [libarmadillo_ok=no])
else
dnl Check for 'main' in libarmadillo (Fedora)
AC_CHECK_LIB([armadillo], [main], [libarmadillo_ok=yes], [libarmadillo_ok=no])
dnl If library found properly, set the flag and move on
if test "$libarmadillo_ok" = "yes" ; then
ARMADILLO_LIBS="$ARMADILLO_LIBS -larmadillo"
else
AC_MSG_RESULT([Could not link to libarmadillo.so])
fi
fi
else
AC_MSG_RESULT([Could not find armadillo headers])
fi
dnl Restore saved variables
LIBS="$armadillo_save_LIBS"
CPPFLAGS="$armadillo_save_CPPFLAGS"
AC_LANG_POP
dnl Execute user actions
if test "x$libarmadillo_ok" = "xyes" ; then
ifelse([$1], , :, [$1])
else
ARMADILLO_CFLAGS=""
ARMADILLO_LIBDIRS=""
ifelse([$2], , :, [$2])
fi
dnl Export our variables
AC_SUBST(ARMADILLO_CFLAGS)
AC_SUBST(ARMADILLO_LIBS)
])
