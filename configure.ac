AC_PREREQ([2.64])
AC_CONFIG_MACRO_DIR([m4])

AC_INIT([My Bittorrent], [1.0], [], [my-bittorrent], [acu.epita.fr])

AM_INIT_AUTOMAKE([foreign subdir-objects -Wall -Werror])

m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

LT_INIT
AC_PROG_CC_C99

AX_CHECK_OPENSSL()

LIBCURL_CHECK_CONFIG(, [7], ,
                     [AC_MSG_ERROR([Missing required libcurl >= 7])])

# check for libraries
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([stdlib.h])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

AC_OUTPUT
