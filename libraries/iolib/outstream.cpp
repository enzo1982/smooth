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

#ifndef __IOLIB_OUTSTREAM_
#define __IOLIB_OUTSTREAM_

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <iolib-cxx.h>

#include "drivers/ansi/driver_ansi.h"
#include "drivers/posix/driver_posix.h"
#include "drivers/zero/driver_zero.h"
#include "drivers/memory/driver_memory.h"

 OutStream::OutStream(int type)
{
	if (type == STREAM_ZERO)
	{
		driver = new IOLibDriverZero();

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		lastfilepos	= 0;
		packageSize	= DEFAULT_PACKAGE_SIZE;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IOLIB_ERROR_BADPARAM; return; }
	}
}

OutStream::OutStream(int type, IOLibDriver *iDriver)
{
	if (type == STREAM_DRIVER)
	{
		driver = iDriver;

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentFilePos	= driver->GetPos();
		lastfilepos	= driver->GetPos();
		data		= new unsigned char [packageSize];

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

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentFilePos	= size;
		lastfilepos	= size;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IOLIB_ERROR_BADPARAM; return; }
	}
}

OutStream::OutStream(int type, int file)
{
	if (type == STREAM_POSIX)
	{
		driver = new IOLibDriverPOSIX(file);

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentFilePos	= driver->GetPos();
		lastfilepos	= 0;
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

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentFilePos	= driver->GetPos();
		lastfilepos	= 0;
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

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		lastfilepos	= 0;
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
		lastfilepos	= currentFilePos;
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

	if (pbd) CompletePBD();

	int	 ps		= packageSize;
	int	 oldcpos	= currentBufferPos;

	if (filter != NULLFILTER && filter->packageSize > 0)
	{
		for (int i = 0; i < (packageSize - oldcpos); i++)
		{
			OutputNumber(0, 1);
		}
	}
	else
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

	if (streamType == STREAM_DRIVER)
	{
		driver->Seek(lastfilepos);

		if (!filter->EncodeData(&data, packageSize, &encsize))
		{
			packageSize = 0;

			return false;
		}

		driver->WriteData(data, encsize);
		driver->Flush();

		delete [] data;

		data = new unsigned char [packageSize];

		currentBufferPos -= packageSize;
		if (size == currentFilePos) size -= (packageSize - encsize);
		currentFilePos -= (packageSize - encsize);
		lastfilepos += encsize;
	}

	return true;
}

bool OutStream::OutputNumber(long number, int bytes)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }
	if (bytes > 4 || bytes < 0)	{ lastError = IOLIB_ERROR_BADPARAM; return false; }

	if (pbd && !holdpbd) CompletePBD();

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

	if (pbd && !holdpbd) CompletePBD();

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

	if (pbd && !holdpbd) CompletePBD();

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

	if (!pbd) InitPBD();

	unsigned char	 out;

	for (int j = 0; j < bits; j++)
	{
		pbdbuffer[pbdlen] = GetBit(number, j);
		pbdlen++;
	}

	while (pbdlen >= 8)
	{
		out = 0;

		for (int i = 0; i < 8; i++)
		{
			out = out | (pbdbuffer[i] << i);
		}

		pbdlen = pbdlen - 8;

		for (int j = 0; j < pbdlen; j++)
		{
			pbdbuffer[j] = pbdbuffer[j+8];
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

	if (pbd && !holdpbd) CompletePBD();

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

	if (pbd && !holdpbd) CompletePBD();

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

	if (pbd && !holdpbd) CompletePBD();

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
	pbdlen	= 0;
	pbd	= 1;

	return true;
}

bool OutStream::CompletePBD()
{
	if (pbdlen > 0)
	{
		int	out = 0;

		for (int i = 0; i < 8; i++)
		{
			if (i < pbdlen) out = out | (pbdbuffer[i] << i);
		}

		holdpbd = true;
		OutputNumber(out, 1);
		holdpbd = false;
	}

	pbd = 0;

	return true;
}

bool OutStream::SetPackageSize(int newPackagesize)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }
	if (!allowpackset)		{ lastError = IOLIB_ERROR_OPNOTAVAIL; return false; }
	if (newPackagesize <= 0)	{ lastError = IOLIB_ERROR_BADPARAM; return false; }

	Flush();

	lastfilepos = currentFilePos;

	delete [] data;

	data = new unsigned char [newPackagesize];

	packageSize = newPackagesize;
	stdpacksize = packageSize;

	return true;
}

bool OutStream::SetFilter(IOLibFilter *newFilter)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }

	if (pbd && !holdpbd) CompletePBD();

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

	filter->Activate();

	return true;
}

bool OutStream::RemoveFilter()
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }

	if (pbd && !holdpbd) CompletePBD();

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

	filter = NULLFILTER;

	SetPackageSize(origpacksize);

	return true;
}

bool OutStream::Close()
{
	if (streamType == STREAM_NONE) { lastError = IOLIB_ERROR_NOTOPEN; return false; }

	if (filter != NULLFILTER) RemoveFilter();

	Flush();

	if (crosslinked)
	{
		if (closefile) inStream->closefile = true;

		inStream->crosslinked	= false;
		inStream->outStream	= NULL;

		closefile = false;
	}

	if (closefile && driver != NULLDRIVER) delete driver;

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
	lastfilepos		= position;

	return true;
}

bool OutStream::RelSeek(long offset)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }

	Flush();

	driver->Seek(currentFilePos + offset);

	currentFilePos		+= offset;
	currentBufferPos	= 0;
	lastfilepos		= currentFilePos;

	return true;
}

#endif
