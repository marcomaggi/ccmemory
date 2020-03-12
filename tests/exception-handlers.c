/*
  Part of: CCMemory
  Contents: tests for the exception handlers
  Date: Sep 25, 2018

  Abstract



  Copyright (C) 2018 Marco Maggi <mrc.mgg@gmail.com>

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

#include "ccmemory.h"
#include <cctests.h>


/** --------------------------------------------------------------------
 ** Generic memory allocator.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t upper_L)
{
  cce_location_t        L[1];
  ccmem_clean_handler_t P_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    void *      P = ccmem_malloc_guarded(L, P_H, ccmem_standard_allocator, 4096);

    memset(P, 0, 4096);
    cce_run_body_handlers(L);
  }
}

void
test_1_2 (cce_destination_t upper_L)
{
  cce_location_t        L[1];
  ccmem_clean_handler_t P_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    void *      P = ccmem_calloc_guarded(L, P_H, ccmem_standard_allocator, 16, 4096);

    memset(P, 0, 16 * 4096);
    cce_run_body_handlers(L);
  }
}

void
test_1_3 (cce_destination_t upper_L)
{
  cce_location_t        L[1];
  ccmem_clean_handler_t P_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    void *      P = ccmem_malloc_guarded(L, P_H, ccmem_standard_allocator, 4096);

    memset(P, 0, 4096);
    {
      void *    Q = ccmem_realloc_guarded(L, P_H,
					  ccmem_standard_allocator, P, 2 * 4096);

      memset(Q, 0, 2 * 4096);
    }
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Standard memory allocator.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t upper_L)
{
  cce_location_t        L[1];
  ccmem_clean_handler_t P_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    void *      P = ccmem_std_malloc_guarded(L, P_H, 4096);

    memset(P, 0, 4096);
    cce_run_body_handlers(L);
  }
}

void
test_2_2 (cce_destination_t upper_L)
{
  cce_location_t        L[1];
  ccmem_clean_handler_t P_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    void *      P = ccmem_std_calloc_guarded(L, P_H, 16, 4096);

    memset(P, 0, 16 * 4096);
    cce_run_body_handlers(L);
  }
}

void
test_2_3 (cce_destination_t upper_L)
{
  cce_location_t        L[1];
  ccmem_clean_handler_t P_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    void *      P = ccmem_std_malloc_guarded(L, P_H, 4096);

    memset(P, 0, 4096);
    {
      void *    Q = ccmem_std_realloc_guarded(L, P_H, P, 2 * 4096);

      memset(Q, 0, 2 * 4096);
    }
    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  cctests_init("tests for the exception handlers");
  {
    cctests_begin_group("generic memory allocator");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
      cctests_run(test_1_3);
    }
    cctests_end_group();

    cctests_begin_group("standard memory allocator");
    {
      cctests_run(test_2_1);
      cctests_run(test_2_2);
      cctests_run(test_2_3);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
