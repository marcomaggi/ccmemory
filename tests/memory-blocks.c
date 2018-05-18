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
#include <cctests.h>
#include <complex.h>


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

    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Memory allocation and release using exception handlers.
 ** ----------------------------------------------------------------- */

void
test_2_1 (cce_destination_t upper_L)
{
  cce_location_t		L[1];
  ccmem_block_clean_handler_t	B_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccmem_block_t	B = ccmem_block_new(L, ccmem_standard_allocator, 4096);
    ccmem_block_register_clean_handler(L, B_H, ccmem_standard_allocator, B);

    cctests_assert(L, 4096 == B.len);
    cctests_assert(L, NULL != B.ptr);

    cce_run_clean_handlers(L);
  }
}

void
test_2_2 (cce_destination_t upper_L)
{
  cce_location_t		L[1];
  ccmem_block_clean_handler_t	B_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    cce_location_t		inner_L[1];
    ccmem_block_error_handler_t	inner_B_H[1];
    ccmem_block_t		B;

    if (cce_location(L)) {
      cce_run_error_handlers_raise(inner_L, L);
    } else {
      B = ccmem_block_new(L, ccmem_standard_allocator, 4096);
      ccmem_block_register_error_handler(L, inner_B_H, ccmem_standard_allocator, B);

      cctests_assert(L, 4096 == B.len);
      cctests_assert(L, NULL != B.ptr);

      cce_run_clean_handlers(L);
    }

    ccmem_block_register_clean_handler(L, B_H, ccmem_standard_allocator, B);
    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Memory allocation and release using guarded constructors.
 ** ----------------------------------------------------------------- */

void
test_3_1 (cce_destination_t upper_L)
{
  cce_location_t		L[1];
  ccmem_block_clean_handler_t	B_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccmem_block_t	B = ccmem_block_new_guarded(L, B_H, ccmem_standard_allocator, 4096);

    cctests_assert(L, 4096 == B.len);
    cctests_assert(L, NULL != B.ptr);

    cce_run_clean_handlers(L);
  }
}

void
test_3_2 (cce_destination_t upper_L)
{
  cce_location_t		L[1];
  ccmem_block_clean_handler_t	B_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    cce_location_t		inner_L[1];
    ccmem_block_error_handler_t	inner_B_H[1];
    ccmem_block_t		B;

    if (cce_location(L)) {
      cce_run_error_handlers_raise(inner_L, L);
    } else {
      B = ccmem_block_new_guarded(L, inner_B_H, ccmem_standard_allocator, 4096);
      cctests_assert(L, 4096 == B.len);
      cctests_assert(L, NULL != B.ptr);

      cce_run_clean_handlers(L);
    }

    ccmem_block_register_clean_handler(L, B_H, ccmem_standard_allocator, B);
    cce_run_clean_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Operations.
 ** ----------------------------------------------------------------- */

void
test_4_1_1 (cce_destination_t L)
/* Test for "ccmem_block_shift()". */
{
  static const complex	C[3] = {
    CMPLX(1.0, 2.0),
    CMPLX(3.0, 4.0),
    CMPLX(5.0, 6.0)
  };

  ccmem_block_t	B = {
    .ptr = (uint8_t *)C,
    .len = 3 * sizeof(complex)
  };

  complex *	Z;

  Z = (complex *)B.ptr;
  cctests_assert_equal_double(L, 1.0, creal(*Z));
  cctests_assert_equal_double(L, 2.0, cimag(*Z));

  B = ccmem_block_shift(B, +1, sizeof(complex));
  Z = (complex *)B.ptr;
  cctests_assert_equal_double(L, 3.0, creal(*Z));
  cctests_assert_equal_double(L, 4.0, cimag(*Z));

  B = ccmem_block_shift(B, -1, sizeof(complex));
  Z = (complex *)B.ptr;
  cctests_assert_equal_double(L, 1.0, creal(*Z));
  cctests_assert_equal_double(L, 2.0, cimag(*Z));

  B = ccmem_block_shift(B, +2, sizeof(complex));
  Z = (complex *)B.ptr;
  cctests_assert_equal_double(L, 5.0, creal(*Z));
  cctests_assert_equal_double(L, 6.0, cimag(*Z));

  B = ccmem_block_shift(B, -2, sizeof(complex));
  Z = (complex *)B.ptr;
  cctests_assert_equal_double(L, 1.0, creal(*Z));
  cctests_assert_equal_double(L, 2.0, cimag(*Z));
}

/* ------------------------------------------------------------------ */

void
test_4_2_1 (cce_destination_t upper_L)
/* Test for "ccmem_block_difference()". */
{
  cce_location_t		L[1];
  ccmem_block_clean_handler_t	A_H[1];

  if (cce_location(L)) {
    cce_run_error_handlers_raise(L, upper_L);
  } else {
    ccmem_block_t	A = ccmem_block_new_guarded(L, A_H, ccmem_standard_allocator, 4096);
    ccmem_block_t	B = {
      .ptr = A.ptr,
      .len = 1024,
    };
    ccmem_block_t	C = ccmem_block_difference(A, B);

    cctests_assert_equal_pointer(L, C.ptr, A.ptr + 1024);
    cctests_assert_equal_size(L, C.len, A.len - 1024);

    cce_run_clean_handlers(L);
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

    cctests_begin_group("memory allocation and release using exception handlers");
    {
      cctests_run(test_2_1);
      cctests_run(test_2_2);
    }
    cctests_end_group();

    cctests_begin_group("memory allocation and release using guarded constructors");
    {
      cctests_run(test_3_1);
      cctests_run(test_3_2);
    }
    cctests_end_group();

    cctests_begin_group("operations");
    {
      cctests_run(test_4_1_1);
      cctests_run(test_4_2_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
