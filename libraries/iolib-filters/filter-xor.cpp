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

#ifndef __IOLIB_FILTERS_XOR_
#define __IOLIB_FILTERS_XOR_

#include <iolib-cxx.h>
#include <filter-xor-cxx.h>

FilterXOR::FilterXOR()
{
	packageSize = 4;
	modifier = 0;
}

FilterXOR::~FilterXOR()
{
}

bool FilterXOR::EncodeData(unsigned char **data, int size, int *outsize)
{
	unsigned int value;

	*outsize = size;

	value = (*data)[3] + 256 * (*data)[2] + 65536 * (*data)[1] + 16777216 * (*data)[0];

	value ^= modifier;

	(*data)[3] = value % 256;
	(*data)[2] = value % 16777216 % 65536 / 256;
	(*data)[1] = value % 16777216 / 65536;
	(*data)[0] = value / 16777216;

	return true;
}

bool FilterXOR::DecodeData(unsigned char **data, int size, int *outsize)
{
	return EncodeData(data, size, outsize);
}

void FilterXOR::SetModifier(int mod)
{
	modifier = mod;
}

#endif
