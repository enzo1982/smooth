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

#ifndef __IOLIB_INSTREAM_
#define __IOLIB_INSTREAM_

#include <stdio.h>
#include <stdarg.h>
#include <iolib-cxx.h>

#include "drivers/ansi/driver_ansi.cpp"
#include "drivers/posix/driver_posix.cpp"
#include "drivers/zero/driver_zero.cpp"
#include "drivers/memory/driver_memory.cpp"

 InStream::InStream(int type)
{
	if (type == STREAM_ZERO)
	{
		driver = new IOLibDriverZero();

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentBufferPos= DEFAULT_PACKAGE_SIZE;
		origsize	= size;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IOLIB_ERROR_BADPARAM; return; }
	}
}

InStream::InStream(int type, IOLibDriver *iDriver)
{
	if (type == STREAM_DRIVER)
	{
		driver = iDriver;

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentBufferPos= DEFAULT_PACKAGE_SIZE;
		origsize	= size;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IOLIB_ERROR_BADPARAM; return; }
	}
}

InStream::InStream(int type, const char *filename, int mode)
{
	if (type == STREAM_FILE)
	{
		driver = new IOLibDriverPOSIX(filename, mode);

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentBufferPos= DEFAULT_PACKAGE_SIZE;
		origsize	= size;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IOLIB_ERROR_BADPARAM; return; }
	}
}

InStream::InStream(int type, int file)
{
	if (type == STREAM_POSIX)
	{
		driver = new IOLibDriverPOSIX(file);

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentFilePos	= driver->GetPos();
		currentBufferPos= DEFAULT_PACKAGE_SIZE;
		origsize	= size;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IOLIB_ERROR_BADPARAM; return; }
	}
}

InStream::InStream(int type, FILE *openfile)
{
	if (type == STREAM_ANSI)
	{
		driver = new IOLibDriverANSI(openfile);

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentFilePos	= driver->GetPos();
		currentBufferPos= DEFAULT_PACKAGE_SIZE;
		origsize	= size;
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

InStream::InStream(int type, void *inbuffer, long bufsize)
{
	if (type == STREAM_BUFFER)
	{
		driver = new IOLibDriverMemory(inbuffer, bufsize);

		if (driver->GetLastError() != IOLIB_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		packageSize	= 1;
		stdpacksize	= packageSize;
		origpacksize	= packageSize;
		currentBufferPos= packageSize;
		origsize	= size;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IOLIB_ERROR_BADPARAM; return; }
	}
}

InStream::InStream(int type, OutStream *out)
{
	if (type != STREAM_STREAM)				{ lastError = IOLIB_ERROR_BADPARAM; return; }
	if (out->streamType == STREAM_NONE || out->crosslinked)	{ lastError = IOLIB_ERROR_OPNOTAVAIL; return; }

	streamType = STREAM_STREAM;

	crosslinked		= true;
	outStream		= out;
	outStream->inStream	= this;
	outStream->crosslinked	= true;

	if (outStream->streamType == STREAM_DRIVER)
	{
		streamType	= STREAM_DRIVER;
		closefile	= false;
		driver		= outStream->driver;
		currentFilePos	= outStream->currentFilePos;
		packageSize	= 1;
		stdpacksize	= packageSize;
		origpacksize	= packageSize;
		data		= new unsigned char [packageSize];
		currentBufferPos= packageSize;
		size		= outStream->size;
		origsize	= size;

		return;
	}
}

InStream::~InStream()
{
	Close();	// close stream
}

bool InStream::ReadData()
{
	if (streamType == STREAM_NONE) { lastError = IOLIB_ERROR_NOTOPEN; return false; }

	int decsize = 0;

	if (streamType == STREAM_DRIVER)
	{
		if (filter != NULLFILTER)
		{
			if (filter->packageSize > 0)	packageSize = filter->packageSize;
			else				packageSize = stdpacksize;
		}
		else	packageSize = stdpacksize;

		size		= origsize;
		currentFilePos	= origfilepos;

		delete [] data;

		data = new unsigned char [packageSize];

		driver->Seek(currentFilePos);

		driver->ReadData(data, ((packageSize)<(size-currentFilePos)?(packageSize):(size-currentFilePos)));

		if (packageSize <= size-currentFilePos || (filter != NULLFILTER && filter->packageSize == 0))
		{
			origfilepos = currentFilePos + packageSize;

			if (!filter->DecodeData(&data, ((packageSize)<(size-currentFilePos)?(packageSize):(size-currentFilePos)), &decsize))
			{
				packageSize = 0;

				return false;
			}

			packageSize	= decsize;
			origsize	= size;

			if (packageSize + currentFilePos > size) size = packageSize + currentFilePos;
		}

		currentBufferPos = 0;
	}

	return true;
}

long InStream::InputNumber(int bytes)	// Intel byte order DCBA
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return -1; }
	if (bytes > 4 || bytes < 0)	{ lastError = IOLIB_ERROR_BADPARAM; return -1; }

	if (pbd && !holdpbd) CompletePBD();

	long	 rval = 0;

	for (int i = 0; i < bytes; i++)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IOLIB_ERROR_UNKNOWN; return -1; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData()) { lastError = IOLIB_ERROR_UNKNOWN; return -1; }
		}

		rval += data[currentBufferPos] * (1 << (i * 8));
		currentBufferPos++;
		currentFilePos++;
	}

	return rval;
}

long InStream::InputNumberRaw(int bytes)	// Raw byte order ABCD
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return -1; }
	if (bytes > 4 || bytes < 0)	{ lastError = IOLIB_ERROR_BADPARAM; return -1; }

	if (pbd && !holdpbd) CompletePBD();

	long	 rval = 0;

	for (int i = bytes - 1; i >= 0; i--)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IOLIB_ERROR_UNKNOWN; return -1; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData()) { lastError = IOLIB_ERROR_UNKNOWN; return -1; }
		}

		rval += data[currentBufferPos] * (1 << (i * 8));
		currentBufferPos++;
		currentFilePos++;
	}

	return rval;
}

long InStream::InputNumberPDP(int bytes)	// PDP byte order BADC
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return -1; }
	if (bytes > 4 || bytes < 0)	{ lastError = IOLIB_ERROR_BADPARAM; return -1; }

	if (pbd && !holdpbd) CompletePBD();

	long	 rval = 0;

	for (int i = 0; i < 4; i++)
	{
		if (bytes >= (i ^ 1) + 1)
		{
			if (currentFilePos >= (origfilepos + packageSize)) { lastError = IOLIB_ERROR_UNKNOWN; return -1; }

			while (currentBufferPos >= packageSize)
			{
				if (!ReadData()) { lastError = IOLIB_ERROR_UNKNOWN; return -1; }
			}

			rval += (data[currentBufferPos] << (((3 - i) ^ 1) * 8)) >> (8 * (4 - bytes));
			currentBufferPos++;
			currentFilePos++;
		}
	}

	return rval;
}

long InStream::InputNumberPBD(int bits)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return -1; }
	if (bits > 32 || bits < 0)	{ lastError = IOLIB_ERROR_BADPARAM; return -1; }

	if (!pbd) InitPBD();

	unsigned char	 inp;
	long		 rval = 0;

	while (pbdlen < bits)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IOLIB_ERROR_UNKNOWN; return -1; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData()) { lastError = IOLIB_ERROR_UNKNOWN; return -1; }
		}

		inp = data[currentBufferPos];
		currentBufferPos++;
		currentFilePos++;

		for (int i = 0; i < 8; i++)
		{
			pbdbuffer[pbdlen] = GetBit(inp, i);
			pbdlen++;
		}
	}

	for (int i = 0; i < bits; i++)
	{
		rval = rval | (pbdbuffer[i] << i);
	}

	pbdlen = pbdlen - bits;

	for (int j = 0; j < pbdlen; j++)
	{
		pbdbuffer[j] = pbdbuffer[j + bits];
	}

	return rval;
}

char *InStream::InputString(int bytes)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return NULL; }
	if (bytes <= 0)			{ lastError = IOLIB_ERROR_BADPARAM; return NULL; }

	if (pbd && !holdpbd) CompletePBD();

	int	 bytesleft = bytes;
	int	 databufferpos = 0;
	int	 amount = 0;
	char	*rval = new char [bytes+1];

	while (bytesleft)
	{
		if (currentFilePos >= (origfilepos + packageSize))
		{
			delete [] rval;

			{ lastError = IOLIB_ERROR_UNKNOWN; return NULL; }
		}

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData()) { lastError = IOLIB_ERROR_UNKNOWN; return NULL; }
		}

		amount = ((packageSize - currentBufferPos)<(bytesleft))?(packageSize - currentBufferPos):(bytesleft);

		memcpy((void *) ((unsigned char *) rval + databufferpos), (void *) (data + currentBufferPos), amount);

		bytesleft -= amount;
		databufferpos += amount;
		currentBufferPos += amount;
		currentFilePos += amount;
	}

	rval[bytes] = 0;

	return rval;
}

char *InStream::InputLine()
{
	if (streamType == STREAM_NONE)		{ lastError = IOLIB_ERROR_NOTOPEN; return NULL; }

	if (pbd && !holdpbd) CompletePBD();

	long	 inpval;
	int	 bytes = 0;
	char	*rval;
	char	*linebuffer1 = new char [1024];
	char	*linebuffer2;

	for (int j = 0; j >= 0; j++)
	{
		for (int i = 0; i < 1024; i++)
		{
			inpval = InputNumber(1);

			if (inpval == -1)
			{
				linebuffer1[bytes] = 0;

				rval = new char [bytes + 1];
				for (int k = 0; k <= bytes; k++) rval[k] = linebuffer1[k];
				delete [] linebuffer1;

				return rval;
			}

			if (inpval != 13 && inpval != 10)
			{
				linebuffer1[bytes] = (char) inpval;
				bytes++;
			}
			else
			{
				if (inpval == 13) InputNumber(1);

				linebuffer1[bytes] = 0;

				rval = new char [bytes + 1];
				for (int k = 0; k <= bytes; k++) rval[k] = linebuffer1[k];
				delete [] linebuffer1;

				return rval;
			}
		}

		linebuffer2 = new char [bytes];

		for (int l = 0; l < bytes; l++) linebuffer2[l] = linebuffer1[l];

		delete [] linebuffer1;

		linebuffer1 = new char [bytes + 1024];

		for (int k = 0; k < bytes; k++) linebuffer1[k] = linebuffer2[k];

		delete [] linebuffer2;
	}

	return NULL; // never reached
}

void *InStream::InputData(void *pointer, int bytes)
{
	if (streamType == STREAM_NONE)		{ lastError = IOLIB_ERROR_NOTOPEN; return NULL; }
	if (pointer == NULL)			{ lastError = IOLIB_ERROR_BADPARAM; return NULL; }
	if (bytes < 0)				{ lastError = IOLIB_ERROR_BADPARAM; return NULL; }

	if (pbd && !holdpbd) CompletePBD();

	int	 bytesleft = bytes;
	int	 databufferpos = 0;
	int	 amount = 0;

	while (bytesleft)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IOLIB_ERROR_UNKNOWN; return NULL; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData()) { lastError = IOLIB_ERROR_UNKNOWN; return NULL; }
		}

		amount = ((packageSize - currentBufferPos)<(bytesleft))?(packageSize - currentBufferPos):(bytesleft);

		memcpy((void *) ((unsigned char *) pointer + databufferpos), (void *) (data + currentBufferPos), amount);

		bytesleft -= amount;
		databufferpos += amount;
		currentBufferPos += amount;
		currentFilePos += amount;
	}

	return pointer;
}

bool InStream::InitPBD()
{
	pbdlen	= 0;
	pbd	= 1;

	return true;
}

bool InStream::CompletePBD()
{
	pbdlen	= 0;
	pbd	= 0;

	return true;
}

bool InStream::SetPackageSize(int newPackagesize)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }
	if (!allowpackset)		{ lastError = IOLIB_ERROR_OPNOTAVAIL; return false; }
	if (newPackagesize <= 0)	{ lastError = IOLIB_ERROR_BADPARAM; return false; }

	if (pbd) CompletePBD();

	delete [] data;

	data = new unsigned char [newPackagesize];

	packageSize = newPackagesize;
	stdpacksize = packageSize;

	Seek(currentFilePos);

	return true;
}

bool InStream::SetFilter(IOLibFilter *newFilter)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }

	filter = newFilter;

	allowpackset = true;

	if (filter->packageSize > 0)
	{
		SetPackageSize(filter->packageSize);

		allowpackset = false;
	}
	else if (filter->packageSize == -1)
	{
		SetPackageSize(size - currentFilePos);

		allowpackset = false;
	}

	Seek(currentFilePos);

	filter->Activate();

	while (currentBufferPos >= packageSize)
	{
		if (!ReadData()) { lastError = IOLIB_ERROR_UNKNOWN; return false; }
	}

	return true;
}

bool InStream::RemoveFilter()
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }

	filter->Deactivate();

	filter = NULLFILTER;

	allowpackset = true;

	SetPackageSize(origpacksize);

	return true;
}

bool InStream::Close()
{
	if (streamType == STREAM_NONE) { lastError = IOLIB_ERROR_NOTOPEN; return false; }

	if (crosslinked)
	{
		if (closefile) outStream->closefile = true;

		outStream->crosslinked	= false;
		outStream->inStream	= NULL;

		closefile = false;
	}

	if (pbd) CompletePBD();

	if (closefile && driver != NULLDRIVER) delete driver;

	delete [] data;
	data = NULL;

	streamType = STREAM_NONE;

	return true;
}

bool InStream::Seek(long position)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }

	if (pbd) CompletePBD();

	driver->Seek(position);

	currentFilePos		= position;
	currentBufferPos	= packageSize;
	origfilepos		= position;

	return true;
}

bool InStream::RelSeek(long offset)
{
	if (streamType == STREAM_NONE)	{ lastError = IOLIB_ERROR_NOTOPEN; return false; }

	if (pbd) CompletePBD();

	driver->Seek(currentFilePos + offset);

	currentFilePos		+= offset;
	currentBufferPos	= packageSize;
	origfilepos		= currentFilePos;

	return true;
}

#endif
