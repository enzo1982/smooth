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

#include "driver_unicode.h"

IOLibDriverUnicode::IOLibDriverUnicode(const wchar_t *fileName, int mode) : IOLibDriver()
{
	switch (mode)
	{
		default:
			lastError = IOLIB_ERROR_BADPARAM;
			return;
		case 0:						// open a file for appending data
			stream = CreateFileW(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			Seek(GetSize());
			break;
		case 1:						// create or overwrite a file
			stream = CreateFileW(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			break;
		case 2:						// open a file for reading data
			stream = CreateFileW(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			break;
		case 3:						// open a file in read only mode
			stream = CreateFileW(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			break;
	}

	if (stream == INVALID_HANDLE_VALUE)
	{
		lastError = IOLIB_ERROR_UNEXPECTED;

		return;
	}

	closeStream = true;
}

IOLibDriverUnicode::IOLibDriverUnicode(HANDLE iStream) : IOLibDriver()
{
	stream		= iStream;
	closeStream	= false;
}

IOLibDriverUnicode::~IOLibDriverUnicode()
{
	if (closeStream) CloseHandle(stream);
}

int IOLibDriverUnicode::ReadData(unsigned char *data, int dataSize)
{
	unsigned long	 bytes;

	ReadFile(stream, (void *) data, (dataSize < (GetSize() - GetPos()) ? dataSize : (GetSize() - GetPos())), &bytes, NULL);

	return bytes;
}

int IOLibDriverUnicode::WriteData(unsigned char *data, int dataSize)
{
	unsigned long	 bytes;

	WriteFile(stream, (void *) data, dataSize, &bytes, NULL);

	return bytes;
}

int IOLibDriverUnicode::Seek(int newPos)
{
	return SetFilePointer(stream, newPos, NULL, FILE_BEGIN);
}

int IOLibDriverUnicode::GetSize()
{
	return GetFileSize(stream, NULL);
}

int IOLibDriverUnicode::GetPos()
{
	return SetFilePointer(stream, 0, NULL, FILE_CURRENT);
}

int IOLibDriverUnicode::Flush()
{
	if (FlushFileBuffers(stream))	return 0;
	else				return -1;
}
