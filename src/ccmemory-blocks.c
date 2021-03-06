/*
  Part of: CCMemory
  Contents: functions for memory blocks
  Date: May  8, 2018

  Abstract


  Copyright (C) 2018, 2020 Marco Maggi <mrc.mgg@gmail.com>

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
#include <stdio.h>


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

ccmem_block_t
ccmem_block_difference (ccmem_block_t A, ccmem_block_t B)
{
  assert(A.ptr == B.ptr);
  assert(A.len >= B.len);
  ccmem_block_t	C = {
    .ptr = A.ptr + B.len,
    .len = A.len - B.len
  };
  return C;
}

/* end of file */
