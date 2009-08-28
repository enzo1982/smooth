 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_memory.h>

#include <memory.h>

S::IO::DriverMemory::DriverMemory(Void *iStream, Int iSize) : Driver()
{
	stream	= iStream;
	size	= iSize;
}

S::IO::DriverMemory::~DriverMemory()
{
}

S::Int S::IO::DriverMemory::ReadData(UnsignedByte *data, Int dataSize)
{
	dataSize = (dataSize < (GetSize() - GetPos()) ? dataSize : (GetSize() - GetPos()));

	memcpy((void *) data, (void *) ((unsigned char *) stream + position), dataSize);

	position += dataSize;

	return dataSize;
}

S::Int S::IO::DriverMemory::WriteData(UnsignedByte *data, Int dataSize)
{
	dataSize = (dataSize < (GetSize() - GetPos()) ? dataSize : (GetSize() - GetPos()));

	memcpy((void *) ((unsigned char *) stream + position), (void *) data, dataSize);

	position += dataSize;

	return dataSize;
}


S::Int64 S::IO::DriverMemory::Seek(Int64 newPosition)
{
	if (newPosition > size) return -1;

	position = newPosition;

	return position;
}
