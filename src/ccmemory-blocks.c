/*
  Part of: CCMemory
  Contents: functions for memory blocks
  Date: May  8, 2018

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
 ** Exception handlers.
 ** ----------------------------------------------------------------- */

static void
ccmem_block_delete_handler (cce_condition_t const * C CCMEM_UNUSED, cce_handler_t * _H)
{
  CCMEM_PC(ccmem_block_clean_handler_t, H, _H);
  cce_location_t	L[1];

  if (cce_location(L)) {
    /* The exception is discarded here!!!! */
    cce_run_error_handlers_final(L);
  } else {
    if (0) { fprintf(stderr, "%s: releasing block %p\n", __func__, (void *)H->B.ptr); }
    ccmem_block_delete(L, H->A, H->B);
    cce_run_clean_handlers(L);
  }
}

void
ccmem_block_register_clean_handler (cce_destination_t L, ccmem_block_clean_handler_t * H,
				    ccmem_allocator_t const * const A, ccmem_block_t B)
{
  H->handler.handler.function	= ccmem_block_delete_handler;
  H->A	= A;
  H->B	= B;
  cce_register_clean_handler(L, &(H->handler.handler));
}

void
ccmem_block_register_error_handler (cce_destination_t L, ccmem_block_error_handler_t * H,
				    ccmem_allocator_t const * const A, ccmem_block_t B)
{
  H->handler.handler.function	= ccmem_block_delete_handler;
  H->A	= A;
  H->B	= B;
  cce_register_error_handler(L, &(H->handler.handler));
}


/** --------------------------------------------------------------------
 ** Guarded allocation.
 ** ----------------------------------------------------------------- */

ccmem_block_t
ccmem_block_new_guarded_clean (cce_destination_t L, ccmem_block_clean_handler_t * H, ccmem_allocator_t const * const A, size_t const len)
{
  ccmem_block_t	B = ccmem_block_new(L, A, len);
  ccmem_block_register_clean_handler(L, H, A, B);
  return B;
}

ccmem_block_t
ccmem_block_new_guarded_error (cce_destination_t L, ccmem_block_error_handler_t * H, ccmem_allocator_t const * const A, size_t const len)
{
  ccmem_block_t B = ccmem_block_new(L, A, len);
  ccmem_block_register_error_handler(L, H, A, B);
  return B;
}


/** --------------------------------------------------------------------
 ** Operations.
 ** ----------------------------------------------------------------- */

ccmem_block_t
ccmem_block_shift (ccmem_block_t const A, ssize_t const offset, size_t const dim)
{
  if ((0 == dim) || (0 == offset)) {
    return A;
  } else if (1 == dim) {
    ccmem_block_t	B = {
      .ptr = A.ptr + offset,
      .len = A.len + offset
    };
    return B;
  } else if (1 == offset) {
    ccmem_block_t	B = {
      .ptr = A.ptr + dim,
      .len = A.len + dim
    };
    return B;
  } else {
    ccmem_block_t	B = {
      .ptr = A.ptr + offset * dim,
      .len = A.len + offset * dim
    };
    return B;
  }
}

/* end of file */
