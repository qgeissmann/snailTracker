AC_PREREQ([2.68])

AC_INIT([tracker],[0.01],[qgeissmann@gmail.com],[tracker],[http://www.quentelery.sourceforge.net/])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([1.10 -Wall no-define foreign])

#~ AC_CONFIG_SRCDIR([src/classifier/main.cpp])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# We cheack for compiler compatibility with std+11
AX_CXX_COMPILE_STDCXX_11([noext],[mandatory])

# We check for openmp
AC_OPENMP
# We check for opencv 2.4. if absent, stop!!
PKG_CHECK_MODULES([OPENCV],[opencv >= 2.4.3])




#~ AC_DEFINE(TRAINING_SET_IMG, "data/training-set1/", [Location of the training set])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
