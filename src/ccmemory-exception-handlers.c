/*
  Part of: CCMemory
  Contents: exception handlers and guarded allocators
  Date: Sep 25, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  This is free software; you  can redistribute it and/or modify it under
  the terms of the GNU Lesser General Public License as published by the
  Free Software  Foundation; either version  3.0 of the License,  or (at
  your option) any later version.

  This library  is distributed in the  hope that it will  be useful, but
  WITHOUT   ANY  WARRANTY;   without  even   the  implied   warranty  of
  MERCHANTABILITY  or FITNESS  FOR A  PARTICULAR PURPOSE.   See  the GNU
  Lesser General Public License for more details.

  You  should have  received a  copy of  the GNU  Lesser  General Public
  License along  with this library; if  not, write to  the Free Software
  Foundation, Inc.,  59 Temple Place,  Suite 330, Boston,  MA 02111-1307
  USA.
*/


/** --------------------------------------------------------------------
 ** Headers.
 ** ----------------------------------------------------------------- */

#include "ccmemory-internals.h"


/** --------------------------------------------------------------------
 ** Handlers.
 ** ----------------------------------------------------------------- */

static void
ccmem_clean_handler (cce_condition_t const * C CCMEM_UNUSED, cce_handler_t * H)
{
  CCMEM_PC(ccmem_clean_handler_t, P_H, H);

  ccmem_free(P_H->allocator, P_H->handler.handler.pointer);
}

static void
ccmem_error_handler (cce_condition_t const * C CCMEM_UNUSED, cce_handler_t * H)
{
  CCMEM_PC(ccmem_error_handler_t, P_H, H);

  ccmem_free(P_H->allocator, P_H->handler.handler.pointer);
}


/** --------------------------------------------------------------------
 ** Generic guarded malloc.
 ** ----------------------------------------------------------------- */

void *
ccmem_malloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
			    ccmem_allocator_t const * A, size_t size)
{
  void *	P = ccmem_malloc(L, A, size);

  P_H->handler.handler.function	= ccmem_clean_handler;
  P_H->handler.handler.pointer	= P;
  P_H->allocator		= A;
  cce_register_clean_handler(L, &(P_H->handler));
  return P;
}

void *
ccmem_malloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
			    ccmem_allocator_t const * A, size_t size)
{
  void *	P = ccmem_malloc(L, A, size);

  P_H->handler.handler.function	= ccmem_error_handler;
  P_H->handler.handler.pointer	= P;
  P_H->allocator		= A;
  cce_register_error_handler(L, &(P_H->handler));
  return P;
}


/** --------------------------------------------------------------------
 ** Generic guarded calloc.
 ** ----------------------------------------------------------------- */

void *
ccmem_calloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
			    ccmem_allocator_t const * A, size_t count, size_t eltsize)
{
  void *	P = ccmem_calloc(L, A, count, eltsize);

  P_H->handler.handler.function	= ccmem_clean_handler;
  P_H->handler.handler.pointer	= P;
  P_H->allocator		= A;
  cce_register_clean_handler(L, &(P_H->handler));
  return P;
}

void *
ccmem_calloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
			    ccmem_allocator_t const * A, size_t count, size_t eltsize)
{
  void *	P = ccmem_calloc(L, A, count, eltsize);

  P_H->handler.handler.function	= ccmem_error_handler;
  P_H->handler.handler.pointer	= P;
  P_H->allocator		= A;
  cce_register_error_handler(L, &(P_H->handler));
  return P;
}


/** --------------------------------------------------------------------
 ** Generic guarded realloc.
 ** ----------------------------------------------------------------- */

void *
ccmem_realloc_guarded_clean (cce_destination_t L, ccmem_clean_handler_t * P_H,
			     ccmem_allocator_t const * A, void * P, size_t newsize)
{
  void *	Q = ccmem_realloc(L, A, P, newsize);

  P_H->handler.handler.pointer	= Q;
  return Q;
}

void *
ccmem_realloc_guarded_error (cce_destination_t L, ccmem_error_handler_t * P_H,
			     ccmem_allocator_t const * A, void * P, size_t newsize)
{
  void *	Q = ccmem_realloc(L, A, P, newsize);

  P_H->handler.handler.pointer	= Q;
  return Q;
}

/* end of file */
