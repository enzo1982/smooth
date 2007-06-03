 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/driver.h>
#include <memory.h>

S::IO::Driver::Driver()
{
	streamID	= NIL;

	size		= 2147483647;
	position	= 0;

	lastError	= IO_ERROR_OK;
}

S::IO::Driver::~Driver()
{
}

S::Int S::IO::Driver::GetLastError() const
{
	return lastError;
}

S::Int S::IO::Driver::ReadData(UnsignedByte *data, Int dataSize)
{
	memset((Void *) data, 0, dataSize);

	position += dataSize;

	return dataSize;
}

S::Int S::IO::Driver::WriteData(UnsignedByte *data, Int dataSize)
{
	position += dataSize;

	return dataSize;
}

S::Int64 S::IO::Driver::Seek(Int64 newPos)
{
	position = newPos;

	return position;
}

S::Int64 S::IO::Driver::GetSize() const
{
	return size;
}

S::Int64 S::IO::Driver::GetPos() const
{
	return position;
}

S::Bool S::IO::Driver::Flush()
{
	return True;
}

const S::String &S::IO::Driver::GetStreamID() const
{
	return streamID;
}
