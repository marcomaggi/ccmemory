## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS		= -I meta/autotools
AUTOMAKE_OPTIONS	= foreign
EXTRA_DIST		= INSTALL configure.sh		\
			  meta/slackstuff/config.in	\
			  meta/slackware/slack-desc.in
dist_doc_DATA		= README COPYING
AM_CFLAGS		= -I$(srcdir)/src $(AX_CFLAGS)

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
libccmemory_la_CPPFLAGS	= $(CCEXCEPTIONS_CFLAGS)
libccmemory_la_LDFLAGS	= -version-info $(ccmemory_CURRENT):$(ccmemory_REVISION):$(ccmemory_AGE) \
			  $(CCEXCEPTIONS_LIBS)
libccmemory_la_SOURCES	= \
	src/ccmemory.h				\
	src/ccmemory-internals.h		\
	src/ccmemory-version.c			\
	src/ccmemory-standard-allocator.c

#page
#### tests

check_PROGRAMS	= \
	tests/version			\
	tests/standard-allocator

if CCSEMVER_TESTING_ENABLED
TESTS				= $(check_PROGRAMS)
endif

ccmemory_tests_cppflags		= -I$(srcdir)/src $(CCEXCEPTIONS_CFLAGS) $(CCTESTS_CFLAGS)
ccmemory_tests_ldadd		= libccmemory.la $(CCEXCEPTIONS_LIBS) $(CCTESTS_LIBS)

tests_version_CPPFLAGS			= $(ccmemory_tests_cppflags)
tests_version_LDADD			= $(ccmemory_tests_ldadd)

tests_standard_allocator_CPPFLAGS	= $(ccmemory_tests_cppflags)
tests_standard_allocator_LDADD		= $(ccmemory_tests_ldadd)

## --------------------------------------------------------------------

installcheck-local: $(TESTS)
	@for f in $(TESTS); do $(builddir)/$$f; done

#page
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

#page
#### Coverity support
#
# We can visit the project site at:
#
#    <https://scan.coverity.com/projects/marcomaggi-ccmemory>
#
# Remember to  put in  the PATH  the directory  in which  the Coverity
# tools are installed; for example:
#
#    $ export PATH=/opt/coverity/8.7.0/cov-analysis-linux64-8.7.0/bin:$PATH
#
# The tool  will use  a temporary directory  "$(builddir)/cov-int"; we
# must *not* change this pathname.
#
# To generate and submit a report we must do:
#
#    $ make clean
#    $ make coverity-scan
#    $ make coverity-archive
#    $ make coverity-upload
#
# after "make  coverity-scan" we should  check that the last  lines of
# the file "cov-int/build-log.txt" report a successful build:
#
#    $ tail cov-int/build-log.txt
#
# NOTE If  your Linux kernel  is at version 4.8  or above you  have to
# first issue the command:
#
#    $ sudo /sbin/sysctl vsyscall=emulate
#
# to make the Coverity tools work.
#

CCMEMORY_COVERITY_TOKEN_FILE = /home/unsafe-users/marco-devel/.coverity-ccmemory-token

.PHONY: coverity-scan coverity-archive coverity-upload

coverity-scan:
	cov-build --dir cov-int make -j 4

coverity-archive:
	tar caf ccmemory.xz cov-int

coverity-upload:
	curl --form token=$$(<$(CCMEMORY_COVERITY_TOKEN_FILE)) \
	     --form email='marco.maggi-ipsu@poste.it' \
	     --form file='@ccmemory.xz' \
	     --form version="$(VERSION)" \
	     --form description="A build description" \
	     'https://scan.coverity.com/builds?project=marcomaggi%2Fccmemory'

#page
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
