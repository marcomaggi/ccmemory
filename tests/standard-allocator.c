/*
  Part of: CCMemory
  Contents: tests for the standard allocator
  Date: Sun May  6, 2018

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
 ** Memory allocation and release: explicit allocator selector.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t L)
{
  void *	P = ccmem_malloc(L, ccmem_standard_allocator, 4096);

  memset(P, 0, 4096);
  ccmem_free(ccmem_standard_allocator, P);
}

void
test_1_2 (cce_destination_t L)
{
  void *	P = ccmem_malloc(L, ccmem_standard_allocator, 4096);

  memset(P, 0, 4096);
  {
    void *	Q = ccmem_realloc(L, ccmem_standard_allocator, P, 2 * 4096);

    memset(Q, 0, 2 * 4096);
    ccmem_free(ccmem_standard_allocator, Q);
  }
}

void
test_1_3 (cce_destination_t L)
{
  void *	P = ccmem_calloc(L, ccmem_standard_allocator, 16, 4096);

  memset(P, 0, 16 * 4096);
  ccmem_free(ccmem_standard_allocator, P);
}


/** --------------------------------------------------------------------
 ** Memory allocation and release: implicit allocator selector.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t L)
{
  void *	P = ccmem_std_malloc(L, 4096);

  memset(P, 0, 4096);
  ccmem_std_free(P);
}

void
test_2_2 (cce_destination_t L)
{
  void *	P = ccmem_std_malloc(L, 4096);

  memset(P, 0, 4096);
  {
    void *	Q = ccmem_std_realloc(L, P, 2 * 4096);

    memset(Q, 0, 2 * 4096);
    ccmem_std_free(Q);
  }
}

void
test_2_3 (cce_destination_t L)
{
  void *	P = ccmem_std_calloc(L, 16, 4096);

  memset(P, 0, 16 * 4096);
  ccmem_std_free(P);
}


int
main (void)
{
  cctests_init("tests for the standard memory allocator");
  {
    cctests_begin_group("memory allocation and release, explicit allocator selection");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
      cctests_run(test_1_3);
    }
    cctests_end_group();

    cctests_begin_group("memory allocation and release, implicit allocator selection");
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
