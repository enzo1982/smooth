 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/filters/filter_bzip2.h>
#include <smooth/io/driver.h>
#include <bzlib.h>

S::IO::FilterBZip2::FilterBZip2()
{
	packageSize = -1;

	buffer = NIL;
}

S::IO::FilterBZip2::~FilterBZip2()
{
	if (buffer != NIL) delete [] buffer;
}

S::Int S::IO::FilterBZip2::WriteData(UnsignedByte *data, Int size)
{
	int		 outsize = size + size / 100 + 1000;
	unsigned char	*dest = new unsigned char [outsize];

	BZ2_bzBuffToBuffCompress((char *) dest, (unsigned int *) &outsize, (char *) data, size, 5, 0, 0);

	driver->WriteData(dest, outsize);

	delete [] dest;

	return outsize;
}

S::Int S::IO::FilterBZip2::ReadData(UnsignedByte **data, Int size)
{
	// try to guess the size of the uncompressed data

	int	 outsize;

	if (size < (200 * 1024))		outsize = size * 100 + 10000;
	else if (size > (20 * 1024 * 1024))	outsize = size * 5 + 10000;
	else					outsize = size * 10 + 10000;

	unsigned char	*src = new unsigned char [size];

	if (buffer != NIL) delete [] buffer;

	buffer = new unsigned char [outsize];

	driver->ReadData(src, size);

	BZ2_bzBuffToBuffDecompress((char *) buffer, (unsigned int *) &outsize, (char *) src, size, 0, 0);

	delete [] src;

	*data = buffer;

	return outsize;
}
