/*
  Part of: CCMemory
  Contents: test for version functions
  Date: Sat Dec 17, 2016

  Abstract

	Test file for version functions.

  Copyright (C) 2016, 2018 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/

#include <ccmemory.h>
#include <stdio.h>
#include <stdlib.h>

int
main (void)
{
  printf("version number string: %s\n", ccmem_version_string());
  printf("libtool version number: %d:%d:%d\n",
	 ccmem_version_interface_current(),
	 ccmem_version_interface_revision(),
	 ccmem_version_interface_age());
  exit(EXIT_SUCCESS);
}

/* end of file */
