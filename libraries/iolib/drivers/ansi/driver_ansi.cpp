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

#include "driver_ansi.h"

IOLibDriverANSI::IOLibDriverANSI(const char *fileName, int mode) : IOLibDriver()
{
	switch (mode)
	{
		default:
			lastError = IOLIB_ERROR_BADPARAM;
			return;
		case 0:						// open a file for appending data
			stream = fopen(fileName, "r+b");
			Seek(GetSize());
			break;
		case 1:						// create or overwrite a file
			stream = fopen(fileName, "w+b");
			break;
		case 2:						// open a file for reading data
			stream = fopen(fileName, "r+b");
			break;
		case 3:						// open a file in read only mode
			stream = fopen(fileName, "rb");
			break;
	}

	if (stream == NULL)
	{
		lastError = IOLIB_ERROR_UNEXPECTED;

		return;
	}

	streamID = new char [strlen(fileName) + 1];

	strcpy(streamID, fileName);

	closeStream = true;
}

IOLibDriverANSI::IOLibDriverANSI(FILE *iStream) : IOLibDriver()
{
	stream		= iStream;
	closeStream	= false;
}

IOLibDriverANSI::~IOLibDriverANSI()
{
	if (closeStream) fclose(stream);
}

int IOLibDriverANSI::ReadData(unsigned char *data, int dataSize)
{
	return fread((void *) data, 1, (dataSize < (GetSize() - GetPos()) ? dataSize : (GetSize() - GetPos())), stream);
}

int IOLibDriverANSI::WriteData(unsigned char *data, int dataSize)
{
	return fwrite((void *) data, 1, dataSize, stream);
}

int IOLibDriverANSI::Seek(int newPos)
{
	return fseek(stream, newPos, SEEK_SET);
}

int IOLibDriverANSI::GetSize()
{
	int	 oldPos = GetPos();

	fseek(stream, 0, SEEK_END);

	int	 size = GetPos();

	fseek(stream, oldPos, SEEK_SET);

	return size;
}

int IOLibDriverANSI::GetPos()
{
	return ftell(stream);
}

int IOLibDriverANSI::Flush()
{
	return 0;
}
