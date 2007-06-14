 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_ansi.h>

#include <stdio.h>

#ifndef __WIN32__
#	define _wfopen fopen
#endif

S::IO::DriverANSI::DriverANSI(const String &fileName, Int mode) : Driver()
{
	closeStream = false;

	switch (mode)
	{
		default:
			lastError = IO_ERROR_BADPARAM;
			return;
		case 0:				// open a file for appending data
			if (Setup::enableUnicode)	stream = _wfopen(fileName, String("r+b"));
			else				stream = fopen(fileName, "r+b");
			Seek(GetSize());
			break;
		case 1:				// create or overwrite a file
			if (Setup::enableUnicode)	stream = _wfopen(fileName, String("w+b"));
			else				stream = fopen(fileName, "w+b");
			break;
		case 2:				// open a file for reading data
			if (Setup::enableUnicode)	stream = _wfopen(fileName, String("r+b"));
			else				stream = fopen(fileName, "r+b");
			break;
		case 3:				// open a file in read only mode
			if (Setup::enableUnicode)	stream = _wfopen(fileName, String("rb"));
			else				stream = fopen(fileName, "rb");
			break;
	}

	if (stream == NULL)
	{
		lastError = IO_ERROR_UNEXPECTED;

		return;
	}

	streamID = fileName;

	closeStream = true;
}

S::IO::DriverANSI::DriverANSI(FILE *iStream) : Driver()
{
	stream		= iStream;
	closeStream	= false;
}

S::IO::DriverANSI::~DriverANSI()
{
	if (closeStream) fclose(stream);
}

S::Int S::IO::DriverANSI::ReadData(UnsignedByte *data, Int dataSize)
{
	return fread((Void *) data, 1, (dataSize < (GetSize() - GetPos()) ? dataSize : (GetSize() - GetPos())), stream);
}

S::Int S::IO::DriverANSI::WriteData(UnsignedByte *data, Int dataSize)
{
	return fwrite((Void *) data, 1, dataSize, stream);
}

S::Int64 S::IO::DriverANSI::Seek(Int64 newPos)
{
	return fseek(stream, newPos, SEEK_SET);
}

S::Int64 S::IO::DriverANSI::GetSize() const
{
	Int64	 oldPos = GetPos();

	fseek(stream, 0, SEEK_END);

	Int64	 size = GetPos();

	fseek(stream, oldPos, SEEK_SET);

	return size;
}

S::Int64 S::IO::DriverANSI::GetPos() const
{
	return ftell(stream);
}
