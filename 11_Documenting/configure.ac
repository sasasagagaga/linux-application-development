#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([guesser], [0.0.0], [alexander.v.chernyshev@mail.ru])
AM_INIT_AUTOMAKE([foreign])
DX_INIT_DOXYGEN([guesser])
AC_CONFIG_SRCDIR([guesser.c])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PATH_PROG([HELP2MAN], [help2man])

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([libintl.h locale.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([setlocale strcspn])

AC_CONFIG_FILES([Makefile Doxyfile])
AC_OUTPUT

