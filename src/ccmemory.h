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
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


/** --------------------------------------------------------------------
 ** Version functions.
 ** ----------------------------------------------------------------- */

ccmem_decl char const *	ccmem_version_string		(void);
ccmem_decl int		ccmem_version_interface_current	(void);
ccmem_decl int		ccmem_version_interface_revision(void);
ccmem_decl int		ccmem_version_interface_age	(void);


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
 ** Memory handling: allocator.
 ** ----------------------------------------------------------------- */

typedef struct ccmem_memory_allocator_t		ccmem_memory_allocator_t;
typedef struct ccmem_memory_allocator_methods_t	ccmem_memory_allocator_methods_t;

typedef void * ccmem_malloc_fun_t  (ccmem_memory_allocator_t A, cce_destination_t L, size_t size);
typedef void * ccmem_realloc_fun_t (ccmem_memory_allocator_t A, cce_destination_t L, void * ptr, size_t newsize);
typedef void * ccmem_calloc_fun_t  (ccmem_memory_allocator_t A, cce_destination_t L, size_t count, size_t eltsize);
typedef void   ccmem_free_fun_t    (ccmem_memory_allocator_t A, cce_destination_t L, void * ptr);

struct ccmem_memory_allocator_methods_t {
  ccmem_malloc_fun_t  * const	malloc;
  ccmem_realloc_fun_t * const	realloc;
  ccmem_calloc_fun_t  * const	calloc;
  ccmem_free_fun_t    * const	free;
};

struct ccmem_memory_allocator_t {
  ccmem_memory_allocator_methods_t const * const	methods;
};

/* ------------------------------------------------------------------ */

static inline void *
ccmem_malloc (ccmem_memory_allocator_t A, cce_destination_t L, size_t size)
{
  return A.methods->malloc(A, L, size);
}

static inline void *
ccmem_realloc (ccmem_memory_allocator_t A, cce_destination_t L, void * ptr, size_t newsize)
{
  return A.methods->realloc(A, L, ptr, newsize);
}

static inline void *
ccmem_calloc (ccmem_memory_allocator_t A, cce_destination_t L, size_t count, size_t eltsize)
{
  return A.methods->calloc(A, L, count, eltsize);
}

static inline void
ccmem_free (ccmem_memory_allocator_t A, cce_destination_t L, void * ptr)
{
  A.methods->free(A, L, ptr);
}


/** --------------------------------------------------------------------
 ** Memory handling: blocks.
 ** ----------------------------------------------------------------- */

typedef struct ccmem_block_t {
  size_t	len;
  uint8_t *	ptr;
} ccmem_block_t;

#if 0

static inline __attribute__((__always_inline__,__nonnull__))
void
ccmem_block_set (ccmem_block_t * block, void * p, size_t len)
{
  block->ptr = p;
  block->len = len;
}
static inline __attribute__((__always_inline__,__nonnull__))
void
ccmem_block_reset (ccmem_block_t * block)
{
  block->ptr = NULL;
  block->len = 0;
}
static inline __attribute__((__always_inline__))
ccmem_bool_t
ccmem_block_is_null (ccmem_block_t block)
{
  return (NULL == block.ptr);
}
static inline __attribute__((__always_inline__))
void
ccmem_block_clean_memory (ccmem_block_t block)
{
  memset(block.ptr, '\0', block.len);
}
static inline __attribute__((__always_inline__))
ccmem_block_t
ccmem_block_alloc (ccmem_memory_allocator_t allocator, size_t dim)
{
  ccmem_block_t	block = { .ptr = NULL, .len = dim };
  allocator.alloc(allocator.data, &(block.ptr), dim);
  return block;
}
static inline __attribute__((__always_inline__))
ccmem_block_t
ccmem_block_realloc (ccmem_memory_allocator_t allocator, ccmem_block_t block, size_t new_dim)
{
  allocator.alloc(allocator.data, &(block.ptr), new_dim);
  block.len = new_dim;
  return block;
}
static inline __attribute__((__always_inline__))
void
ccmem_block_free (ccmem_memory_allocator_t allocator, ccmem_block_t block)
{
  if (block.ptr)
    allocator.alloc(allocator.data, &(block.ptr), 0);
}
static inline void
ccmem_block_shift_x (ccmem_block_t * block, ssize_t offset, size_t dim)
{
  if (dim) {
    if (1 != dim) {
      block->ptr += offset;
      block->len -= offset;
    } else {
      block->ptr += offset * dim;
      block->len -= offset * dim;
    }
  }
}
static inline ccmem_block_t
ccmem_block_shift (ccmem_block_t A, ssize_t offset, size_t dim)
{
  ccmem_block_t	B;
  if (dim) {
    if (1 != dim) {
      B.ptr = A.ptr + offset;
      B.len = A.len + offset;
    } else {
      B.ptr = A.ptr + offset * dim;
      B.len = A.len + offset * dim;
    }
  } else {
    B = A;
  }
  return B;
}
static inline __attribute__((__always_inline__))
ccmem_block_t
ccmem_block_difference (ccmem_block_t A, ccmem_block_t B)
{
  ccmem_block_t	C = { .ptr = A.ptr, .len = B.ptr - A.ptr };
  return C;
}

#endif


/** --------------------------------------------------------------------
 ** Done.
 ** ----------------------------------------------------------------- */

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* CCMEMORY_H */

/* end of file */
