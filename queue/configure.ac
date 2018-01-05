dnl Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)

AC_INIT([ogs-crawler], [3.2],[simon.martin@etherapeutics.co.uk])
AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])

# Ensure long file access
CXXFLAGS="$CXXFLAGS -std=c++0x `getconf LFS_CFLAGS` -lrt "

# add checking message
AC_MSG_CHECKING(whether to build with debug information)
# create configure parameter and init variable $debuger
debuger=no
AC_ARG_ENABLE(debug,
	AC_HELP_STRING(
	    [--enable-debug],
	    [enable debug data generation (def=no)]
	),
  	debuger="$enableval"
)

# resulting value to screen (to complete checking message)
AC_MSG_RESULT($debuger)

# set DEBUG flag and introduce additional compiler flags
if test x"$debuger" = x"yes"; then
	AC_DEFINE(DEBUG,[],[whether to build with debug information])
	CXXFLAGS="$CXXFLAGS -g -pg -Wall -Weffc++ -Wextra -Werror -Wstrict-null-sentinel -Wp,-D_FORTIFY_SOURCE=2 "
fi

# add checking message
AC_MSG_CHECKING(whether to build with binary retention)
# create configure parameter and init variable $binars
binars=yes
AC_ARG_ENABLE(binaries,
	AC_HELP_STRING([--enable-binaries], [enable binary storage (def=yes)] ),
  	binars="$enableval"
)

# resulting value to screen (to complete checking message)
AC_MSG_RESULT($binars)

# set BINARIES flag
if test x"$binars" = x"yes"; then
	AC_DEFINE(BINARIES,[],[whether to build with binary retention])
	AC_SUBST(BINARIES)
fi

AM_INIT_AUTOMAKE([1.9 subdir-objects])
AM_CONFIG_HEADER(config.h:config.in)
AM_CONDITIONAL(BINARIES, test x"$binars" = x"yes")
AM_MAINTAINER_MODE([enable])

# Checks for programs
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_CXX
AC_PROG_LIBTOOL

# Checks for header files
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h valgrind/valgrind.h regex.h pthread.h getopt.h stdarg.h])
AC_CHECK_HEADERS([xapian.h magic.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_CHECK_SIZEOF(size_t)
AC_TYPE_SIZE_T
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int16_t)
AC_CHECK_SIZEOF(uint16_t)
AC_CHECK_SIZEOF(int32_t)
AC_CHECK_SIZEOF(uint32_t)
AC_CHECK_SIZEOF(int64_t)
AC_CHECK_SIZEOF(off_t)
AC_CHECK_SIZEOF(off64_t)
AC_CHECK_SIZEOF(time_t)
AC_HEADER_TIME

# Checks for libraries
LIBCURL_CHECK_CONFIG

AC_CHECK_LIB(pthread, pthread_create)
AC_CHECK_LIB(l, yywrap)
AC_CHECK_LIB(fl, yywrap)
AC_CHECK_LIB(y, yyerror)
AC_CHECK_LIB(z, deflate)
AC_CHECK_LIB(magic, magic_open)

# Checks for pkg-check modules
#PKG_CHECK_MODULES([OGS], [ogscore ogsinet])
#AC_SUBST(OGS_CFLAGS)
#AC_SUBST(OGS_LIBS)

#PKG_CHECK_MODULES([SQLITE], [sqlite3])
#AC_SUBST(SQLITE_CFLAGS)
#AC_SUBST(SQLITE_LIBS)

#PKG_CHECK_MODULES([LUA], [lua5.1],, [ PKG_CHECK_MODULES([LUA], [lua-5.1]) ])
#AC_SUBST(LUA_CFLAGS)
#AC_SUBST(LUA_LIBS)

#PKG_CHECK_MODULES([XML], [libxml-2.0])
#AC_SUBST(XML_CFLAGS)
#AC_SUBST(XML_LIBS)

#PKG_CHECK_MODULES([STXXL], [stxxl_debug])
#AC_SUBST(STXXL_CFLAGS)
#AC_SUBST(STXXL_LIBS)

PKG_CHECK_MODULES([HTTPD], [libmicrohttpd])
AC_SUBST(HTTPD_CFLAGS)
AC_SUBST(HTTPD_LIBS)

PKG_CHECK_MODULES([RABBITMQ], [librabbitmq])
AC_SUBST(RABBITMQ_CFLAGS)
AC_SUBST(RABBITMQ_LIBS)

# Checks for library functions
AC_FUNC_SELECT_ARGTYPES
AC_FUNC_VPRINTF
AC_FUNC_SETVBUF_REVERSED
AC_CHECK_FUNCS([ftruncate gethostbyname inet_ntoa memset select socket strcasecmp strerror])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
