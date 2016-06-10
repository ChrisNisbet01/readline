AC_PREREQ(2.61)
AC_INIT([readline_cn], [1.0], [nisbet@ihug.co.nz])
AC_CONFIG_SRCDIR([src/read_line.c])
AC_CONFIG_HEADER([config.h])
AC_CONFIG_FILES([Makefile src/Makefile include/Makefile])

# Prepare automake
AM_INIT_AUTOMAKE([no-dist-gzip dist-xz])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Checks for libraries.
LT_INIT

AM_CFLAGS="-std=gnu99 $inline_cflags -Wall -Werror"
AC_SUBST(AM_CFLAGS)

AC_OUTPUT

