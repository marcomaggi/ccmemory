/*
  Part of: CCMemory
  Contents: tests for memory blocks
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

#include "ccmemory.h"
#include "cctests.h"


/** --------------------------------------------------------------------
 ** Memory allocation and release.
 ** ----------------------------------------------------------------- */

void
test_1_1 (cce_destination_t L)
{
  ccmem_block_t	B = ccmem_block_new(L, ccmem_standard_allocator, 4096);

  memset(B.ptr, 0, 4096);

  cctests_assert(L, 4096 == B.len);
  cctests_assert(L, NULL != B.ptr);

  ccmem_block_delete(L, ccmem_standard_allocator, B);
}

void
test_1_2 (cce_destination_t upper_L)
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccmem_block_t	B = ccmem_block_new(L, ccmem_standard_allocator, 4096);

    memset(B.ptr, 0, 4096);

    cctests_assert(L, 4096 == B.len);
    cctests_assert(L, NULL != B.ptr);

    ccmem_block_delete(L, ccmem_standard_allocator, B);

    cce_run_cleanup_handlers(L);
  }
}


int
main (void)
{
  cctests_init("tests for memory blocks");
  {
    cctests_begin_group("memory allocation and release");
    {
      cctests_run(test_1_1);
      cctests_run(test_1_2);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
