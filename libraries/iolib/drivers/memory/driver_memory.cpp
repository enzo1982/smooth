 /* IOLib-C++, Universal IO Library
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

#ifndef __IOLIB_DRIVER_MEMORY_
#define __IOLIB_DRIVER_MEMORY_

#include "driver_memory.h"

#include <memory.h>

IOLibDriverMemory::IOLibDriverMemory(void *iStream, int iSize) : IOLibDriver()
{
	stream	= iStream;
	size	= iSize;
	pos	= 0;
}

IOLibDriverMemory::~IOLibDriverMemory()
{
}

int IOLibDriverMemory::ReadData(unsigned char *data, int dataSize)
{
	dataSize = (dataSize < (GetSize() - GetPos()) ? dataSize : (GetSize() - GetPos()));

	memcpy((void *) data, (void *) ((unsigned char *) stream + pos), dataSize);

	pos += dataSize;

	return dataSize;
}

int IOLibDriverMemory::WriteData(unsigned char *data, int dataSize)
{
	dataSize = (dataSize < (GetSize() - GetPos()) ? dataSize : (GetSize() - GetPos()));

	memcpy((void *) ((unsigned char *) stream + pos), (void *) data, dataSize);

	pos += dataSize;

	return dataSize;
}

int IOLibDriverMemory::Seek(int newPos)
{
	if (pos >= size) return -1;

	pos = newPos;

	return pos;
}

int IOLibDriverMemory::GetSize()
{
	return size;
}

int IOLibDriverMemory::GetPos()
{
	return pos;
}

int IOLibDriverMemory::Flush()
{
	return 0;
}

#endif
