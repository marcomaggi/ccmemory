/*
  Part of: CCMemory
  Contents: test for version functions
  Date: Sat Dec 17, 2016

  Abstract

	Test file for version functions.

  Copyright (C) 2016 Marco Maggi <marco.maggi-ipsu@poste.it>

  See the COPYING file.
*/

#include <stdio.h>
#include <stdlib.h>
#include <ccmemory.h>

int
main (int argc, const char *const argv[])
{
  printf("version number string: %s\n", ccm_version_string());
  printf("libtool version number: %d:%d:%d\n",
	 ccm_version_interface_current(),
	 ccm_version_interface_revision(),
	 ccm_version_interface_age());
  exit(EXIT_SUCCESS);
}

/* end of file */
