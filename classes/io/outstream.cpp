 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/instream.h>
#include <smooth/io/outstream.h>
#include <smooth/io/filter.h>

#include <smooth/io/drivers/driver_ansi.h>
#include <smooth/io/drivers/driver_posix.h>
#include <smooth/io/drivers/driver_memory.h>

#ifdef __WIN32__
#	include <smooth/io/drivers/driver_win32.h>
#endif

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>

S::IO::OutStream::OutStream(Int type, Driver *iDriver)
{
	if (type != STREAM_DRIVER)		   { lastError = IO_ERROR_BADPARAM;	 return; }

	driver = iDriver;

	if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); return; }

	streamType	= STREAM_DRIVER;
	size		= driver->GetSize();
	currentFilePos	= driver->GetPos();
	closefile	= false;

	dataBuffer.Resize(packageSize);
}

S::IO::OutStream::OutStream(Int type, const String &file, Int mode)
{
	if (type != STREAM_FILE)		   { lastError = IO_ERROR_BADPARAM;			return; }

#ifdef __WIN32__
	driver = new DriverWin32(file, mode);
#else
	driver = new DriverPOSIX(file, mode);
#endif

	if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

	streamType	= STREAM_DRIVER;
	size		= driver->GetSize();
	currentFilePos	= size;

	dataBuffer.Resize(packageSize);
}

S::IO::OutStream::OutStream(Int type, FILE *openfile)
{
	if (type != STREAM_ANSI)		   { lastError = IO_ERROR_BADPARAM;			return; }

	driver = new DriverANSI(openfile);

	if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

	streamType	= STREAM_DRIVER;
	size		= driver->GetSize();
	currentFilePos	= driver->GetPos();
	packageSize	= 1; // low package size, 'cause openfile could point at the console or so
	stdpacksize	= packageSize;
	origpacksize	= packageSize;

	dataBuffer.Resize(packageSize);
}

S::IO::OutStream::OutStream(Int type, Void *outbuffer, Long bufsize)
{
	if (type != STREAM_BUFFER)		   { lastError = IO_ERROR_BADPARAM;			return; }

	driver = new DriverMemory(outbuffer, bufsize);

	if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

	streamType	= STREAM_DRIVER;
	size		= driver->GetSize();
	packageSize	= 1;
	stdpacksize	= packageSize;
	origpacksize	= packageSize;

	dataBuffer.Resize(packageSize);
}

S::IO::OutStream::OutStream(Int type, InStream *in)
{
	if (type != STREAM_STREAM)			      { lastError = IO_ERROR_BADPARAM;	 return; }
	if (in->streamType == STREAM_NONE || in->crosslinked) { lastError = IO_ERROR_OPNOTAVAIL; return; }

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
		size		= inStream->origsize;

		dataBuffer.Resize(packageSize);
	}
}

S::IO::OutStream::~OutStream()
{
	Close(); // close stream; ignore return value
}

S::Bool S::IO::OutStream::Flush()
{
	if (streamType == STREAM_NONE) { lastError = IO_ERROR_NOTOPEN; return false; }

	if (currentBufferPos <= 0) return true;

	if (pbdActive) CompletePBD();

	int	 ps		= packageSize;
	int	 oldcpos	= currentBufferPos;

	if (filters.Length() > 0)
	{
		if (filters.GetFirst()->GetPackageSize() > 0)
		{
			for (int i = 0; i < (packageSize - oldcpos); i++) OutputNumber(0, 1);
		}
	}

	if (currentBufferPos > 0)
	{
		packageSize = currentBufferPos;

		WriteData();

		packageSize = ps;
	}

	return true;
}

S::Bool S::IO::OutStream::WriteData()
{
	if (streamType == STREAM_NONE) { lastError = IO_ERROR_NOTOPEN; return false; }

	if (currentBufferPos < packageSize) return true;

	Int		 encsize = 0;

	if (filters.Length() > 0)
	{
		if (filters.GetFirst()->GetPackageSize() == -1)
		{
			backBuffer.Resize(packageSize);

			memcpy(backBuffer, dataBuffer, packageSize);

			dataBuffer.Resize(packageSize + defaultPackageSize);

			memcpy(dataBuffer, backBuffer, packageSize);

			packageSize += defaultPackageSize;
			stdpacksize = packageSize;

			return true;
		}
	}

	if (streamType == STREAM_DRIVER)
	{
		if (filters.Length() == 0) encsize = driver->WriteData(dataBuffer, packageSize);
		else			   encsize = filters.GetFirst()->WriteData(dataBuffer, packageSize);

		if (encsize == -1)
		{
			packageSize = 0;

			return false;
		}

		driver->Flush();

		if (size == currentFilePos) size -= (packageSize - encsize);

		currentFilePos	 -= (packageSize - encsize);
		currentBufferPos -= packageSize;
	}

	return true;
}

S::Bool S::IO::OutStream::OutputNumber(Long number, Int bytes)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }
	if (bytes > 4 || bytes < 0)	{ lastError = IO_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	for (int i = 0; i < bytes; i++)
	{
		if (currentBufferPos >= packageSize)
		{
			if (!WriteData()) { lastError = IO_ERROR_UNKNOWN; return false; }
		}

		dataBuffer[currentBufferPos] = IOGetByte(number, i);
		if (currentFilePos == size) size++;
		currentBufferPos++;
		currentFilePos++;
	}

	if (currentBufferPos >= packageSize)
	{
		if (!WriteData()) { lastError = IO_ERROR_UNKNOWN; return false; }
	}

	return true;
}

S::Bool S::IO::OutStream::OutputNumberRaw(Long number, Int bytes)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }
	if (bytes > 4 || bytes < 0)	{ lastError = IO_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	for (int i = bytes - 1; i >= 0; i--)
	{
		if (currentBufferPos >= packageSize)
		{
			if (!WriteData()) { lastError = IO_ERROR_UNKNOWN; return false; }
		}

		dataBuffer[currentBufferPos] = IOGetByte(number, i);
		if (currentFilePos == size) size++;
		currentBufferPos++;
		currentFilePos++;
	}

	if (currentBufferPos >= packageSize)
	{
		if (!WriteData()) { lastError = IO_ERROR_UNKNOWN; return false; }
	}

	return true;
}

S::Bool S::IO::OutStream::OutputNumberPDP(Long number, Int bytes)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }
	if (bytes > 4 || bytes < 0)	{ lastError = IO_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	number <<= 8 * (4 - bytes);

	for (int i = 0; i < 4; i++)
	{
		if (bytes >= (i ^ 1) + 1)
		{
			if (currentBufferPos >= packageSize)
			{
				if (!WriteData()) { lastError = IO_ERROR_UNKNOWN; return false; }
			}

			dataBuffer[currentBufferPos] = IOGetByte(number, (3 - i) ^ 1);
			if (currentFilePos == size) size++;
			currentBufferPos++;
			currentFilePos++;
		}
	}

	if (currentBufferPos >= packageSize)
	{
		if (!WriteData()) { lastError = IO_ERROR_UNKNOWN; return false; }
	}

	return true;
}

S::Bool S::IO::OutStream::OutputNumberPBD(Long number, Int bits)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }
	if (bits > 32 || bits < 0)	{ lastError = IO_ERROR_BADPARAM; return false; }

	if (!pbdActive) InitPBD();

	unsigned char	 out;

	for (int j = 0; j < bits; j++)
	{
		pbdBuffer[pbdLength] = IOGetBit(number, j);
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

		dataBuffer[currentBufferPos] = out;
		if (currentFilePos == size) size++;
		currentBufferPos++;
		currentFilePos++;

		if (currentBufferPos >= packageSize)
		{
			if (!WriteData()) { lastError = IO_ERROR_UNKNOWN; return false; }
		}
	}

	return true;
}

S::Bool S::IO::OutStream::OutputString(const String &string)
{
	if (streamType == STREAM_NONE)		{ lastError = IO_ERROR_NOTOPEN; return false; }
	if (string == NIL)			{ lastError = IO_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	int	 bytesleft	= strlen(string);
	int	 databufferpos	= 0;
	int	 amount		= 0;

	while (bytesleft)
	{
		amount = ((packageSize - currentBufferPos) < (bytesleft)) ? (packageSize - currentBufferPos) : (bytesleft);

		for (Int i = 0; i < amount; i++) ((char *) (((UnsignedByte *) dataBuffer) + currentBufferPos))[i] = ((char *) string)[databufferpos + i];

		bytesleft	 -= amount;
		databufferpos	 += amount;
		currentBufferPos += amount;
		currentFilePos	 += amount;

		if (size < currentFilePos) size = currentFilePos;

		if (!WriteData()) { lastError = IO_ERROR_UNKNOWN; return false; }
	}

	return true;
}

S::Bool S::IO::OutStream::OutputLine(const String &string)
{
	OutputString(string);

#if (defined __WIN32__ || defined MSDOS) && !defined __CYGWIN32__
	OutputNumber(13, 1);
#endif

	OutputNumber(10, 1);

	return true;
}

S::Bool S::IO::OutStream::OutputData(const Void *pointer, Int bytes)
{
	if (streamType == STREAM_NONE)		{ lastError = IO_ERROR_NOTOPEN; return false; }
	if (pointer == NULL)			{ lastError = IO_ERROR_BADPARAM; return false; }
	if (bytes < 0)				{ lastError = IO_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	int	 bytesleft	= bytes;
	int	 databufferpos	= 0;
	int	 amount		= 0;

	while (bytesleft)
	{
		amount = ((packageSize - currentBufferPos) < (bytesleft)) ? (packageSize - currentBufferPos) : (bytesleft);

		memcpy((void *) (((UnsignedByte *) dataBuffer) + currentBufferPos), (void *) ((unsigned char *) pointer + databufferpos), amount);

		bytesleft	 -= amount;
		databufferpos	 += amount;
		currentBufferPos += amount;
		currentFilePos	 += amount;

		if (size < currentFilePos) size = currentFilePos;

		if (!WriteData()) { lastError = IO_ERROR_UNKNOWN; return false; }
	}

	return true;
}

S::Bool S::IO::OutStream::InitPBD()
{
	pbdLength	= 0;
	pbdActive	= 1;

	return true;
}

S::Bool S::IO::OutStream::CompletePBD()
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

S::Bool S::IO::OutStream::SetPackageSize(Int newPackageSize)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }
	if (!allowpackset)		{ lastError = IO_ERROR_OPNOTAVAIL; return false; }
	if (newPackageSize <= 0)	{ lastError = IO_ERROR_BADPARAM; return false; }

	Flush();

	dataBuffer.Resize(newPackageSize);

	packageSize = newPackageSize;
	stdpacksize = packageSize;

	return true;
}

S::Bool S::IO::OutStream::AddFilter(Filter *newFilter)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }

	newFilter->SetDriver(driver);

	if (newFilter->Activate() == false) { lastError = IO_ERROR_UNKNOWN; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	Flush();

	filters.Add(newFilter);

	allowpackset = true;

	if (newFilter->GetPackageSize() > 0)
	{
		SetPackageSize(newFilter->GetPackageSize());	// package size must be eqv filter size

		allowpackset = false;
	}
	else if (newFilter->GetPackageSize() == -1)
	{
		SetPackageSize(defaultPackageSize);

		allowpackset = false;
	}

	return true;
}

S::Bool S::IO::OutStream::RemoveFilter(Filter *oldFilter)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }

	Int	 index = -1;

	for (Int i = 0; i < filters.Length(); i++) if (filters.GetNth(i) == oldFilter) index = filters.GetNthIndex(i);

	if (index == -1) { lastError = IO_ERROR_BADPARAM; return false; }

	if (pbdActive && !keepPbd) CompletePBD();

	int	 oldcpos = currentBufferPos;

	if (oldFilter->GetPackageSize() > 0 && currentBufferPos != 0)
	{
		for (int i = 0; i < (packageSize - oldcpos); i++)
		{
			OutputNumber(0, 1);
		}
	}
	else if (oldFilter->GetPackageSize() == -1)
	{
		allowpackset = true;

		Flush();
	}
	else
	{
		Flush();
	}

	allowpackset = true;

	oldFilter->Deactivate();

	filters.Remove(index);

	SetPackageSize(origpacksize);

	return true;
}

S::Bool S::IO::OutStream::Close()
{
	if (streamType == STREAM_NONE) { lastError = IO_ERROR_NOTOPEN; return false; }

	while (filters.Length() != 0) RemoveFilter(filters.GetLast());

	Flush();

	if (crosslinked)
	{
		if (closefile) inStream->closefile = true;

		inStream->crosslinked	= false;
		inStream->outStream	= NULL;

		closefile = false;
	}

	if (closefile) delete driver;

	streamType = STREAM_NONE;

	return true;
}

S::Bool S::IO::OutStream::Seek(Int64 position)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }

	Flush();

	driver->Seek(position);

	currentFilePos	 = position;
	currentBufferPos = 0;

	return true;
}

S::Bool S::IO::OutStream::RelSeek(Int64 offset)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }

	Flush();

	driver->Seek(currentFilePos + offset);

	currentFilePos	 += offset;
	currentBufferPos = 0;

	return true;
}
