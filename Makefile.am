## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL			\
			  meta/slackstuff/config.in	\
			  meta/slackware/slack-desc.in
dist_doc_DATA		= README COPYING
AM_CFLAGS		= $(MMUX_CFLAGS) -I$(srcdir)/src

## --------------------------------------------------------------------

EXTRA_DIST		+= meta/scripts/ccmemory.pc.in
pkgconfigdir		= $(libdir)/pkgconfig
nodist_pkgconfig_DATA	= meta/scripts/ccmemory.pc

## --------------------------------------------------------------------

AM_MAKEINFOFLAGS	= --no-split
AM_MAKEINFOHTMLFLAGS	= --split=node -c WORDS_IN_PAGE=0 \
	-c PRE_BODY_CLOSE="<p>This document describes version <tt>$(PACKAGE_VERSION)</tt> of <em>$(PACKAGE_NAME)</em>.</p>"

info_TEXINFOS		= doc/ccmemory.texi
doc_ccmemory_TEXINFOS	= \
	doc/macros.texi		\
	doc/lgpl-3.0.texi	\
	doc/fdl-1.3.texi


#### libraries

ccmemory_CURRENT	= @ccmemory_VERSION_INTERFACE_CURRENT@
ccmemory_REVISION	= @ccmemory_VERSION_INTERFACE_REVISION@
ccmemory_AGE		= @ccmemory_VERSION_INTERFACE_AGE@

include_HEADERS		= src/ccmemory.h

lib_LTLIBRARIES		= libccmemory.la
libccmemory_la_CPPFLAGS	= $(CCEXCEPTIONS_CFLAGS)
libccmemory_la_LDFLAGS	= -version-info $(ccmemory_CURRENT):$(ccmemory_REVISION):$(ccmemory_AGE) \
			  $(CCEXCEPTIONS_LIBS)
libccmemory_la_SOURCES	= \
	src/ccmemory.h				\
	src/ccmemory-internals.h		\
	src/ccmemory-version.c			\
	src/ccmemory-standard-allocator.c	\
	src/ccmemory-exception-handlers.c	\
	src/ccmemory-blocks.c


#### tests

check_PROGRAMS	= \
	tests/version			\
	tests/standard-allocator	\
	tests/exception-handlers	\
	tests/memory-blocks		\
	tests/ascii-strings		\
	tests/asciiz-strings

if CCSEMVER_TESTING_ENABLED
TESTS				= $(check_PROGRAMS)
endif

ccmemory_tests_cppflags		= -I$(srcdir)/src $(CCEXCEPTIONS_CFLAGS) $(CCTESTS_CFLAGS)
ccmemory_tests_ldadd		= libccmemory.la $(CCEXCEPTIONS_LIBS) $(CCTESTS_LIBS)

tests_version_CPPFLAGS			= $(ccmemory_tests_cppflags)
tests_version_LDADD			= $(ccmemory_tests_ldadd)

tests_standard_allocator_CPPFLAGS	= $(ccmemory_tests_cppflags)
tests_standard_allocator_LDADD		= $(ccmemory_tests_ldadd)

tests_exception_handlers_CPPFLAGS	= $(ccmemory_tests_cppflags)
tests_exception_handlers_LDADD		= $(ccmemory_tests_ldadd)

tests_memory_blocks_CPPFLAGS		= $(ccmemory_tests_cppflags)
tests_memory_blocks_LDADD		= $(ccmemory_tests_ldadd)

tests_ascii_strings_CPPFLAGS		= $(ccmemory_tests_cppflags)
tests_ascii_strings_LDADD		= $(ccmemory_tests_ldadd)

tests_asciiz_strings_CPPFLAGS		= $(ccmemory_tests_cppflags)
tests_asciiz_strings_LDADD		= $(ccmemory_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done


#### testing under GDB

$(builddir)/.gdbinit:
	ln -sv "$(top_srcdir)/gdbinit" "$(builddir)/.gdbinit"

# This rule is meant to be used as:
#
#   $ make all gdb-test FILE=tests/inspection CFLAGS='-g'
#
# after having created an appropriate file "$(top_srcdir)/.gdbinit".
#
gdb-test: $(builddir)/.gdbinit
	libtool --mode=execute gdb $$FILE


#### Static analysis with Clang's Static Analyzer
#
# See the documentation for the command line tool at:
#
#    <http://clang-analyzer.llvm.org/scan-build.html>
#
# To run the tool we must do:
#
#    $ make clean
#    $ make clang-static-analysis
#
# The  program  "scan-build"  works  by  overriding  the  CC  and  CXX
# environment variables.  Automake defines CXX as:
#
#    CXX = g++ -std=c++11
#
# so we override CXXFLAGS to include the language specification.
#

.PHONY: clang-static-analysis

clang-static-analysis:
	scan-build make CXXFLAGS='-std=c++11'

### end of file
