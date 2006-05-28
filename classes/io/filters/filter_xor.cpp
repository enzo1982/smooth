 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/filters/filter_xor.h>
#include <smooth/io/driver.h>

S::IO::FilterXOR::FilterXOR()
{
	packageSize	= 4;
	modifier	= 0;
}

S::IO::FilterXOR::~FilterXOR()
{
}

S::Int S::IO::FilterXOR::WriteData(Buffer<UnsignedByte> &data, Int size)
{
	Int	 value = (data[3] + 256 * data[2] + 65536 * data[1] + 16777216 * data[0]) ^ modifier;

	return driver->WriteData((unsigned char *) &value, size);
}

S::Int S::IO::FilterXOR::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	data.Resize(size);

	driver->ReadData(data, size);

	Int	 value = (data[3] + 256 * data[2] + 65536 * data[1] + 16777216 * data[0]) ^ modifier;

	data[0] = (value >> 24) & 255;
	data[1] = (value >> 16) & 255;
	data[2] = (value >>  8) & 255;
	data[3] =  value	& 255;

	return size;
}

S::Void S::IO::FilterXOR::SetModifier(Int mod)
{
	modifier = mod;
}
