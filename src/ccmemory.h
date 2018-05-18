/*
  Part of: CCMemory
  Contents: public header file
  Date: Dec 17, 2016

  Abstract

	This  public header  must be  included in  all the  source files
	using the features of the library CCMemory.

  Copyright (C) 2016, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

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
 ** Preliminary definitions.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
extern "C" {
#endif

/* The macro  CCMEM_UNUSED indicates that a  function, function argument
   or variable may potentially be unused. Usage examples:

   static int unused_function (char arg) CCMEM_UNUSED;
   int foo (char unused_argument CCMEM_UNUSED);
   int unused_variable CCMEM_UNUSED;
*/
#ifdef __GNUC__
#  define CCMEM_UNUSED		__attribute__((__unused__))
#else
#  define CCMEM_UNUSED		/* empty */
#endif

#ifndef __GNUC__
#  define __attribute__(...)	/* empty */
#endif

/* I found  the following chunk on  the Net.  (Marco Maggi;  Sun Feb 26,
   2012) */
#if defined _WIN32 || defined __CYGWIN__
#  ifdef BUILDING_DLL
#    ifdef __GNUC__
#      define ccmem_decl		__attribute__((__dllexport__)) extern
#    else
#      define ccmem_decl		__declspec(dllexport) extern
#    endif
#  else
#    ifdef __GNUC__
#      define ccmem_decl		__attribute__((__dllimport__)) extern
#    else
#      define ccmem_decl		__declspec(dllimport) extern
#    endif
#  endif
#  define ccmem_private_decl	extern
#else
#  if __GNUC__ >= 4
#    define ccmem_decl		__attribute__((__visibility__("default"))) extern
#    define ccmem_private_decl	__attribute__((__visibility__("hidden")))  extern
#  else
#    define ccmem_decl		extern
#    define ccmem_private_decl	extern
#  endif
#endif


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

ccmem_decl char const *	ccmem_version_string			(void);
ccmem_decl int		ccmem_version_interface_current		(void);
ccmem_decl int		ccmem_version_interface_revision	(void);
ccmem_decl int		ccmem_version_interface_age		(void);


/** --------------------------------------------------------------------
 ** Initialisation.
 ** ----------------------------------------------------------------- */

/*
ccmem_decl void ccmem_library_init (void)
  __attribute__((__constructor__));
*/


/** --------------------------------------------------------------------
 ** Constants and preprocessor macros.
 ** ----------------------------------------------------------------- */

#define CCMEM_PC(POINTER_TYPE, POINTER_NAME, EXPRESSION)	\
  POINTER_TYPE * POINTER_NAME = (POINTER_TYPE *) (EXPRESSION)


/** --------------------------------------------------------------------
 ** Memory allocator interface definition.
 ** ----------------------------------------------------------------- */

typedef struct ccmem_allocator_t		ccmem_allocator_t;
typedef struct ccmem_allocator_methods_t	ccmem_allocator_methods_t;

typedef void * ccmem_malloc_fun_t  (cce_destination_t L, ccmem_allocator_t const * const A, size_t size);
typedef void * ccmem_realloc_fun_t (cce_destination_t L, ccmem_allocator_t const * const A, void * ptr, size_t newsize);
typedef void * ccmem_calloc_fun_t  (cce_destination_t L, ccmem_allocator_t const * const A, size_t count, size_t eltsize);
typedef void   ccmem_free_fun_t    (cce_destination_t L, ccmem_allocator_t const * const A, void * ptr);

struct ccmem_allocator_methods_t {
  ccmem_malloc_fun_t  * const	malloc;
  ccmem_realloc_fun_t * const	realloc;
  ccmem_calloc_fun_t  * const	calloc;
  ccmem_free_fun_t    * const	free;
};

struct ccmem_allocator_t {
  ccmem_allocator_methods_t const * const	methods;
};

/* ------------------------------------------------------------------ */

static inline void *
ccmem_malloc (cce_destination_t L, ccmem_allocator_t const * const A, size_t size)
{
  return A->methods->malloc(L, A, size);
}

static inline void *
ccmem_realloc (cce_destination_t L, ccmem_allocator_t const * const A, void * ptr, size_t newsize)
{
  return A->methods->realloc(L, A, ptr, newsize);
}

static inline void *
ccmem_calloc (cce_destination_t L, ccmem_allocator_t const * const A, size_t count, size_t eltsize)
{
  return A->methods->calloc(L, A, count, eltsize);
}

static inline void
ccmem_free (cce_destination_t L, ccmem_allocator_t const * const A, void * ptr)
{
  A->methods->free(L, A, ptr);
}


/** --------------------------------------------------------------------
 ** Standard memory allocator definition.
 ** ----------------------------------------------------------------- */

ccmem_decl ccmem_allocator_t const * const ccmem_standard_allocator;

/* ------------------------------------------------------------------ */

__attribute__((__always_inline__,__nonnull__(1),__returns_nonnull__))
static inline void *
ccmem_std_malloc (cce_destination_t L, size_t size)
{
  return ccmem_standard_allocator->methods->malloc(L, ccmem_standard_allocator, size);
}

__attribute__((__always_inline__,__nonnull__(1,2),__returns_nonnull__))
static inline void *
ccmem_std_realloc (cce_destination_t L, void * ptr, size_t newsize)
{
  return ccmem_standard_allocator->methods->realloc(L, ccmem_standard_allocator, ptr, newsize);
}

__attribute__((__always_inline__,__nonnull__(1),__returns_nonnull__))
static inline void *
ccmem_std_calloc (cce_destination_t L, size_t count, size_t eltsize)
{
  return ccmem_standard_allocator->methods->calloc(L, ccmem_standard_allocator, count, eltsize);
}

__attribute__((__always_inline__,__nonnull__(1,2)))
static inline void
ccmem_std_free (cce_destination_t L, void * ptr)
{
  ccmem_standard_allocator->methods->free(L, ccmem_standard_allocator, ptr);
}


/** --------------------------------------------------------------------
 ** Memory blocks.
 ** ----------------------------------------------------------------- */

typedef struct ccmem_block_t {
  size_t	len;
  uint8_t *	ptr;
} ccmem_block_t;

/* ------------------------------------------------------------------ */

__attribute__((__always_inline__,__nonnull__(1,2)))
static inline ccmem_block_t
ccmem_block_new (cce_destination_t L, ccmem_allocator_t const * const A, size_t const len)
{
  ccmem_block_t	B = {
    .ptr	= ccmem_malloc(L, A, len),
    .len	= len
  };
  return B;
}

__attribute__((__always_inline__,__nonnull__(1,2)))
static inline void
ccmem_block_delete (cce_destination_t L, ccmem_allocator_t const * const A, ccmem_block_t B)
{
  ccmem_free(L, A, B.ptr);
}

/* ------------------------------------------------------------------ */

typedef struct ccmem_block_clean_handler_t	ccmem_block_clean_handler_t;
typedef struct ccmem_block_error_handler_t	ccmem_block_error_handler_t;

struct ccmem_block_clean_handler_t {
  cce_clean_handler_t		handler;
  ccmem_allocator_t const *	A;
  ccmem_block_t			B;
};

struct ccmem_block_error_handler_t {
  cce_error_handler_t		handler;
  ccmem_allocator_t const *	A;
  ccmem_block_t			B;
};

ccmem_decl void ccmem_block_register_clean_handler (cce_destination_t L, ccmem_block_clean_handler_t * H,
						    ccmem_allocator_t const * const A, ccmem_block_t B)
  __attribute__((__nonnull__(1,2,3)));

ccmem_decl void ccmem_block_register_error_handler (cce_destination_t L, ccmem_block_error_handler_t * H,
						    ccmem_allocator_t const * const A, ccmem_block_t B)
  __attribute__((__nonnull__(1,2,3)));

ccmem_decl ccmem_block_t ccmem_block_new_guarded_clean (cce_destination_t L, ccmem_block_clean_handler_t * H,
							ccmem_allocator_t const * const A, size_t const len)
  __attribute__((__nonnull__(1,2,3)));

ccmem_decl ccmem_block_t ccmem_block_new_guarded_error (cce_destination_t L, ccmem_block_error_handler_t * H,
							ccmem_allocator_t const * const A, size_t const len)
  __attribute__((__nonnull__(1,2,3)));

#define ccmem_block_new_guarded(L,H,A,len) \
  _Generic((H), \
	   ccmem_block_clean_handler_t	*: ccmem_block_new_guarded_clean, \
	   ccmem_block_error_handler_t	*: ccmem_block_new_guarded_error)(L,H,A,len)

/* ------------------------------------------------------------------ */

__attribute__((__always_inline__,__const__))
static inline bool
ccmem_block_is_empty (ccmem_block_t B)
{
  return (0 == B.len);
}

__attribute__((__always_inline__,__const__))
static inline bool
ccmem_block_is_null (ccmem_block_t B)
{
  return (NULL == B.ptr);
}

/* ------------------------------------------------------------------ */

__attribute__((__always_inline__))
static inline void
ccmem_block_memset (ccmem_block_t B, uint8_t octet)
{
  memset(B.ptr, octet, B.len);
}

__attribute__((__always_inline__))
static inline void
ccmem_block_clean (ccmem_block_t B)
{
  ccmem_block_memset(B, 0);
}

__attribute__((__always_inline__,__nonnull__(1)))
static inline void
ccmem_block_nullify (ccmem_block_t * B)
{
  B->ptr = NULL;
  B->len = 0;
}

ccmem_decl ccmem_block_t ccmem_block_shift (ccmem_block_t B, ssize_t offset, size_t dim)
  __attribute__((__const__));

ccmem_decl ccmem_block_t ccmem_block_difference (ccmem_block_t A, ccmem_block_t B)
  __attribute__((__const__));


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCMEMORY_H */

/* end of file */
