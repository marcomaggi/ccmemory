/*
  Part of: CCMemory
  Contents: exception handlers and guarded allocators
  Date: Sep 25, 2018

  Abstract



  Copyright (C) 2018, 2019 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you can redistribute  it and/or modify it under the terms of
  the GNU Lesser General Public License as published by the Free Software Foundation;
  either version 3.0 of the License, or (at your option) any later version.

  This library  is distributed in the  hope that it  will be useful, but  WITHOUT ANY
  WARRANTY; without  even the implied  warranty of  MERCHANTABILITY or FITNESS  FOR A
  PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License along with
  this library; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
  Suite 330, Boston, MA 02111-1307 USA.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ccmemory-internals.h"


/** --------------------------------------------------------------------
 ** Handlers.
 ** ----------------------------------------------------------------- */

__attribute__((__nonnull__(1,2)))
static void
ccmem_clean_handler (cce_condition_t const * C CCMEM_UNUSED, cce_clean_handler_t const * const H)
{
  CCMEM_PC(ccmem_clean_handler_t const, P_H, H);

  ccmem_free(ccmem_handler_allocator(P_H), cce_handler_resource_pointer(H));
}

__attribute__((__nonnull__(1,2)))
static void
ccmem_error_handler (cce_condition_t const * C CCMEM_UNUSED, cce_error_handler_t const * const H)
{
  CCMEM_PC(ccmem_error_handler_t const, P_H, H);

  ccmem_free(ccmem_handler_allocator(P_H), cce_handler_resource_pointer(H));
}


/** --------------------------------------------------------------------
 ** Generic guarded malloc.
 ** ----------------------------------------------------------------- */

void *
ccmem_malloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
			    ccmem_allocator_t const * A, size_t size)
{
  void	* P = ccmem_malloc(L, A, size);

  cce_init_and_register_handler(L, ccmem_handler_handler(P_H), ccmem_clean_handler, P);
  P_H->allocator = A;
  return P;
}

void *
ccmem_malloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
			    ccmem_allocator_t const * A, size_t size)
{
  void	* P = ccmem_malloc(L, A, size);

  cce_init_and_register_handler(L, ccmem_handler_handler(P_H), ccmem_error_handler, P);
  P_H->allocator = A;
  return P;
}


/** --------------------------------------------------------------------
 ** Generic guarded calloc.
 ** ----------------------------------------------------------------- */

void *
ccmem_calloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
			    ccmem_allocator_t const * A, size_t count, size_t eltsize)
{
  void	* P = ccmem_calloc(L, A, count, eltsize);

  cce_init_and_register_handler(L, ccmem_handler_handler(P_H), ccmem_clean_handler, P);
  P_H->allocator = A;
  return P;
}

void *
ccmem_calloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
			    ccmem_allocator_t const * A, size_t count, size_t eltsize)
{
  void	* P = ccmem_calloc(L, A, count, eltsize);

  cce_init_and_register_handler(L, ccmem_handler_handler(P_H), ccmem_error_handler, P);
  P_H->allocator = A;
  return P;
}


/** --------------------------------------------------------------------
 ** Generic guarded realloc.
 ** ----------------------------------------------------------------- */

void *
ccmem_realloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
			     ccmem_allocator_t const * A, void * P, size_t newsize)
{
  void			* Q = ccmem_realloc(L, A, P, newsize);
  cce_clean_handler_t	* H = ccmem_handler_handler(P_H);

  H->handler.resource_pointer	= Q;
  return Q;
}

void *
ccmem_realloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
			     ccmem_allocator_t const * A, void * P, size_t newsize)
{
  void			* Q = ccmem_realloc(L, A, P, newsize);
  cce_error_handler_t	* H = ccmem_handler_handler(P_H);

  H->handler.resource_pointer	= Q;
  return Q;
}

/* end of file */
