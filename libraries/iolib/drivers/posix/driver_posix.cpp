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

#include "driver_posix.h"

#include <fcntl.h>

#if !defined __WIN32__ || defined __CYGWIN32__
	#include <unistd.h>
#endif

#ifndef O_BINARY
	#define O_BINARY	0
#endif

#ifndef O_RANDOM
	#define O_RANDOM	0
#endif

IOLibDriverPOSIX::IOLibDriverPOSIX(const char *fileName, int mode) : IOLibDriver()
{
	closeStream = false;

	switch (mode)
	{
		default:
			lastError = IOLIB_ERROR_BADPARAM;
			return;
		case 0:						// open a file for appending data
			stream = open(fileName, O_RDWR | O_BINARY | O_RANDOM | O_CREAT, 0600);
			Seek(GetSize());
			break;
		case 1:						// create or overwrite a file
			stream = open(fileName, O_RDWR | O_BINARY | O_RANDOM | O_CREAT | O_TRUNC, 0600);
			break;
		case 2:						// open a file for reading data
			stream = open(fileName, O_RDWR | O_BINARY);
			break;
		case 3:						// open a file in read only mode
			stream = open(fileName, O_RDONLY | O_BINARY);
			break;
	}

	if (stream == -1)
	{
		lastError = IOLIB_ERROR_UNEXPECTED;

		return;
	}

	streamID = new char [strlen(fileName) + 1];

	strcpy(streamID, fileName);

	closeStream = true;
}

IOLibDriverPOSIX::IOLibDriverPOSIX(int iStream) : IOLibDriver()
{
	stream		= iStream;
	closeStream	= false;
}

IOLibDriverPOSIX::~IOLibDriverPOSIX()
{
	if (closeStream) close(stream);

	if (streamID != NULL) delete [] streamID;
}

int IOLibDriverPOSIX::ReadData(unsigned char *data, int dataSize)
{
	return read(stream, data, (dataSize < (GetSize() - GetPos()) ? dataSize : (GetSize() - GetPos())));
}

int IOLibDriverPOSIX::WriteData(unsigned char *data, int dataSize)
{
	return write(stream, data, dataSize);
}

int IOLibDriverPOSIX::Seek(int newPos)
{
	return lseek(stream, newPos, SEEK_SET);
}

int IOLibDriverPOSIX::GetSize()
{
	int	 oldPos = GetPos();
	int	 size = lseek(stream, 0, SEEK_END);

	lseek(stream, oldPos, SEEK_SET);

	return size;
}

int IOLibDriverPOSIX::GetPos()
{
	return lseek(stream, 0, SEEK_CUR);
}

int IOLibDriverPOSIX::Flush()
{
	return 0;
}
