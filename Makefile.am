## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL configure.sh
dist_doc_DATA		= README COPYING
AM_CFLAGS		= -Wall

## --------------------------------------------------------------------

EXTRA_DIST		+= meta/scripts/ccmemory.pc.in
pkgconfigdir		= $(libdir)/pkgconfig
nodist_pkgconfig_DATA	= meta/scripts/ccmemory.pc

## --------------------------------------------------------------------

AM_MAKEINFOFLAGS	= --no-split

info_TEXINFOS		= doc/ccmemory.texi
doc_ccmemory_TEXINFOS	= \
	doc/macros.texi		\
	doc/lgpl-3.0.texi	\
	doc/fdl-1.3.texi

#page
#### libraries

ccmemory_CURRENT	= @ccmemory_VERSION_INTERFACE_CURRENT@
ccmemory_REVISION	= @ccmemory_VERSION_INTERFACE_REVISION@
ccmemory_AGE		= @ccmemory_VERSION_INTERFACE_AGE@

include_HEADERS		= src/ccmemory.h

lib_LTLIBRARIES		= libccmemory.la
libccmemory_la_LDFLAGS= -version-info $(ccmemory_CURRENT):$(ccmemory_REVISION):$(ccmemory_AGE)
libccmemory_la_SOURCES= \
	src/ccmemory.h		\
	src/ccmemory-internals.h	\
	src/ccmemory-version.c

#page
#### tests

check_PROGRAMS	= \
	tests/version

TESTS				= $(check_PROGRAMS)

ccmemory_tests_cppflags		= -I$(srcdir)/src
ccmemory_tests_ldadd		= libccmemory.la

tests_version_CPPFLAGS		= $(ccmemory_tests_cppflags)
tests_version_LDADD		= $(ccmemory_tests_ldadd)

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done

### end of file
