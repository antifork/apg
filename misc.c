/* Miscellaneous generic support functions.
Copyright (C) 1988,89,90,91,92,93,94,95,97 Free Software Foundation, Inc.
This file is part of GNU Make.

GNU Make is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Make is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Make; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */


/* xmalloc, xcalloc, xrealloc */
#include <stdlib.h>

void *
xmalloc(size)
	unsigned int size;
{
	void *result = malloc(size);
	if (result == 0)
		fatalerr("xmalloc: virtual memory exhausted");
	return result;
}


void *
xrealloc(ptr, size)
	void *ptr;
	unsigned int size;
{
	void *result;

	/* Some older implementations of realloc() don't conform to ANSI.  */
	result = ptr ? realloc(ptr, size) : malloc(size);
	if (result == 0)
		fatalerr("xrealloc: virtual memory exhausted");
	return result;
}

void *
xcalloc(nelem, elsize)
	unsigned int nelem;
	unsigned int elsize;
{
	void *result = (char *) calloc(nelem, elsize);
	if (result == 0)
		fatalerr("xcalloc: virtual memory exhausted");
	return result;
}
