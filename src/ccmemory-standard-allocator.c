/*
  Part of: CCMemory
  Contents: memory allocators
  Date: May  5, 2018

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
 ** Predefined memory allocator.
 ** ----------------------------------------------------------------- */

static void *
ccmem_standard_allocator_malloc (ccmem_memory_allocator_t A CCMEM_UNUSED, cce_destination_t L, size_t size)
{
  return cce_sys_malloc(L, size);
}

void *
ccmem_standard_allocator_realloc (ccmem_memory_allocator_t A CCMEM_UNUSED, cce_destination_t L, void * ptr, size_t newsize)
{
  return cce_sys_realloc(L, ptr, newsize);
}

void *
ccmem_standard_allocator_calloc (ccmem_memory_allocator_t A CCMEM_UNUSED, cce_destination_t L, size_t count, size_t eltsize)
{
  return cce_sys_calloc(L, count, eltsize);
}

void
ccmem_standard_allocator_free (ccmem_memory_allocator_t A CCMEM_UNUSED, cce_destination_t L CCMEM_UNUSED, void * ptr)
{
  free(ptr);
}

static ccmem_memory_allocator_methods_t const ccmem_standard_allocator_methods_stru = {
  .malloc	= ccmem_standard_allocator_malloc,
  .realloc	= ccmem_standard_allocator_realloc,
  .calloc	= ccmem_standard_allocator_calloc,
  .free		= ccmem_standard_allocator_free
};

static ccmem_memory_allocator_t const ccmem_standard_allocator_stru = {
  .methods	= &ccmem_standard_allocator_methods_stru
};

ccmem_memory_allocator_t const * const ccmem_standard_allocator = &ccmem_standard_allocator_stru;

/* end of file */
