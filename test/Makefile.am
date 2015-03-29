AM_CXXFLAGS = -I$(top_srcdir)/src @AM_CXXFLAGS@
check_PROGRAMS = unittest

unittest_SOURCES = \
main.cpp \
testcases.cpp \
test_pca.cpp \
test_utils.cpp
unittest_LDADD = $(top_builddir)/src/lib/.libs/libunittest.a $(AM_LDFLAGS)
