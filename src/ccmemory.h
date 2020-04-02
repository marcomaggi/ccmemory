/*
  Part of: CCMemory
  Contents: public header file
  Date: Dec 17, 2016

  Abstract

	This  public header  must  be included  in  all the  source  files using  the
	features of the library CCMemory.

  Copyright (C) 2016, 2018-2020 Marco Maggi <mrc.mgg@gmail.com>

  This program is free  software: you can redistribute it and/or  modify it under the
  terms of the  GNU Lesser General Public  License as published by  the Free Software
  Foundation, either version 3 of the License, or (at your option) any later version.

  This program  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CCMEMORY_H
#define CCMEMORY_H 1

#ifdef __cplusplus
extern "C" {
#endif


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include <ccexceptions.h>
#include <stdbool.h>
#include <stdint.h> /* for "uint8_t" */
#include <unistd.h> /* for "ssize_t" */
#include <string.h>


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

cclib_decl char const *	ccmem_version_string			(void);
cclib_decl int		ccmem_version_interface_current		(void);
cclib_decl int		ccmem_version_interface_revision	(void);
cclib_decl int		ccmem_version_interface_age		(void);


/** --------------------------------------------------------------------
 ** Initialisation.
 ** ----------------------------------------------------------------- */

/*
cclib_decl void ccmem_library_init (void)
  CCLIB_FUNC_ATTRIBUTE_CONSTRUCTOR
*/


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

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline void *
ccmem_malloc (cce_destination_t L, ccmem_allocator_t const * A, size_t size)
{
  return A->methods->malloc(L, A, size);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline void *
ccmem_realloc (cce_destination_t L, ccmem_allocator_t const * A, void * ptr, size_t newsize)
{
  return A->methods->realloc(L, A, ptr, newsize);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline void *
ccmem_calloc (cce_destination_t L, ccmem_allocator_t const * A, size_t count, size_t eltsize)
{
  return A->methods->calloc(L, A, count, eltsize);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
static inline void
ccmem_free (ccmem_allocator_t const * A, void * ptr)
{
  A->methods->free(A, ptr);
}


/** --------------------------------------------------------------------
 ** Standard memory allocator definition.
 ** ----------------------------------------------------------------- */

cclib_decl ccmem_allocator_t const * const ccmem_standard_allocator;

/* ------------------------------------------------------------------ */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline void *
ccmem_std_malloc (cce_destination_t L, size_t size)
{
  return ccmem_standard_allocator->methods->malloc(L, ccmem_standard_allocator, size);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline void *
ccmem_std_realloc (cce_destination_t L, void * ptr, size_t newsize)
{
  return ccmem_standard_allocator->methods->realloc(L, ccmem_standard_allocator, ptr, newsize);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline void *
ccmem_std_calloc (cce_destination_t L, size_t count, size_t eltsize)
{
  return ccmem_standard_allocator->methods->calloc(L, ccmem_standard_allocator, count, eltsize);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
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

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline cce_clean_handler_t *
ccmem_clean_handler_handler (ccmem_clean_handler_t * const H)
{
  return &(H->handler);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline cce_error_handler_t *
ccmem_error_handler_handler (ccmem_error_handler_t * const H)
{
  return &(H->handler);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline cce_clean_handler_t const *
ccmem_const_clean_handler_handler (ccmem_clean_handler_t const * const H)
{
  return &(H->handler);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline cce_error_handler_t const *
ccmem_const_error_handler_handler (ccmem_error_handler_t const * const H)
{
  return &(H->handler);
}

#define ccmem_handler_handler(H)					\
  _Generic((H),								\
	   ccmem_clean_handler_t *: ccmem_clean_handler_handler,	\
	   ccmem_error_handler_t *: ccmem_error_handler_handler,	\
	   ccmem_clean_handler_t const *: ccmem_const_clean_handler_handler, \
	   ccmem_error_handler_t const *: ccmem_const_error_handler_handler)(H)

/* ------------------------------------------------------------------ */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline cce_resource_data_t *
ccmem_clean_handler_resource_pointer (ccmem_clean_handler_t * H)
{
  return cce_handler_resource_pointer(ccmem_handler_handler(H));
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline cce_resource_data_t *
ccmem_error_handler_resource_pointer (ccmem_error_handler_t * H)
{
  return cce_handler_resource_pointer(ccmem_handler_handler(H));
}

#define ccmem_handler_resource_pointer(H)				\
  _Generic((H),								\
	   ccmem_clean_handler_t *: ccmem_clean_handler_resource_pointer, \
	   ccmem_error_handler_t *: ccmem_error_handler_resource_pointer, \
	   ccmem_clean_handler_t const *: ccmem_clean_handler_resource_pointer, \
	   ccmem_error_handler_t const *: ccmem_error_handler_resource_pointer)(H)

/* ------------------------------------------------------------------ */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline ccmem_allocator_t const *
ccmem_clean_handler_allocator (ccmem_clean_handler_t const * const H)
{
  return H->allocator;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
static inline ccmem_allocator_t const *
ccmem_error_handler_allocator (ccmem_error_handler_t const * const H)
{
  return H->allocator;
}

#define ccmem_handler_allocator(H)					\
  _Generic((H),								\
	   ccmem_clean_handler_t *: ccmem_clean_handler_allocator,	\
	   ccmem_error_handler_t *: ccmem_error_handler_allocator,	\
	   ccmem_clean_handler_t const *: ccmem_clean_handler_allocator, \
	   ccmem_error_handler_t const *: ccmem_error_handler_allocator)(H)

/* ------------------------------------------------------------------ */

cclib_decl void * ccmem_malloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
					      ccmem_allocator_t const * A, size_t size)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl void * ccmem_malloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
					      ccmem_allocator_t const * A, size_t size)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

#define ccmem_malloc_guarded(L,P_H,A,size)				\
  _Generic((P_H),							\
	   ccmem_clean_handler_t	*: ccmem_malloc_guarded_clean,	\
	   ccmem_error_handler_t	*: ccmem_malloc_guarded_error)(L,P_H,A,size)

/* ------------------------------------------------------------------ */

cclib_decl void * ccmem_calloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
					      ccmem_allocator_t const * A, size_t count, size_t eltsize)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl void * ccmem_calloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
					      ccmem_allocator_t const * A, size_t count, size_t eltsize)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

#define ccmem_calloc_guarded(L,P_H,A,count,eltsize)			\
  _Generic((P_H),							\
	   ccmem_clean_handler_t	*: ccmem_calloc_guarded_clean,	\
	   ccmem_error_handler_t	*: ccmem_calloc_guarded_error)(L,P_H,A,count,eltsize)

/* ------------------------------------------------------------------ */

cclib_decl void * ccmem_realloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
					       ccmem_allocator_t const * A, void * P, size_t newsize)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

cclib_decl void * ccmem_realloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
					       ccmem_allocator_t const * A, void * P, size_t newsize)
  CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3,4)
  CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL;

#define ccmem_realloc_guarded(L,P_H,A,P,newsize)			\
  _Generic((P_H),							\
	   ccmem_clean_handler_t	*: ccmem_realloc_guarded_clean, \
	   ccmem_error_handler_t	*: ccmem_realloc_guarded_error)(L,P_H,A,P,newsize)


/** --------------------------------------------------------------------
 ** Standard memory allocator: exception handlers.
 ** ----------------------------------------------------------------- */

CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void *
ccmem_std_malloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H, size_t size)
{
  return ccmem_malloc_guarded(L, P_H, ccmem_standard_allocator, size);
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void *
ccmem_std_malloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H, size_t size)
{
  return ccmem_malloc_guarded(L, P_H, ccmem_standard_allocator, size);
}

#define ccmem_std_malloc_guarded(L,P_H,size)				\
  _Generic((P_H),							\
	   ccmem_clean_handler_t *: ccmem_std_malloc_guarded_clean,	\
	   ccmem_error_handler_t *: ccmem_std_malloc_guarded_error)(L,P_H,size)

/* ------------------------------------------------------------------ */

CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void *
ccmem_std_calloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H, size_t count, size_t eltsize)
{
  return ccmem_calloc_guarded(L, P_H, ccmem_standard_allocator, count, eltsize);
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void *
ccmem_std_calloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H, size_t count, size_t eltsize)
{
  return ccmem_calloc_guarded(L, P_H, ccmem_standard_allocator, count, eltsize);
}

#define ccmem_std_calloc_guarded(L,P_H,count,eltsize)			\
  _Generic((P_H),							\
	   ccmem_clean_handler_t *: ccmem_std_calloc_guarded_clean,	\
	   ccmem_error_handler_t *: ccmem_std_calloc_guarded_error)(L,P_H,count,eltsize)

/* ------------------------------------------------------------------ */

CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void *
ccmem_std_realloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H, void * P, size_t newsize)
{
  return ccmem_realloc_guarded(L, P_H, ccmem_standard_allocator, P, newsize);
}

CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
CCLIB_FUNC_ATTRIBUTE_RETURNS_NONNULL
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void *
ccmem_std_realloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H, void * P, size_t newsize)
{
  return ccmem_realloc_guarded(L, P_H, ccmem_standard_allocator, P, newsize);
}

#define ccmem_std_realloc_guarded(L,P_H,P,newsize)			\
  _Generic((P_H),							\
	   ccmem_clean_handler_t *: ccmem_std_realloc_guarded_clean,	\
	   ccmem_error_handler_t *: ccmem_std_realloc_guarded_error)(L,P_H,P,newsize)


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

/* constructors */

CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_block_t
ccmem_new_block (uint8_t * ptr, size_t len)
{
  ccmem_block_t		S = {
    .ptr = ptr,
    .len = len
  };
  return S;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_block_t
ccmem_new_block_null (void)
{
  ccmem_block_t		S = {
    .ptr = NULL,
    .len = 0
  };
  return S;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_block_t
ccmem_new_block_from_ascii (ccmem_ascii_t S)
{
  return ccmem_new_block((uint8_t *)S.ptr, S.len);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_block_t
ccmem_new_block_from_asciiz (ccmem_asciiz_t S)
{
  return ccmem_new_block((uint8_t *)S.ptr, 1 + S.len);
}

/* ------------------------------------------------------------------ */
/* predicates */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccmem_block_is_empty (ccmem_block_t const S)
{
  return ((0 == S.len) && (NULL != S.ptr))? true : false;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccmem_block_is_null (ccmem_block_t const S)
{
  return ((0 == S.len) && (NULL == S.ptr))? true : false;
}

/* ------------------------------------------------------------------ */
/* comparison */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccmem_block_equal (ccmem_block_t const S1, ccmem_block_t const S2)
{
  return ((S1.len == S2.len) && (0 == memcmp(S1.ptr, S2.ptr, S1.len)))? true : false;
}

/* ------------------------------------------------------------------ */
/* operations */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void
ccmem_block_clean_memory (ccmem_block_t S)
{
  memset(S.ptr, '\0', S.len);
}

cclib_decl ccmem_block_t ccmem_block_shift (ccmem_block_t B, ssize_t offset, size_t dim)
  CCLIB_FUNC_ATTRIBUTE_CONST;

cclib_decl ccmem_block_t ccmem_block_difference (ccmem_block_t A, ccmem_block_t B)
  CCLIB_FUNC_ATTRIBUTE_CONST;

/* ------------------------------------------------------------ */
/* memory allocation */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
static inline ccmem_block_t
ccmem_block_malloc (cce_destination_t L, ccmem_allocator_t const * const A, size_t const len)
{
  return ccmem_new_block(ccmem_malloc(L, A, len), len);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
static inline ccmem_block_t
ccmem_block_realloc (cce_destination_t L, ccmem_allocator_t const * const A, ccmem_block_t S, size_t newlen)
{
  return ccmem_new_block(ccmem_realloc(L, A, S.ptr, newlen), newlen);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
static inline void
ccmem_block_free (ccmem_allocator_t const * const A, ccmem_block_t S)
{
  ccmem_free(A, S.ptr);
}

/* ------------------------------------------------------------ */
/* guarded memory allocation */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
static inline ccmem_block_t
ccmem_block_malloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * S_H,
				  ccmem_allocator_t const * const A, size_t len)
{
  return ccmem_new_block(ccmem_malloc_guarded(L, S_H, A, len), len);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
static inline ccmem_block_t
ccmem_block_malloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * S_H,
				  ccmem_allocator_t const * const A, size_t len)
{
  return ccmem_new_block(ccmem_malloc_guarded(L, S_H, A, len), len);
}

#define ccmem_block_malloc_guarded(L, S_H, A, len)			\
  _Generic((S_H),							\
	   ccmem_clean_handler_t *:	ccmem_block_malloc_guarded_clean, \
	   ccmem_error_handler_t *:	ccmem_block_malloc_guarded_error)(L, S_H, A, len)

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
static inline ccmem_block_t
ccmem_block_realloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * S_H,
				   ccmem_allocator_t const * const A, ccmem_block_t S, size_t newlen)
{
  return ccmem_new_block(ccmem_realloc_guarded(L, S_H, A, S.ptr, newlen), newlen);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
static inline ccmem_block_t
ccmem_block_realloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * S_H,
				   ccmem_allocator_t const * const A, ccmem_block_t S, size_t newlen)
{
  return ccmem_new_block(ccmem_realloc_guarded(L, S_H, A, S.ptr, newlen), newlen);
}

#define ccmem_block_realloc_guarded(L, S_H, A, S, newlen)		\
  _Generic((S_H),							\
	   ccmem_clean_handler_t *:	ccmem_block_realloc_guarded_clean, \
	   ccmem_error_handler_t *:	ccmem_block_realloc_guarded_error)(L, S_H, A, S, newlen)


/** ------------------------------------------------------------
 ** Memory handling: ASCII-coded, not zero-terminated strings.
 ** ----------------------------------------------------------*/

/* constructors */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
static inline ccmem_ascii_t
ccmem_new_ascii (char * ptr, size_t len)
{
  ccmem_ascii_t		S = {
    .ptr = ptr,
    .len = len
  };
  return S;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_ascii_t
ccmem_new_ascii_empty (void)
{
  ccmem_ascii_t		S = {
    .ptr = "",
    .len = 0
  };
  return S;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_ascii_t
ccmem_new_ascii_null (void)
{
  ccmem_ascii_t		S = {
    .ptr = NULL,
    .len = 0
  };
  return S;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
static inline ccmem_ascii_t
ccmem_new_ascii_from_str (char * str)
{
  return ccmem_new_ascii(str, strlen(str));
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_ascii_t
ccmem_new_ascii_from_block (ccmem_block_t B)
{
  return ccmem_new_ascii((char *)B.ptr, B.len);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_ascii_t
ccmem_new_ascii_from_asciiz (ccmem_asciiz_t S)
{
  return ccmem_new_ascii((char *)S.ptr, S.len);
}

/* ------------------------------------------------------------------ */
/* predicates */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccmem_ascii_is_empty (ccmem_ascii_t const S)
{
  return ((0 == S.len) && (NULL != S.ptr));
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccmem_ascii_is_null (ccmem_ascii_t const S)
{
  return ((0 == S.len) && (NULL == S.ptr))? true : false;
}

/* ------------------------------------------------------------------ */
/* comparison */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccmem_ascii_equal (ccmem_ascii_t const S1, ccmem_ascii_t const S2)
{
  return ((S1.len == S2.len) && (0 == strncmp(S1.ptr, S2.ptr, S1.len)))? true : false;
}

/* ------------------------------------------------------------------ */
/* operations */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void
ccmem_ascii_clean_memory (ccmem_ascii_t S)
{
  memset(S.ptr, '\0', S.len);
}

/* ------------------------------------------------------------ */
/* memory allocation */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
static inline ccmem_ascii_t
ccmem_ascii_malloc (cce_destination_t L, ccmem_allocator_t const * const A, size_t const len)
{
  return ccmem_new_ascii(ccmem_malloc(L, A, len), len);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2)
static inline ccmem_ascii_t
ccmem_ascii_realloc (cce_destination_t L, ccmem_allocator_t const * const A, ccmem_ascii_t S, size_t newlen)
{
  return ccmem_new_ascii(ccmem_realloc(L, A, S.ptr, newlen), newlen);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
static inline void
ccmem_ascii_free (ccmem_allocator_t const * const A, ccmem_ascii_t S)
{
  ccmem_free(A, S.ptr);
}

/* ------------------------------------------------------------ */
/* guarded memory allocation */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
static inline ccmem_ascii_t
ccmem_ascii_malloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * S_H,
				  ccmem_allocator_t const * const A, size_t len)
{
  return ccmem_new_ascii(ccmem_malloc_guarded(L, S_H, A, len), len);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
static inline ccmem_ascii_t
ccmem_ascii_malloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * S_H,
				  ccmem_allocator_t const * const A, size_t len)
{
  return ccmem_new_ascii(ccmem_malloc_guarded(L, S_H, A, len), len);
}

#define ccmem_ascii_malloc_guarded(L, S_H, A, len)			\
  _Generic((S_H),							\
	   ccmem_clean_handler_t *:	ccmem_ascii_malloc_guarded_clean, \
	   ccmem_error_handler_t *:	ccmem_ascii_malloc_guarded_error)(L, S_H, A, len)

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
static inline ccmem_ascii_t
ccmem_ascii_realloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * S_H,
				   ccmem_allocator_t const * const A, ccmem_ascii_t S, size_t newlen)
{
  return ccmem_new_ascii(ccmem_realloc_guarded(L, S_H, A, S.ptr, newlen), newlen);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
static inline ccmem_ascii_t
ccmem_ascii_realloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * S_H,
				   ccmem_allocator_t const * const A, ccmem_ascii_t S, size_t newlen)
{
  return ccmem_new_ascii(ccmem_realloc_guarded(L, S_H, A, S.ptr, newlen), newlen);
}

#define ccmem_ascii_realloc_guarded(L, S_H, A, S, newlen)		\
  _Generic((S_H),							\
	   ccmem_clean_handler_t *:	ccmem_ascii_realloc_guarded_clean, \
	   ccmem_error_handler_t *:	ccmem_ascii_realloc_guarded_error)(L, S_H, A, S, newlen)


/** ------------------------------------------------------------
 ** Memory handling: ASCIIZ-coded, zero-terminated strings.
 ** ----------------------------------------------------------*/

/* constructors */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
static inline ccmem_asciiz_t
ccmem_new_asciiz (char * ptr, size_t len)
{
  ccmem_asciiz_t	S = {
    .ptr = ptr,
    .len = len
  };
  return S;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_asciiz_t
ccmem_new_asciiz_empty (void)
{
  ccmem_asciiz_t	S = {
    .ptr = "",
    .len = 0
  };
  return S;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_asciiz_t
ccmem_new_asciiz_null (void)
{
  ccmem_asciiz_t	S = {
    .ptr = NULL,
    .len = 0
  };
  return S;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1)
static inline ccmem_asciiz_t
ccmem_new_asciiz_from_str (char * str)
{
  return ccmem_new_asciiz(str, strlen(str));
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_asciiz_t
ccmem_new_asciiz_from_block (ccmem_block_t B)
/* We assume  that the string  in the  block is zero-terminated  and the
   terminating zero is an octet counted by the block's length. */
{
  return ccmem_new_asciiz((char *)B.ptr, B.len - 1);
}

/* ------------------------------------------------------------------ */
/* predicates */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccmem_asciiz_is_empty (ccmem_asciiz_t const S)
{
  return ((0 == S.len) && (NULL != S.ptr) && ('\0' == S.ptr[S.len]));
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccmem_asciiz_is_null (ccmem_asciiz_t const S)
{
  return ((0 == S.len) && (NULL == S.ptr))? true : false;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccmem_asciiz_is_terminated (ccmem_asciiz_t S)
{
  return ((NULL != S.ptr) && ('\0' == S.ptr[S.len]))? true : false;
}

/* ------------------------------------------------------------------ */
/* comparison */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline bool
ccmem_asciiz_equal (ccmem_asciiz_t const S1, ccmem_asciiz_t const S2)
{
  return ((S1.len == S2.len) && (0 == strncmp(S1.ptr, S2.ptr, S1.len)))? true : false;
}

/* ------------------------------------------------------------------ */
/* operations */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void
ccmem_asciiz_clean_memory (ccmem_asciiz_t S)
{
  memset(S.ptr, '\0', 1+S.len);
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void
ccmem_asciiz_terminate (ccmem_asciiz_t S)
{
  S.ptr[S.len] = '\0';
}

/* ------------------------------------------------------------------ */
/* dynamic memory allocation */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_asciiz_t
ccmem_asciiz_malloc (cce_destination_t L, ccmem_allocator_t const * const A, size_t const len)
{
  ccmem_asciiz_t	S = ccmem_new_asciiz(ccmem_malloc(L, A, len+1), len);
  ccmem_asciiz_terminate(S);
  return S;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline ccmem_asciiz_t
ccmem_asciiz_realloc (cce_destination_t L, ccmem_allocator_t const * const A, ccmem_asciiz_t S, size_t newlen)
{
  ccmem_asciiz_t	R = ccmem_new_asciiz(ccmem_realloc(L, A, S.ptr, newlen+1), newlen);
  ccmem_asciiz_terminate(R);
  return R;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
static inline void
ccmem_asciiz_free (ccmem_allocator_t const * const A, ccmem_asciiz_t S)
{
  ccmem_free(A, S.ptr);
}

/* ------------------------------------------------------------ */
/* guarded memory allocation */

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
static inline ccmem_asciiz_t
ccmem_asciiz_malloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * S_H,
				   ccmem_allocator_t const * const A, size_t len)
{
  ccmem_asciiz_t	S = ccmem_new_asciiz(ccmem_malloc_guarded(L, S_H, A, 1+len), len);
  ccmem_asciiz_terminate(S);
  return S;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
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

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
static inline ccmem_asciiz_t
ccmem_asciiz_realloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * S_H,
				    ccmem_allocator_t const * const A, ccmem_asciiz_t S, size_t newlen)
{
  ccmem_asciiz_t	R = ccmem_new_asciiz(ccmem_realloc_guarded(L, S_H, A, S.ptr, 1+newlen), newlen);
  ccmem_asciiz_terminate(R);
  return R;
}

CCLIB_FUNC_ATTRIBUTE_ALWAYS_INLINE
CCLIB_FUNC_ATTRIBUTE_NONNULL(1,2,3)
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
