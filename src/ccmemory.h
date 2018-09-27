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
#include <stdint.h>
#include <unistd.h>
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

typedef void * ccmem_malloc_fun_t  (cce_destination_t L, ccmem_allocator_t const * A, size_t size);
typedef void * ccmem_realloc_fun_t (cce_destination_t L, ccmem_allocator_t const * A, void * ptr, size_t newsize);
typedef void * ccmem_calloc_fun_t  (cce_destination_t L, ccmem_allocator_t const * A, size_t count, size_t eltsize);
typedef void   ccmem_free_fun_t                         (ccmem_allocator_t const * A, void * ptr);

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

__attribute__((__always_inline__,__nonnull__(1,2),__returns_nonnull__))
static inline void *
ccmem_malloc (cce_destination_t L, ccmem_allocator_t const * A, size_t size)
{
  return A->methods->malloc(L, A, size);
}

__attribute__((__always_inline__,__nonnull__(1,2,3),__returns_nonnull__))
static inline void *
ccmem_realloc (cce_destination_t L, ccmem_allocator_t const * A, void * ptr, size_t newsize)
{
  return A->methods->realloc(L, A, ptr, newsize);
}

__attribute__((__always_inline__,__nonnull__(1,2),__returns_nonnull__))
static inline void *
ccmem_calloc (cce_destination_t L, ccmem_allocator_t const * A, size_t count, size_t eltsize)
{
  return A->methods->calloc(L, A, count, eltsize);
}

__attribute__((__always_inline__,__nonnull__(1,2)))
static inline void
ccmem_free (ccmem_allocator_t const * A, void * ptr)
{
  A->methods->free(A, ptr);
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

__attribute__((__always_inline__,__nonnull__(1)))
static inline void
ccmem_std_free (void * ptr)
{
  ccmem_standard_allocator->methods->free(ccmem_standard_allocator, ptr);
}


/** --------------------------------------------------------------------
 ** Memory allocator: exception handlers.
 ** ----------------------------------------------------------------- */

typedef struct ccmem_clean_handler_t	ccmem_clean_handler_t;
typedef struct ccmem_error_handler_t	ccmem_error_handler_t;

struct ccmem_clean_handler_t {
  cce_clean_handler_t		handler;
  ccmem_allocator_t const *	allocator;
};

struct ccmem_error_handler_t {
  cce_error_handler_t		handler;
  ccmem_allocator_t const *	allocator;
};

/* ------------------------------------------------------------------ */

ccmem_decl void * ccmem_malloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
					      ccmem_allocator_t const * A, size_t size)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccmem_decl void * ccmem_malloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
					      ccmem_allocator_t const * A, size_t size)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

#define ccmem_malloc_guarded(L,P_H,A,size) \
  _Generic((P_H),	\
	   ccmem_clean_handler_t	*: ccmem_malloc_guarded_clean, \
	   ccmem_error_handler_t	*: ccmem_malloc_guarded_error)(L,P_H,A,size)

/* ------------------------------------------------------------------ */

ccmem_decl void * ccmem_calloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
					      ccmem_allocator_t const * A, size_t count, size_t eltsize)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

ccmem_decl void * ccmem_calloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
					      ccmem_allocator_t const * A, size_t count, size_t eltsize)
  __attribute__((__nonnull__(1,2,3),__returns_nonnull__));

#define ccmem_calloc_guarded(L,P_H,A,count,eltsize) \
  _Generic((P_H),	\
	   ccmem_clean_handler_t	*: ccmem_calloc_guarded_clean, \
	   ccmem_error_handler_t	*: ccmem_calloc_guarded_error)(L,P_H,A,count,eltsize)

/* ------------------------------------------------------------------ */

ccmem_decl void * ccmem_realloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
					       ccmem_allocator_t const * A, void * P, size_t newsize)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

ccmem_decl void * ccmem_realloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
					       ccmem_allocator_t const * A, void * P, size_t newsize)
  __attribute__((__nonnull__(1,2,3,4),__returns_nonnull__));

#define ccmem_realloc_guarded(L,P_H,A,P,newsize) \
  _Generic((P_H),	\
	   ccmem_clean_handler_t	*: ccmem_realloc_guarded_clean, \
	   ccmem_error_handler_t	*: ccmem_realloc_guarded_error)(L,P_H,A,P,newsize)


/** --------------------------------------------------------------------
 ** Standard memory allocator: exception handlers.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1,2),__returns_nonnull__,__always_inline__))
static inline void *
ccmem_std_malloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H, size_t size)
{
  return ccmem_malloc_guarded(L, P_H, ccmem_standard_allocator, size);
}

__attribute__((__nonnull__(1,2),__returns_nonnull__,__always_inline__))
static inline void *
ccmem_std_malloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H, size_t size)
{
  return ccmem_malloc_guarded(L, P_H, ccmem_standard_allocator, size);
}

#define ccmem_std_malloc_guarded(L,P_H,size) \
  _Generic((P_H),	\
	   ccmem_clean_handler_t	*: ccmem_std_malloc_guarded_clean, \
	   ccmem_error_handler_t	*: ccmem_std_malloc_guarded_error)(L,P_H,size)

/* ------------------------------------------------------------------ */

__attribute__((__nonnull__(1,2),__returns_nonnull__,__always_inline__))
static inline void *
ccmem_std_calloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H, size_t count, size_t eltsize)
{
  return ccmem_calloc_guarded(L, P_H, ccmem_standard_allocator, count, eltsize);
}

__attribute__((__nonnull__(1,2),__returns_nonnull__,__always_inline__))
static inline void *
ccmem_std_calloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H, size_t count, size_t eltsize)
{
  return ccmem_calloc_guarded(L, P_H, ccmem_standard_allocator, count, eltsize);
}

#define ccmem_std_calloc_guarded(L,P_H,count,eltsize) \
  _Generic((P_H),	\
	   ccmem_clean_handler_t	*: ccmem_std_calloc_guarded_clean, \
	   ccmem_error_handler_t	*: ccmem_std_calloc_guarded_error)(L,P_H,count,eltsize)

/* ------------------------------------------------------------------ */

__attribute__((__nonnull__(1,2,3),__returns_nonnull__,__always_inline__))
static inline void *
ccmem_std_realloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H, void * P, size_t newsize)
{
  return ccmem_realloc_guarded(L, P_H, ccmem_standard_allocator, P, newsize);
}

__attribute__((__nonnull__(1,2,3),__returns_nonnull__,__always_inline__))
static inline void *
ccmem_std_realloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H, void * P, size_t newsize)
{
  return ccmem_realloc_guarded(L, P_H, ccmem_standard_allocator, P, newsize);
}

#define ccmem_std_realloc_guarded(L,P_H,P,newsize) \
  _Generic((P_H),	\
	   ccmem_clean_handler_t	*: ccmem_std_realloc_guarded_clean, \
	   ccmem_error_handler_t	*: ccmem_std_realloc_guarded_error)(L,P_H,P,newsize)




/** --------------------------------------------------------------------
 ** Type definitions for memory blocks, ASCII strings, ASCIIZ strings.
 ** ----------------------------------------------------------------- */

typedef struct ccmem_block_t		ccmem_block_t;
typedef struct ccmem_ascii_t		ccmem_ascii_t;
typedef struct ccmem_asciiz_t		ccmem_asciiz_t;

struct ccmem_block_t {
  size_t	len;
  uint8_t *	ptr;
};

struct ccmem_ascii_t {
  size_t	len;
  char *	ptr;
};

struct ccmem_asciiz_t {
  size_t	len;
  char *	ptr;
};


/** --------------------------------------------------------------------
 ** Memory blocks.
 ** ----------------------------------------------------------------- */

__attribute__((__always_inline__,__nonnull__(1)))
static inline ccmem_block_t
ccmem_new_block (uint8_t * ptr, size_t len)
{
  ccmem_block_t		B = {
    .ptr = ptr,
    .len = len
  };
  return B;
}

__attribute__((__always_inline__))
static inline ccmem_block_t
ccmem_new_block_from_ascii (ccmem_ascii_t S)
{
  return ccmem_new_block((uint8_t *)S.ptr, S.len);
}

__attribute__((__always_inline__))
static inline ccmem_block_t
ccmem_new_block_from_asciiz (ccmem_asciiz_t S)
{
  return ccmem_new_block((uint8_t *)S.ptr, 1 + S.len);
}

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

__attribute__((__always_inline__,__nonnull__(1)))
static inline void
ccmem_block_delete (ccmem_allocator_t const * const A, ccmem_block_t B)
{
  ccmem_free(A, B.ptr);
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


/** ------------------------------------------------------------
 ** Memory handling: ASCII-coded, not zero-terminated strings.
 ** ----------------------------------------------------------*/

/* constructors */

__attribute__((__always_inline__,__nonnull__(1)))
static inline ccmem_ascii_t
ccmem_new_ascii (char * ptr, size_t len)
{
  ccmem_ascii_t		S = {
    .ptr = ptr,
    .len = len
  };
  return S;
}

__attribute__((__always_inline__))
static inline ccmem_ascii_t
ccmem_new_ascii_empty (void)
{
  ccmem_ascii_t		S = {
    .ptr = "",
    .len = 0
  };
  return S;
}

__attribute__((__always_inline__))
static inline ccmem_ascii_t
ccmem_new_ascii_null (void)
{
  ccmem_ascii_t		S = {
    .ptr = NULL,
    .len = 0
  };
  return S;
}

__attribute__((__always_inline__,__nonnull__(1)))
static inline ccmem_ascii_t
ccmem_new_ascii_from_str (char * str)
{
  return ccmem_new_ascii(str, strlen(str));
}

__attribute__((__always_inline__))
static inline ccmem_ascii_t
ccmem_new_ascii_from_block (ccmem_block_t B)
{
  return ccmem_new_ascii((char *)B.ptr, B.len);
}

__attribute__((__always_inline__))
static inline ccmem_ascii_t
ccmem_new_ascii_from_asciiz (ccmem_asciiz_t S)
{
  return ccmem_new_ascii((char *)S.ptr, S.len);
}

/* ------------------------------------------------------------------ */
/* predicates */

__attribute__((__always_inline__))
static inline bool
ccmem_ascii_is_empty (ccmem_ascii_t const S)
{
  return ((0 == S.len) && (NULL != S.ptr));
}

__attribute__((__always_inline__))
static inline bool
ccmem_ascii_is_null (ccmem_ascii_t const S)
{
  return ((0 == S.len) && (NULL == S.ptr))? true : false;
}

/* ------------------------------------------------------------------ */
/* comparison */

__attribute__((__always_inline__))
static inline bool
ccmem_ascii_equal (ccmem_ascii_t const S1, ccmem_ascii_t const S2)
{
  return ((S1.len == S2.len) && (0 == strncmp(S1.ptr, S2.ptr, S1.len)))? true : false;
}

/* ------------------------------------------------------------------ */
/* operations */

__attribute__((__always_inline__))
static inline void
ccmem_ascii_clean_memory (ccmem_ascii_t S)
{
  memset(S.ptr, '\0', S.len);
}

/* ------------------------------------------------------------ */
/* memory allocation */

__attribute__((__always_inline__,__nonnull__(1,2)))
static inline ccmem_ascii_t
ccmem_ascii_malloc (cce_destination_t L, ccmem_allocator_t const * const A, size_t const len)
{
  return ccmem_new_ascii(ccmem_malloc(L, A, len), len);
}

__attribute__((__always_inline__,__nonnull__(1,2)))
static inline ccmem_ascii_t
ccmem_ascii_realloc (cce_destination_t L, ccmem_allocator_t const * const A, ccmem_ascii_t S, size_t newlen)
{
  return ccmem_new_ascii(ccmem_realloc(L, A, S.ptr, newlen), newlen);
}

__attribute__((__always_inline__,__nonnull__(1)))
static inline void
ccmem_ascii_free (ccmem_allocator_t const * const A, ccmem_ascii_t S)
{
  ccmem_free(A, S.ptr);
}

/* ------------------------------------------------------------ */
/* guarded memory allocation */

__attribute__((__always_inline__,__nonnull__(1,2,3)))
static inline ccmem_ascii_t
ccmem_ascii_malloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * S_H,
				  ccmem_allocator_t const * const A, size_t len)
{
  return ccmem_new_ascii(ccmem_malloc_guarded(L, S_H, A, len), len);
}

__attribute__((__always_inline__,__nonnull__(1,2,3)))
static inline ccmem_ascii_t
ccmem_ascii_malloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * S_H,
				  ccmem_allocator_t const * const A, size_t len)
{
  return ccmem_new_ascii(ccmem_malloc_guarded(L, S_H, A, len), len);
}

#define ccmem_ascii_malloc_guarded(L, S_H, A, len) \
  _Generic((S_H), \
	   ccmem_clean_handler_t *:	ccmem_ascii_malloc_guarded_clean, \
	   ccmem_error_handler_t *:	ccmem_ascii_malloc_guarded_error)(L, S_H, A, len)

__attribute__((__always_inline__,__nonnull__(1,2,3)))
static inline ccmem_ascii_t
ccmem_ascii_realloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * S_H,
				   ccmem_allocator_t const * const A, ccmem_ascii_t S, size_t newlen)
{
  return ccmem_new_ascii(ccmem_realloc_guarded(L, S_H, A, S.ptr, newlen), newlen);
}

__attribute__((__always_inline__,__nonnull__(1,2,3)))
static inline ccmem_ascii_t
ccmem_ascii_realloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * S_H,
				   ccmem_allocator_t const * const A, ccmem_ascii_t S, size_t newlen)
{
  return ccmem_new_ascii(ccmem_realloc_guarded(L, S_H, A, S.ptr, newlen), newlen);
}

#define ccmem_ascii_realloc_guarded(L, S_H, A, S, newlen) \
  _Generic((S_H), \
	   ccmem_clean_handler_t *:	ccmem_ascii_realloc_guarded_clean, \
	   ccmem_error_handler_t *:	ccmem_ascii_realloc_guarded_error)(L, S_H, A, S, newlen)


/** ------------------------------------------------------------
 ** Memory handling: ASCIIZ-coded, zero-terminated strings.
 ** ----------------------------------------------------------*/

/* constructors */

__attribute__((__always_inline__,__nonnull__(1)))
static inline ccmem_asciiz_t
ccmem_new_asciiz (char * ptr, size_t len)
{
  ccmem_asciiz_t	S = {
    .ptr = ptr,
    .len = len
  };
  return S;
}

__attribute__((__always_inline__))
static inline ccmem_asciiz_t
ccmem_new_asciiz_empty (void)
{
  ccmem_asciiz_t	S = {
    .ptr = "",
    .len = 0
  };
  return S;
}

__attribute__((__always_inline__))
static inline ccmem_asciiz_t
ccmem_new_asciiz_null (void)
{
  ccmem_asciiz_t	S = {
    .ptr = NULL,
    .len = 0
  };
  return S;
}

__attribute__((__always_inline__,__nonnull__(1)))
static inline ccmem_asciiz_t
ccmem_new_asciiz_from_str (char * str)
{
  return ccmem_new_asciiz(str, strlen(str));
}

__attribute__((__always_inline__))
static inline ccmem_asciiz_t
ccmem_new_asciiz_from_block (ccmem_block_t B)
/* We assume  that the string  in the  block is zero-terminated  and the
   terminating zero is an octet counted by the block's length. */
{
  return ccmem_new_asciiz((char *)B.ptr, B.len - 1);
}

/* ------------------------------------------------------------------ */
/* predicates */

__attribute__((__always_inline__))
static inline bool
ccmem_asciiz_is_empty (ccmem_asciiz_t const S)
{
  return ((0 == S.len) && (NULL != S.ptr) && ('\0' == S.ptr[S.len]));
}

__attribute__((__always_inline__))
static inline bool
ccmem_asciiz_is_null (ccmem_asciiz_t const S)
{
  return ((0 == S.len) && (NULL == S.ptr))? true : false;
}

__attribute__((__always_inline__))
static inline bool
ccmem_asciiz_is_terminated (ccmem_asciiz_t S)
{
  return ((NULL != S.ptr) && ('\0' == S.ptr[S.len]))? true : false;
}

/* ------------------------------------------------------------------ */
/* comparison */

__attribute__((__always_inline__))
static inline bool
ccmem_asciiz_equal (ccmem_asciiz_t const S1, ccmem_asciiz_t const S2)
{
  return ((S1.len == S2.len) && (0 == strncmp(S1.ptr, S2.ptr, S1.len)))? true : false;
}

/* ------------------------------------------------------------------ */
/* operations */

__attribute__((__always_inline__))
static inline void
ccmem_asciiz_clean_memory (ccmem_asciiz_t S)
{
  memset(S.ptr, '\0', 1+S.len);
}

__attribute__((__always_inline__))
static inline void
ccmem_asciiz_terminate (ccmem_asciiz_t S)
{
  S.ptr[S.len] = '\0';
}

/* ------------------------------------------------------------------ */
/* dynamic memory allocation */

__attribute__((__always_inline__,__nonnull__(1,2)))
static inline ccmem_asciiz_t
ccmem_asciiz_malloc (cce_destination_t L, ccmem_allocator_t const * const A, size_t const len)
{
  ccmem_asciiz_t	S = ccmem_new_asciiz(ccmem_malloc(L, A, len+1), len);
  ccmem_asciiz_terminate(S);
  return S;
}

__attribute__((__always_inline__,__nonnull__(1,2)))
static inline ccmem_asciiz_t
ccmem_asciiz_realloc (cce_destination_t L, ccmem_allocator_t const * const A, ccmem_asciiz_t S, size_t newlen)
{
  ccmem_asciiz_t	R = ccmem_new_asciiz(ccmem_realloc(L, A, S.ptr, newlen+1), newlen);
  ccmem_asciiz_terminate(R);
  return R;
}

__attribute__((__always_inline__,__nonnull__(1)))
static inline void
ccmem_asciiz_free (ccmem_allocator_t const * const A, ccmem_asciiz_t S)
{
  ccmem_free(A, S.ptr);
}

/* ------------------------------------------------------------ */
/* guarded memory allocation */

__attribute__((__always_inline__,__nonnull__(1,2,3)))
static inline ccmem_asciiz_t
ccmem_asciiz_malloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * S_H,
				   ccmem_allocator_t const * const A, size_t len)
{
  ccmem_asciiz_t	S = ccmem_new_asciiz(ccmem_malloc_guarded(L, S_H, A, 1+len), len);
  ccmem_asciiz_terminate(S);
  return S;
}

__attribute__((__always_inline__,__nonnull__(1,2,3)))
static inline ccmem_asciiz_t
ccmem_asciiz_malloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * S_H,
				   ccmem_allocator_t const * const A, size_t len)
{
  ccmem_asciiz_t	S = ccmem_new_asciiz(ccmem_malloc_guarded(L, S_H, A, 1+len), len);
  ccmem_asciiz_terminate(S);
  return S;
}

#define ccmem_asciiz_malloc_guarded(L, S_H, A, len)			\
  _Generic((S_H),							\
	   ccmem_clean_handler_t *:	ccmem_asciiz_malloc_guarded_clean, \
	   ccmem_error_handler_t *:	ccmem_asciiz_malloc_guarded_error)(L, S_H, A, len)

__attribute__((__always_inline__,__nonnull__(1,2,3)))
static inline ccmem_asciiz_t
ccmem_asciiz_realloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * S_H,
				    ccmem_allocator_t const * const A, ccmem_asciiz_t S, size_t newlen)
{
  ccmem_asciiz_t	R = ccmem_new_asciiz(ccmem_realloc_guarded(L, S_H, A, S.ptr, 1+newlen), newlen);
  ccmem_asciiz_terminate(R);
  return R;
}

__attribute__((__always_inline__,__nonnull__(1,2,3)))
static inline ccmem_asciiz_t
ccmem_asciiz_realloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * S_H,
				    ccmem_allocator_t const * const A, ccmem_asciiz_t S, size_t newlen)
{
  ccmem_asciiz_t	R = ccmem_new_asciiz(ccmem_realloc_guarded(L, S_H, A, S.ptr, 1+newlen), newlen);
  ccmem_asciiz_terminate(R);
  return R;
}

#define ccmem_asciiz_realloc_guarded(L, S_H, A, S, newlen)		\
  _Generic((S_H),							\
	   ccmem_clean_handler_t *:	ccmem_asciiz_realloc_guarded_clean, \
	   ccmem_error_handler_t *:	ccmem_asciiz_realloc_guarded_error)(L, S_H, A, S, newlen)


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCMEMORY_H */

/* end of file */
