 /* IOLib-Filters-C++, Filter package for IOLib-C++
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the GNU Library General Public
  * License as published by the Free Software Foundation; either
  * version 2 of the License, or (at your option) any later version.
  *
  * This library is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  * Library General Public License for more details.
  *
  * You should have received a copy of the GNU Library General Public
  * License along with this library; if not, write to the Free
  * Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
  * MA 02111-1307, USA */

#ifndef __IOLIB_FILTERS_BZ2_
#define __IOLIB_FILTERS_BZ2_

#include <iolib-cxx.h>
#include <filter-bz2-cxx.h>
#include <bzlib.h>
#include <memory.h>
#include <stdlib.h>

FilterBZ2::FilterBZ2()
{
	packageSize = -1;
}

FilterBZ2::~FilterBZ2()
{
}

bool FilterBZ2::EncodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size + size / 100 + 1000;

	unsigned char	*dest = new unsigned char [*outsize];

	BZ2_bzBuffToBuffCompress((char *) dest, (unsigned int *) outsize, (char *) *data, size, 5, 0, 0);

	delete [] *data;

	*data = new unsigned char [*outsize];

	memcpy((void *) *data, (void *) dest, *outsize);

	delete [] dest;

	return true;
}

bool FilterBZ2::DecodeData(unsigned char **data, int size, int *outsize)
{
	// try to guess the size of the uncompressed data

	if (size < (200 * 1024))		*outsize = size * 100 + 10000;
	else if (size > (20 * 1024 * 1024))	*outsize = size * 5 + 10000;
	else					*outsize = size * 10 + 10000;

	unsigned char	*dest = new unsigned char [*outsize];

	BZ2_bzBuffToBuffDecompress((char *) dest, (unsigned int *) outsize, (char *) *data, size, 0, 0);

	delete [] *data;

	*data = new unsigned char [*outsize];

	memcpy((void *) *data, (void *) dest, *outsize);

	delete [] dest;

	return true;
}

#endif
