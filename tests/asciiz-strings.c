/*
  Part of: CCMemory
  Contents: tests for ASCIIZ strings
  Date: Sep 27, 2018

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
 ** Constructors.
 ** ----------------------------------------------------------------- */

void
test_1_1_1 (cce_destination_t L)
/* Test for "ccmem_new_asciiz()". */
{
  static char *		str = "ciao";
  size_t 		len = strlen(str);
  ccmem_asciiz_t	S   = ccmem_new_asciiz(str, len);

  cctests_assert(L, ccmem_asciiz_is_terminated(S));
  cctests_assert_ascii(L, str, S.ptr, S.len);
  cctests_assert_asciiz(L, str, S.ptr);
  cctests_assert_equal_size(L, len, S.len);
}

void
test_1_2_1 (cce_destination_t L)
/* Test for "ccmem_new_asciiz_empty()". */
{
  static char *		str = "";
  ccmem_asciiz_t	S   = ccmem_new_asciiz_empty();

  cctests_assert(L, ccmem_asciiz_is_terminated(S));
  cctests_assert_ascii(L, str, S.ptr, S.len);
  cctests_assert_asciiz(L, str, S.ptr);
  cctests_assert_equal_size(L, 0, S.len);
}

void
test_1_3_1 (cce_destination_t L)
/* Test for "ccmem_new_asciiz_null()". */
{
  ccmem_asciiz_t	S   = ccmem_new_asciiz_null();

  cctests_assert(L, ! ccmem_asciiz_is_terminated(S));
  cctests_assert_equal_pointer(L, NULL, S.ptr);
  cctests_assert_equal_size(L, 0, S.len);
}

void
test_1_4_1 (cce_destination_t L)
/* Test for "ccmem_new_asciiz_from_str()". */
{
  static char *		str = "ciao";
  size_t 		len = strlen(str);
  ccmem_asciiz_t	S = ccmem_new_asciiz_from_str(str);

  cctests_assert(L, ccmem_asciiz_is_terminated(S));
  cctests_assert_ascii(L, str, S.ptr, S.len);
  cctests_assert_asciiz(L, str, S.ptr);
  cctests_assert_equal_size(L, len, S.len);
}

void
test_1_5_1 (cce_destination_t L)
/* Test for "ccmem_new_asciiz_from_block()". */
{
  static char *		str = "ciao";
  size_t 		len = strlen(str);
  ccmem_block_t		B   = ccmem_new_block((uint8_t *)str, 1+len);
  ccmem_asciiz_t	S   = ccmem_new_asciiz_from_block(B);

  cctests_assert(L, ccmem_asciiz_is_terminated(S));
  cctests_assert_equal_pointer(L, B.ptr, S.ptr);
  cctests_assert_equal_size(L, B.len, 1+S.len);
}


/** --------------------------------------------------------------------
 ** Predicates.
 ** ----------------------------------------------------------------- */

void
test_2_1_1 (cce_destination_t L)
/* Test for "ccmem_asciiz_is_empty()". */
{
  static char *		str = "ciao";
  size_t 		len = strlen(str);
  ccmem_asciiz_t	S   = ccmem_new_asciiz(str, len);

  cctests_assert(L, ! ccmem_asciiz_is_empty(S));
}

void
test_2_1_2 (cce_destination_t L)
/* Test for "ccmem_asciiz_is_empty()". */
{
  ccmem_asciiz_t	S = ccmem_new_asciiz_empty();

  cctests_assert(L, ccmem_asciiz_is_empty(S));
}

void
test_2_1_3 (cce_destination_t L)
/* Test for "ccmem_asciiz_is_empty()". */
{
  ccmem_asciiz_t	S = ccmem_new_asciiz_null();

  cctests_assert(L, ! ccmem_asciiz_is_empty(S));
}

/* ------------------------------------------------------------------ */

void
test_2_2_1 (cce_destination_t L)
/* Test for "ccmem_asciiz_is_null()". */
{
  static char *		str = "ciao";
  size_t 		len = strlen(str);
  ccmem_asciiz_t	S   = ccmem_new_asciiz(str, len);

  cctests_assert(L, ! ccmem_asciiz_is_null(S));
}

void
test_2_2_2 (cce_destination_t L)
/* Test for "ccmem_asciiz_is_null()". */
{
  ccmem_asciiz_t	S   = ccmem_new_asciiz_empty();

  cctests_assert(L, ! ccmem_asciiz_is_null(S));
}

void
test_2_2_3 (cce_destination_t L)
/* Test for "ccmem_asciiz_is_null()". */
{
  ccmem_asciiz_t	S = ccmem_new_asciiz_null();

  cctests_assert(L, ccmem_asciiz_is_null(S));
}


/** --------------------------------------------------------------------
 ** Comparison.
 ** ----------------------------------------------------------------- */

void
test_3_1_1 (cce_destination_t L)
/* Test for "ccmem_asciiz_equal()". */
{
  static char *		str = "ciao";
  size_t 		len = strlen(str);
  ccmem_asciiz_t	S1  = ccmem_new_asciiz(str, len);
  ccmem_asciiz_t	S2  = ccmem_new_asciiz(str, len);

  cctests_assert(L, ccmem_asciiz_equal(S1, S2));
}

void
test_3_1_2 (cce_destination_t L)
/* Test for "ccmem_asciiz_equal()". */
{
  static char *		str = "ciao";
  size_t 		len = strlen(str);
  ccmem_asciiz_t	S1  = ccmem_new_asciiz(str, len);
  ccmem_asciiz_t	S2  = ccmem_new_asciiz(str, len-1);

  cctests_assert(L, ! ccmem_asciiz_equal(S1, S2));
}

void
test_3_1_3 (cce_destination_t L)
/* Test for "ccmem_asciiz_equal()". */
{
  static char *		str = "ciao";
  size_t 		len = strlen(str);
  ccmem_asciiz_t	S1  = ccmem_new_asciiz(str, len-1);
  ccmem_asciiz_t	S2  = ccmem_new_asciiz(str, len);

  cctests_assert(L, ! ccmem_asciiz_equal(S1, S2));
}

void
test_3_1_4 (cce_destination_t L)
/* Test for "ccmem_asciiz_equal()". */
{
  static char *		str1 = "ciao";
  static char *		str2 = "hey!";
  size_t 		len1 = strlen(str1);
  size_t 		len2 = strlen(str2);
  ccmem_asciiz_t	S1  = ccmem_new_asciiz(str1, len1);
  ccmem_asciiz_t	S2  = ccmem_new_asciiz(str2, len2);

  cctests_assert(L, ! ccmem_asciiz_equal(S1, S2));
}


/** --------------------------------------------------------------------
 ** Operations.
 ** ----------------------------------------------------------------- */

void
test_4_1_1 (cce_destination_t L)
/* Test for "ccmem_asciiz_clean_memory()". */
{
  size_t 		len     = 4;
  char			str1[5] = { 0x01, 0x02, 0x03, 0x04, 0x00 };
  char			str2[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };
  ccmem_asciiz_t	S       = ccmem_new_asciiz(str1, len);

  ccmem_asciiz_clean_memory(S);
  cctests_assert(L, 0 == memcmp(str2, S.ptr, S.len));
  cctests_assert(L, ccmem_asciiz_is_terminated(S));
}


/** --------------------------------------------------------------------
 ** Dynamic memory allocation.
 ** ----------------------------------------------------------------- */

void
test_5_1_1 (cce_destination_t upper_L)
/* Test for "ccmem_asciiz_malloc()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccmem_asciiz_t	S = ccmem_asciiz_malloc(L, ccmem_standard_allocator, 4);

    S.ptr[0] = 'c';
    S.ptr[1] = 'i';
    S.ptr[2] = 'a';
    S.ptr[3] = 'o';

    cctests_assert_equal_size(L, 4, S.len);
    cctests_assert(L, ccmem_asciiz_is_terminated(S));
    cctests_assert_asciiz(L, "ciao", S.ptr);

    ccmem_asciiz_free(ccmem_standard_allocator, S);
    cce_run_body_handlers(L);
  }
}

void
test_5_2_1 (cce_destination_t upper_L)
/* Test for "ccmem_asciiz_realloc()". */
{
  cce_location_t	L[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccmem_asciiz_t	S = ccmem_asciiz_malloc(L, ccmem_standard_allocator, 4);

    S.ptr[0] = 'c';
    S.ptr[1] = 'i';
    S.ptr[2] = 'a';
    S.ptr[3] = 'o';

    cctests_assert_equal_size(L, 4, S.len);
    cctests_assert(L, ccmem_asciiz_is_terminated(S));
    cctests_assert_asciiz(L, "ciao", S.ptr);

    S = ccmem_asciiz_realloc(L, ccmem_standard_allocator, S, 4+6);

    S.ptr[4] = ' ';
    S.ptr[5] = 'm';
    S.ptr[6] = 'a';
    S.ptr[7] = 'm';
    S.ptr[8] = 'm';
    S.ptr[9] = 'a';

    cctests_assert_equal_size(L, 4+6, S.len);
    cctests_assert(L, ccmem_asciiz_is_terminated(S));
    cctests_assert_asciiz(L, "ciao mamma", S.ptr);

    ccmem_asciiz_free(ccmem_standard_allocator, S);
    cce_run_body_handlers(L);
  }
}


/** --------------------------------------------------------------------
 ** Guarded memory allocation.
 ** ----------------------------------------------------------------- */

void
test_6_1_1 (cce_destination_t upper_L)
/* Test for "ccmem_asciiz_malloc_guardeda()". */
{
  cce_location_t	L[1];
  ccmem_clean_handler_t	S_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccmem_asciiz_t	S = ccmem_asciiz_malloc_guarded(L, S_H, ccmem_standard_allocator, 4);

    S.ptr[0] = 'c';
    S.ptr[1] = 'i';
    S.ptr[2] = 'a';
    S.ptr[3] = 'o';

    cctests_assert_equal_size(L, 4, S.len);
    cctests_assert_asciiz(L, "ciao", S.ptr);
    cce_run_body_handlers(L);
  }
}

void
test_6_2_1 (cce_destination_t upper_L)
/* Test for "ccmem_asciiz_realloc_guarded()". */
{
  cce_location_t	L[1];
  ccmem_clean_handler_t	S_H[1];

  if (cce_location(L)) {
    cce_run_catch_handlers_raise(L, upper_L);
  } else {
    ccmem_asciiz_t	S = ccmem_asciiz_malloc_guarded(L, S_H, ccmem_standard_allocator, 4);

    S.ptr[0] = 'c';
    S.ptr[1] = 'i';
    S.ptr[2] = 'a';
    S.ptr[3] = 'o';

    cctests_assert_equal_size(L, 4, S.len);
    cctests_assert_asciiz(L, "ciao", S.ptr);

    S = ccmem_asciiz_realloc_guarded(L, S_H, ccmem_standard_allocator, S, 4+6);

    S.ptr[4] = ' ';
    S.ptr[5] = 'm';
    S.ptr[6] = 'a';
    S.ptr[7] = 'm';
    S.ptr[8] = 'm';
    S.ptr[9] = 'a';

    cctests_assert_equal_size(L, 4+6, S.len);
    cctests_assert_asciiz(L, "ciao mamma", S.ptr);
    cce_run_body_handlers(L);
  }
}


int
main (void)
{
  cctests_init("tests for ASCIIZ strings");
  {
    cctests_begin_group("constructors");
    {
      cctests_run(test_1_1_1);
      cctests_run(test_1_2_1);
      cctests_run(test_1_3_1);
      cctests_run(test_1_4_1);
      cctests_run(test_1_5_1);
    }
    cctests_end_group();

    cctests_begin_group("predicates");
    {
      cctests_run(test_2_1_1);
      cctests_run(test_2_1_2);
      cctests_run(test_2_1_3);
      cctests_run(test_2_2_1);
      cctests_run(test_2_2_2);
      cctests_run(test_2_2_3);
    }
    cctests_end_group();

    cctests_begin_group("comparison");
    {
      cctests_run(test_3_1_1);
      cctests_run(test_3_1_2);
      cctests_run(test_3_1_3);
      cctests_run(test_3_1_4);
    }
    cctests_end_group();

    cctests_begin_group("operations");
    {
      cctests_run(test_4_1_1);
    }

    cctests_begin_group("dynamic memory allocation");
    {
      cctests_run(test_5_1_1);
      cctests_run(test_5_2_1);
    }

    cctests_begin_group("guarded memory allocation");
    {
      cctests_run(test_6_1_1);
      cctests_run(test_6_2_1);
    }
    cctests_end_group();
  }
  cctests_final();
}

/* end of file */
