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

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <iolib-cxx.h>

#include "drivers/ansi/driver_ansi.h"
#include "drivers/posix/driver_posix.h"
#include "drivers/memory/driver_memory.h"

OutStream::OutStream(int type, IOLibDriver *iDriver)
{
	if (type == STREAM_DRIVER)
	{
		driver = iDriver;

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentFilePos	= driver->GetPos();
		data		= new unsigned char [packageSize];
		closefile	= false;

		return;
	}
	else
	{
		{ lastError = IOLIB_ERROR_BADPARAM; return; }
	}
}

OutStream::OutStream(int type, const char *file, int mode)
{
	if (type == STREAM_FILE)
	{
		driver = new IOLibDriverPOSIX(file, mode);

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentFilePos	= size;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IOLIB_ERROR_BADPARAM; return; }
	}
}

OutStream::OutStream(int type, FILE *openfile)
{
	if (type == STREAM_ANSI)
	{
		driver = new IOLibDriverANSI(openfile);

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentFilePos	= driver->GetPos();
		packageSize	= 1; // low package size, 'cause openfile could point at the console or so
		stdpacksize	= packageSize;
		origpacksize	= packageSize;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IOLIB_ERROR_BADPARAM; return; }
	}
}

OutStream::OutStream(int type, void *outbuffer, long bufsize)
{
	if (type == STREAM_BUFFER)
	{
		driver = new IOLibDriverMemory(outbuffer, bufsize);

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		packageSize	= 1;
		stdpacksize	= packageSize;
		origpacksize	= packageSize;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IOLIB_ERROR_BADPARAM; return; }
	}
}

OutStream::OutStream(int type, InStream *in)
{
	if (type != STREAM_STREAM)				{ lastError = IOLIB_ERROR_BADPARAM; return; }
	if (in->streamType == STREAM_NONE || in->crosslinked)	{ lastError = IOLIB_ERROR_OPNOTAVAIL; return; }

	streamType = STREAM_STREAM;

	crosslinked		= true;
	inStream		= in;
	inStream->outStream	= this;
	inStream->crosslinked	= true;

	if (inStream->streamType == STREAM_DRIVER)
	{
		streamType	= STREAM_DRIVER;
		closefile	= false;
		driver		= inStream->driver;
		currentFilePos	= inStream->currentFilePos;
		packageSize	= 1;
		stdpacksize	= packageSize;
		origpacksize	= packageSize;
		data		= new unsigned char [packageSize];
		size		= inStream->origsize;

		return;
	}
}

OutStream::~OutStream()
{
	Close(); // close stream; ignore return value
}

bool OutStream::Flush()
{
	if (streamType == STREAM_NONE) { lastError = IOLIB_ERROR_NOTOPEN; return false; }

	if (currentBufferPos <= 0) return true;

	if (pbdActive) CompletePBD();

	int	 ps		= packageSize;
	int	 oldcpos	= currentBufferPos;

	if (filter != NULL)
	{
		if (filter->packageSize > 0)
		{
			for (int i = 0; i < (packageSize - oldcpos); i++)
			{
				OutputNumber(0, 1);
			}
		}
	}

	if (currentBufferPos > 0)
	{
		packageSize = currentBufferPos;

		WriteData();

		packageSize = ps;

		delete [] data;

		data = new unsigned char [packageSize];
	}

	return true;
}

bool OutStream::WriteData()
{
	if (streamType == STREAM_NONE) { lastError = IOLIB_ERROR_NOTOPEN; return false; }

	if (currentBufferPos < packageSize) return true;

	unsigned char	*databuffer;
	int		 encsize = 0;

	if (filter != NULL)
	{
		if (filter->packageSize == -1)
		{
			databuffer = new unsigned char [packageSize];

			memcpy((void *) databuffer, (void *) data, packageSize);

			delete [] data;

			data = new unsigned char [packageSize + DEFAULT_PACKAGE_SIZE];

			memcpy((void *) data, (void *) databuffer, packageSize);

			delete [] databuffer;

			packageSize += DEFAULT_PACKAGE_SIZE;
			stdpacksize = packageSize;

			return true;
		}
	}

	if (streamType == STREAM_DRIVER)
	{
		if (filter == NULL)
		{
			encsize = driver->WriteData(data, packageSize);
		}
		else
		{
			encsize = filter->WriteData(data, packageSize);

			if (encsize == -1)
			{
				packageSize = 0;

				return false;
			}
		}

		driver->Flush();

		currentBufferPos -= packageSize;
		if (size == currentFilePos) size -= (packageSize - encsize);
		currentFilePos -= (packageSize - encsize);
	}

	return true;
}

bool OutStream::OutputNumber(long number, int bytes)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }
	if (bytes > 4 || bytes < 0)	{ lastError = IOLIB_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	for (int i = 0; i < bytes; i++)
	{
		if (currentBufferPos >= packageSize)
		{
			if (!WriteData()) { lastError = IOLIB_ERROR_UNKNOWN; return false; }
		}

		data[currentBufferPos] = GetByte(number, i);
		if (currentFilePos == size) size++;
		currentBufferPos++;
		currentFilePos++;
	}

	if (currentBufferPos >= packageSize)
	{
		if (!WriteData()) { lastError = IOLIB_ERROR_UNKNOWN; return false; }
	}

	return true;
}

bool OutStream::OutputNumberRaw(long number, int bytes)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }
	if (bytes > 4 || bytes < 0)	{ lastError = IOLIB_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	for (int i = bytes - 1; i >= 0; i--)
	{
		if (currentBufferPos >= packageSize)
		{
			if (!WriteData()) { lastError = IOLIB_ERROR_UNKNOWN; return false; }
		}

		data[currentBufferPos] = GetByte(number, i);
		if (currentFilePos == size) size++;
		currentBufferPos++;
		currentFilePos++;
	}

	if (currentBufferPos >= packageSize)
	{
		if (!WriteData()) { lastError = IOLIB_ERROR_UNKNOWN; return false; }
	}

	return true;
}

bool OutStream::OutputNumberPDP(long number, int bytes)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }
	if (bytes > 4 || bytes < 0)	{ lastError = IOLIB_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	number <<= 8 * (4 - bytes);

	for (int i = 0; i < 4; i++)
	{
		if (bytes >= (i ^ 1) + 1)
		{
			if (currentBufferPos >= packageSize)
			{
				if (!WriteData()) { lastError = IOLIB_ERROR_UNKNOWN; return false; }
			}

			data[currentBufferPos] = GetByte(number, (3 - i) ^ 1);
			if (currentFilePos == size) size++;
			currentBufferPos++;
			currentFilePos++;
		}
	}

	if (currentBufferPos >= packageSize)
	{
		if (!WriteData()) { lastError = IOLIB_ERROR_UNKNOWN; return false; }
	}

	return true;
}

bool OutStream::OutputNumberPBD(long number, int bits)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }
	if (bits > 32 || bits < 0)	{ lastError = IOLIB_ERROR_BADPARAM; return false; }

	if (!pbdActive) InitPBD();

	unsigned char	 out;

	for (int j = 0; j < bits; j++)
	{
		pbdBuffer[pbdLength] = GetBit(number, j);
		pbdLength++;
	}

	while (pbdLength >= 8)
	{
		out = 0;

		for (int i = 0; i < 8; i++)
		{
			out = out | (pbdBuffer[i] << i);
		}

		pbdLength = pbdLength - 8;

		for (int j = 0; j < pbdLength; j++)
		{
			pbdBuffer[j] = pbdBuffer[j+8];
		}

		data[currentBufferPos] = out;
		if (currentFilePos == size) size++;
		currentBufferPos++;
		currentFilePos++;

		if (currentBufferPos >= packageSize)
		{
			if (!WriteData()) { lastError = IOLIB_ERROR_UNKNOWN; return false; }
		}
	}

	return true;
}

bool OutStream::OutputString(const char *string)
{
	if (streamType == STREAM_NONE)		{ lastError = IOLIB_ERROR_NOTOPEN; return false; }
	if (string == NULL)			{ lastError = IOLIB_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	int	 bytesleft = strlen(string);
	int	 databufferpos = 0;
	int	 amount = 0;

	while (bytesleft)
	{
		amount = ((packageSize - currentBufferPos)<(bytesleft))?(packageSize - currentBufferPos):(bytesleft);

		memcpy((void *) (data + currentBufferPos), (void *) ((unsigned char *) string + databufferpos), amount);

		bytesleft -= amount;
		databufferpos += amount;
		currentBufferPos += amount;
		currentFilePos += amount;

		if (size < currentFilePos) size = currentFilePos;

		if (!WriteData()) { lastError = IOLIB_ERROR_UNKNOWN; return false; }
	}

	return true;
}

bool OutStream::OutputLine(const char *string)
{
	if (streamType == STREAM_NONE)		{ lastError = IOLIB_ERROR_NOTOPEN; return false; }
	if (string == NULL)			{ lastError = IOLIB_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	int	 bytesleft = strlen(string);
	int	 databufferpos = 0;
	int	 amount = 0;

	while (bytesleft)
	{
		amount = ((packageSize - currentBufferPos)<(bytesleft))?(packageSize - currentBufferPos):(bytesleft);

		memcpy((void *) (data + currentBufferPos), (void *) ((unsigned char *) string + databufferpos), amount);

		bytesleft -= amount;
		databufferpos += amount;
		currentBufferPos += amount;
		currentFilePos += amount;

		if (size < currentFilePos) size = currentFilePos;

		if (!WriteData()) { lastError = IOLIB_ERROR_UNKNOWN; return false; }
	}

#if (defined __WIN32__ || defined MSDOS) && !defined __CYGWIN32__
	OutputNumber(13, 1);
#endif

	OutputNumber(10, 1);

	return true;
}

bool OutStream::OutputData(const void *pointer, int bytes)
{
	if (streamType == STREAM_NONE)		{ lastError = IOLIB_ERROR_NOTOPEN; return false; }
	if (pointer == NULL)			{ lastError = IOLIB_ERROR_BADPARAM; return false; }
	if (bytes < 0)				{ lastError = IOLIB_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	int	 bytesleft = bytes;
	int	 databufferpos = 0;
	int	 amount = 0;

	while (bytesleft)
	{
		amount = ((packageSize - currentBufferPos)<(bytesleft))?(packageSize - currentBufferPos):(bytesleft);

		memcpy((void *) (data + currentBufferPos), (void *) ((unsigned char *) pointer + databufferpos), amount);

		bytesleft -= amount;
		databufferpos += amount;
		currentBufferPos += amount;
		currentFilePos += amount;

		if (size < currentFilePos) size = currentFilePos;

		if (!WriteData()) { lastError = IOLIB_ERROR_UNKNOWN; return false; }
	}

	return true;
}

bool OutStream::InitPBD()
{
	pbdLength	= 0;
	pbdActive	= 1;

	return true;
}

bool OutStream::CompletePBD()
{
	if (pbdLength > 0)
	{
		int	out = 0;

		for (int i = 0; i < 8; i++)
		{
			if (i < pbdLength) out = out | (pbdBuffer[i] << i);
		}

		keepPbd = true;
		OutputNumber(out, 1);
		keepPbd = false;
	}

	pbdActive = 0;

	return true;
}

bool OutStream::SetPackageSize(int newPackagesize)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }
	if (!allowpackset)		{ lastError = IOLIB_ERROR_OPNOTAVAIL; return false; }
	if (newPackagesize <= 0)	{ lastError = IOLIB_ERROR_BADPARAM; return false; }

	Flush();

	delete [] data;

	data = new unsigned char [newPackagesize];

	packageSize = newPackagesize;
	stdpacksize = packageSize;

	return true;
}

bool OutStream::SetFilter(IOLibFilter *newFilter)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	allowpackset = true;

	if (newFilter->packageSize > 0)
	{
		SetPackageSize(newFilter->packageSize);	// package size must be eqv filter size

		allowpackset = false;
	}
	else if (newFilter->packageSize == -1)
	{
		SetPackageSize(DEFAULT_PACKAGE_SIZE);

		allowpackset = false;
	}

	filter = newFilter;

	filter->SetDriver(driver);
	filter->Activate();

	return true;
}

bool OutStream::RemoveFilter()
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	int	 oldcpos = currentBufferPos;

	if (filter->packageSize > 0 && currentBufferPos != 0)
	{
		for (int i = 0; i < (packageSize - oldcpos); i++)
		{
			OutputNumber(0, 1);
		}
	}
	else if (filter->packageSize == -1)
	{
		filter->packageSize = 0;

		allowpackset = true;

		Flush();

		filter->packageSize = -1;
	}
	else
	{
		Flush();
	}

	allowpackset = true;

	filter->Deactivate();

	filter = NULL;

	SetPackageSize(origpacksize);

	return true;
}

bool OutStream::Close()
{
	if (streamType == STREAM_NONE) { lastError = IOLIB_ERROR_NOTOPEN; return false; }

	if (filter != NULL) RemoveFilter();

	Flush();

	if (crosslinked)
	{
		if (closefile) inStream->closefile = true;

		inStream->crosslinked	= false;
		inStream->outStream	= NULL;

		closefile = false;
	}

	if (closefile) delete driver;

	delete [] data;
	data = NULL;

	streamType = STREAM_NONE;

	return true;
}

bool OutStream::Seek(long position)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }

	Flush();

	driver->Seek(position);

	currentFilePos		= position;
	currentBufferPos	= 0;

	return true;
}

bool OutStream::RelSeek(long offset)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }

	Flush();

	driver->Seek(currentFilePos + offset);

	currentFilePos		+= offset;
	currentBufferPos	= 0;

	return true;
}
