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

#ifndef __IOLIB_DRIVER_
#define __IOLIB_DRIVER_

#include <iolib-cxx.h>
#include <memory.h>

IOLibDriver::IOLibDriver()
{
	size = 2147483647;
	position = 0;

	lastError = IOLIB_ERROR_OK;
}

IOLibDriver::~IOLibDriver()
{
}

int IOLibDriver::GetLastError()
{
	return lastError;
}

int IOLibDriver::ReadData(unsigned char *data, int dataSize)
{
	memset((void *) data, 0, dataSize);

	position += dataSize;

	return dataSize;
}

int IOLibDriver::WriteData(unsigned char *data, int dataSize)
{
	position += dataSize;

	return dataSize;
}

int IOLibDriver::Seek(int newPos)
{
	position = newPos;

	return position;
}

int IOLibDriver::GetSize()
{
	return size;
}

int IOLibDriver::GetPos()
{
	return position;
}

int IOLibDriver::Flush()
{
	return 0;
}

#endif
