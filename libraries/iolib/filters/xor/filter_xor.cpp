 /* IOLib-C++, Universal IO Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
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

#include <iolib-cxx.h>
#include "filter_xor.h"

IOLibFilterXOR::IOLibFilterXOR()
{
	packageSize = 4;
	modifier = 0;
}

IOLibFilterXOR::~IOLibFilterXOR()
{
}

int IOLibFilterXOR::WriteData(unsigned char *data, int size)
{
	unsigned int value;

	value = data[3] + 256 * data[2] + 65536 * data[1] + 16777216 * data[0];

	value ^= modifier;

	return driver->WriteData((unsigned char *) &value, size);
}

int IOLibFilterXOR::ReadData(unsigned char **data, int size)
{
	unsigned int value;

	driver->ReadData((unsigned char *) &value, size);

	value ^= modifier;

	*data = new unsigned char [size];

	(*data)[3] = value % 256;
	(*data)[2] = value % 16777216 % 65536 / 256;
	(*data)[1] = value % 16777216 / 65536;
	(*data)[0] = value / 16777216;

	return size;
}

void IOLibFilterXOR::SetModifier(int mod)
{
	modifier = mod;
}
