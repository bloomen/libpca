AM_CXXFLAGS = -I$(top_srcdir)/src/lib @AM_CXXFLAGS@
check_PROGRAMS = unittest

EXTRA_DIST = \
test_pca.h \
test_utils.h \
unittest.hpp \
utils.hpp

unittest_SOURCES = \
main.cpp \
testcases.cpp \
test_pca.cpp \
test_utils.cpp
unittest_LDADD = $(top_builddir)/src/lib/.libs/libpca.a $(AM_LDFLAGS)
