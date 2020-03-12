## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL			\
			  meta/slackware/slack-desc.in
dist_doc_DATA		= README COPYING
AM_CPPFLAGS		= -I$(srcdir)/src
AM_CFLAGS		= $(MMUX_CFLAGS)
CLEANFILES		=

## --------------------------------------------------------------------

nodist_pkgconfig_DATA	= meta/scripts/ccmemory.pc


#### documentation

EXTRA_DIST		+= doc/ccmemory.style.css
CLEANFILES		+= doc/ccmemory.css

AM_MAKEINFOFLAGS	= --no-split
AM_MAKEINFOHTMLFLAGS	= --split=node -c WORDS_IN_PAGE=0 --css-ref=ccmemory.css \
	-c PRE_BODY_CLOSE="<p>This document describes version <tt>$(PACKAGE_VERSION)</tt> of <em>$(PACKAGE_NAME)</em>.</p>"

info_TEXINFOS		= doc/ccmemory.texi
doc_ccmemory_TEXINFOS	= \
	doc/macros.texi			\
	doc/overview-linking.texi	\
	doc/overview-ccexceptions.texi	\
	doc/lgpl-3.0.texi		\
	doc/fdl-1.3.texi

## --------------------------------------------------------------------

doc/ccmemory.html/$(am__dirstamp): doc/$(am__dirstamp)
	@$(MKDIR_P) doc/ccmemory.html/
	@: > doc/ccmemory.html/$(am__dirstamp)

doc/ccmemory.html/ccmemory.css: doc/ccmemory.style.css doc/ccmemory.html/$(am__dirstamp)
	$(INSTALL) -m 0444 "$(top_srcdir)/doc/ccmemory.style.css" "$(builddir)/doc/ccmemory.html/ccmemory.css"

## --------------------------------------------------------------------

html-local: doc/ccmemory.html/ccmemory.css

install-html-local:
	$(MKDIR_P) "$(DESTDIR)$(htmldir)/ccmemory.html/"
	$(INSTALL) -m 0444 $(builddir)/doc/ccmemory.html/ccmemory.css "$(DESTDIR)$(htmldir)/ccmemory.html/"


#### libraries

ccmemory_CURRENT	= @ccmemory_VERSION_INTERFACE_CURRENT@
ccmemory_REVISION	= @ccmemory_VERSION_INTERFACE_REVISION@
ccmemory_AGE		= @ccmemory_VERSION_INTERFACE_AGE@
ccmemory_LINKER_VERSION	= -version-info $(ccmemory_CURRENT):$(ccmemory_REVISION):$(ccmemory_AGE)

include_HEADERS		= src/ccmemory.h

lib_LTLIBRARIES		= libccmemory.la
libccmemory_la_CPPFLAGS	= $(AM_CPPFLAGS)
libccmemory_la_CFLAGS	= $(AM_CFLAGS) $(CCEXCEPTIONS_CFLAGS)
libccmemory_la_LDFLAGS	= $(ccmemory_LINKER_VERSION) $(AM_LDFLAGS) $(CCEXCEPTIONS_LIBS)
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

if MMUX_TESTING_ENABLED
TESTS				= $(check_PROGRAMS)
endif

ccmemory_tests_cppflags		= $(AM_CPPFLAGS)
ccmemory_tests_cflags		= $(AM_CFLAGS) $(CCEXCEPTIONS_CFLAGS) $(CCTESTS_CFLAGS)
ccmemory_tests_ldadd		= $(AM_LDFLAGS) libccmemory.la $(CCEXCEPTIONS_LIBS) $(CCTESTS_LIBS)

## --------------------------------------------------------------------

tests_version_CPPFLAGS			= $(ccmemory_tests_cppflags)
tests_version_CFLAGS			= $(ccmemory_tests_cflags)
tests_version_LDADD			= $(ccmemory_tests_ldadd)

tests_standard_allocator_CPPFLAGS	= $(ccmemory_tests_cppflags)
tests_standard_allocator_CFLAGS		= $(ccmemory_tests_cflags)
tests_standard_allocator_LDADD		= $(ccmemory_tests_ldadd)

tests_exception_handlers_CPPFLAGS	= $(ccmemory_tests_cppflags)
tests_exception_handlers_CFLAGS		= $(ccmemory_tests_cflags)
tests_exception_handlers_LDADD		= $(ccmemory_tests_ldadd)

tests_memory_blocks_CPPFLAGS		= $(ccmemory_tests_cppflags)
tests_memory_blocks_CFLAGS		= $(ccmemory_tests_cflags)
tests_memory_blocks_LDADD		= $(ccmemory_tests_ldadd)

tests_ascii_strings_CPPFLAGS		= $(ccmemory_tests_cppflags)
tests_ascii_strings_CFLAGS		= $(ccmemory_tests_cflags)
tests_ascii_strings_LDADD		= $(ccmemory_tests_ldadd)

tests_asciiz_strings_CPPFLAGS		= $(ccmemory_tests_cppflags)
tests_asciiz_strings_CFLAGS		= $(ccmemory_tests_cflags)
tests_asciiz_strings_LDADD		= $(ccmemory_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done


#### common rules

include $(top_srcdir)/meta/automake/mmux-makefile-rules

### end of file
