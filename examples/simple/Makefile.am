AM_CXXFLAGS = -I$(top_srcdir)/src/lib @AM_CXXFLAGS@
check_PROGRAMS = pca_example

pca_example_SOURCES = \
pca_example.cpp
pca_example_LDADD = $(top_builddir)/src/lib/.libs/libpca.a $(AM_LDFLAGS)
