/*
  Part of: CCMemory
  Contents: public header file
  Date: Sat Dec 17, 2016

  Abstract



  Copyright (C) 2016 Marco Maggi <marco.maggi-ipsu@poste.it>

  This program is  free software: you can redistribute  it and/or modify
  it  under the  terms  of  the GNU  Lesser  General  Public License  as
  published by  the Free  Software Foundation, either  version 3  of the
  License, or (at your option) any later version.

  This program  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  General Public License for more details.

  You  should have  received a  copy of  the GNU  Lesser General  Public
  License     along    with     this    program.      If    not,     see
  <http://www.gnu.org/licenses/>.
*/

#ifndef CCMEMORY_H
#define CCMEMORY_H 1


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* The  macro  CCMEMORY_UNUSED  indicates  that a  function,  function
   argument or variable may potentially be unused. Usage examples:

   static int unused_function (char arg) CCMEMORY_UNUSED;
   int foo (char unused_argument CCMEMORY_UNUSED);
   int unused_variable CCMEMORY_UNUSED;
*/
#ifdef __GNUC__
#  define CCMEMORY_UNUSED		__attribute__((unused))
#else
#  define CCMEMORY_UNUSED		/* empty */
#endif

#ifndef __GNUC__
#  define __attribute__(...)	/* empty */
#endif

/* I found  the following chunk on  the Net.  (Marco Maggi;  Sun Feb 26,
   2012) */
#if defined _WIN32 || defined __CYGWIN__
#  ifdef BUILDING_DLL
#    ifdef __GNUC__
#      define ccmemory_decl		__attribute__((dllexport))
#    else
#      define ccmemory_decl		__declspec(dllexport)
#    endif
#  else
#    ifdef __GNUC__
#      define ccmemory_decl		__attribute__((dllimport))
#    else
#      define ccmemory_decl		__declspec(dllimport)
#    endif
#  endif
#  define ccmemory_private_decl	extern
#else
#  if __GNUC__ >= 4
#    define ccmemory_decl		__attribute__((visibility ("default")))
#    define ccmemory_private_decl	__attribute__((visibility ("hidden")))
#  else
#    define ccmemory_decl		extern
#    define ccmemory_private_decl	extern
#  endif
#endif


/** --------------------------------------------------------------------
 ** Constants.
 ** ----------------------------------------------------------------- */




/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

ccmemory_decl const char *	ccm_version_string		(void);
ccmemory_decl int		ccm_version_interface_current	(void);
ccmemory_decl int		ccm_version_interface_revision	(void);
ccmemory_decl int		ccm_version_interface_age	(void);


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCMEMORY_H */

/* end of file */
