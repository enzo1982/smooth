 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
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

#include <smooth/files/file.h>

#include <stdio.h>
#include <stdarg.h>
#include <memory.h>

S::IO::InStream::InStream(Int type, Driver *iDriver)
{
	outStream	 = NIL;

	if (type != STREAM_DRIVER)		   { lastError = IO_ERROR_BADPARAM;	 return; }

	driver		 = iDriver;

	if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); return; }

	streamType	 = STREAM_DRIVER;
	size		 = driver->GetSize();
	closefile	 = false;

	currentBufferPos = defaultPackageSize;
	origsize	 = size;

	dataBuffer.Resize(packageSize);
}

S::IO::InStream::InStream(Int type, const String &fileName, Int mode)
{
	outStream	 = NIL;

	if (type != STREAM_FILE)		   { lastError = IO_ERROR_BADPARAM;			return; }

#ifdef __WIN32__
	driver		 = new DriverWin32(File(fileName), mode);
#else
	driver		 = new DriverPOSIX(File(fileName), mode);
#endif

	if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

	streamType	 = STREAM_DRIVER;
	size		 = driver->GetSize();

	currentBufferPos = defaultPackageSize;
	origsize	 = size;

	dataBuffer.Resize(packageSize);
}

S::IO::InStream::InStream(Int type, FILE *openFile)
{
	outStream	 = NIL;

	if (type != STREAM_ANSI)		   { lastError = IO_ERROR_BADPARAM;			return; }

	driver		 = new DriverANSI(openFile);

	if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

	streamType	 = STREAM_DRIVER;
	size		 = driver->GetSize();
	currentFilePos	 = driver->GetPos();

	packageSize	 = 1;
	stdpacksize	 = packageSize;
	origpacksize	 = packageSize;
	currentBufferPos = defaultPackageSize;
	origsize	 = size;

	dataBuffer.Resize(packageSize);
}

S::IO::InStream::InStream(Int type, Void *inBuffer, Long bufferSize)
{
	outStream	 = NIL;

	if (type != STREAM_BUFFER)		   { lastError = IO_ERROR_BADPARAM;			return; }

	driver		 = new DriverMemory(inBuffer, bufferSize);

	if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

	streamType	 = STREAM_DRIVER;
	size		 = driver->GetSize();

	packageSize	 = 1;
	stdpacksize	 = packageSize;
	origpacksize	 = packageSize;
	currentBufferPos = packageSize;
	origsize	 = size;

	dataBuffer.Resize(packageSize);
}

S::IO::InStream::InStream(Int type, OutStream *out)
{
	outStream	 = NIL;

	if (type != STREAM_STREAM)				{ lastError = IO_ERROR_BADPARAM;   return; }
	if (out->streamType == STREAM_NONE || out->crosslinked)	{ lastError = IO_ERROR_OPNOTAVAIL; return; }

	streamType	 = STREAM_STREAM;

	crosslinked		= true;
	outStream		= out;
	outStream->inStream	= this;
	outStream->crosslinked	= true;

	if (outStream->streamType == STREAM_DRIVER)
	{
		driver		 = outStream->driver;

		streamType	 = STREAM_DRIVER;
		size		 = outStream->size;
		currentFilePos	 = outStream->currentFilePos;
		closefile	 = false;

		packageSize	 = 1;
		stdpacksize	 = packageSize;
		origpacksize	 = packageSize;
		currentBufferPos = packageSize;
		origsize	 = size;

		dataBuffer.Resize(packageSize);
	}
}

S::IO::InStream::~InStream()
{
	Close();	// close stream
}

S::Bool S::IO::InStream::ReadData()
{
	if (streamType == STREAM_NONE) { lastError = IO_ERROR_NOTOPEN; return false; }

	if (streamType != STREAM_DRIVER) return true;

	Int	 decsize = 0;

	size	       = origsize;
	currentFilePos = origfilepos;

	if (filters.Length() == 0)
	{
		/* Read unfiltered data.
		 */
		packageSize = stdpacksize;

		dataBuffer.Resize(packageSize);

		driver->Seek(currentFilePos);

		if (size != -1)	decsize = driver->ReadData(dataBuffer, ((packageSize) < (size - currentFilePos) ? (packageSize) : (size - currentFilePos)));
		else		decsize = driver->ReadData(dataBuffer, packageSize);
	}
	else
	{
		/* Read filtered data.
		 */
		if (filters.GetFirst()->GetPackageSize() > 0) packageSize = filters.GetFirst()->GetPackageSize();
		else					      packageSize = stdpacksize;

		if (size != -1)	decsize = filters.GetFirst()->ReadData(dataBuffer, ((packageSize) < (size - currentFilePos) ? (packageSize) : (size - currentFilePos)));
		else		decsize = filters.GetFirst()->ReadData(dataBuffer, packageSize);
	}

	if (packageSize <= size - currentFilePos || filters.Length() > 0 || size == -1)
	{
		origfilepos = currentFilePos + packageSize;

		if (decsize == -1) { packageSize = 0; return false; }

		packageSize = decsize;
		origsize    = size;

		if (packageSize + currentFilePos > size && size != -1) size = packageSize + currentFilePos;
	}
	else
	{
		if (decsize <= 0) { packageSize = 0; return false; }
	}

	currentBufferPos = 0;

	if (packageSize == 0) return ReadData();

	return true;
}

S::Int64 S::IO::InStream::InputNumber(Int bytes)	// Intel byte order DCBA
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return -1; }
	if (bytes > 8 || bytes < 0)	{ lastError = IO_ERROR_BADPARAM; return -1; }

	if (pbdActive && !keepPbd) CompletePBD();

	Int64	 rval = 0;

	for (Int i = 0; i < bytes; i++)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IO_ERROR_UNKNOWN; return -1; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData()) { lastError = IO_ERROR_NODATA; return -1; }
		}

		rval += dataBuffer[currentBufferPos] * (1 << (i * 8));
		currentBufferPos++;
		currentFilePos++;
	}

	return rval;
}

S::Int64 S::IO::InStream::InputNumberRaw(Int bytes)	// Raw byte order ABCD
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return -1; }
	if (bytes > 8 || bytes < 0)	{ lastError = IO_ERROR_BADPARAM; return -1; }

	if (pbdActive && !keepPbd) CompletePBD();

	Int64	 rval = 0;

	for (Int i = bytes - 1; i >= 0; i--)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IO_ERROR_UNKNOWN; return -1; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData()) { lastError = IO_ERROR_NODATA; return -1; }
		}

		rval += dataBuffer[currentBufferPos] * (1 << (i * 8));
		currentBufferPos++;
		currentFilePos++;
	}

	return rval;
}

S::Int64 S::IO::InStream::InputNumberPDP(Int bytes)	// PDP byte order BADC
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return -1; }
	if (bytes > 4 || bytes < 0)	{ lastError = IO_ERROR_BADPARAM; return -1; }

	if (pbdActive && !keepPbd) CompletePBD();

	Int64	 rval = 0;

	for (Int i = 0; i < 4; i++)
	{
		if (bytes >= (i ^ 1) + 1)
		{
			if (currentFilePos >= (origfilepos + packageSize)) { lastError = IO_ERROR_UNKNOWN; return -1; }

			while (currentBufferPos >= packageSize)
			{
				if (!ReadData()) { lastError = IO_ERROR_NODATA; return -1; }
			}

			rval += (dataBuffer[currentBufferPos] << (((3 - i) ^ 1) * 8)) >> (8 * (4 - bytes));
			currentBufferPos++;
			currentFilePos++;
		}
	}

	return rval;
}

S::Int64 S::IO::InStream::InputNumberPBD(Int bits)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return -1; }
	if (bits > 64 || bits < 0)	{ lastError = IO_ERROR_BADPARAM; return -1; }

	if (!pbdActive) InitPBD();

	Int64	 rval = 0;

	while (pbdLength < bits)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IO_ERROR_UNKNOWN; return -1; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData()) { lastError = IO_ERROR_NODATA; return -1; }
		}

		for (Int i = 0; i < 8; i++)
		{
			pbdBuffer[pbdLength] = IOGetBit(dataBuffer[currentBufferPos], i);
			pbdLength++;
		}

		currentBufferPos++;
		currentFilePos++;
	}

	for (Int i = 0; i < bits; i++)
	{
		rval = rval | (pbdBuffer[i] << i);
	}

	pbdLength = pbdLength - bits;

	for (Int i = 0; i < pbdLength; i++)
	{
		pbdBuffer[i] = pbdBuffer[i + bits];
	}

	return rval;
}

S::String S::IO::InStream::InputString(Int bytes)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return NIL; }
	if (bytes <= 0)			{ lastError = IO_ERROR_BADPARAM; return NIL; }

	if (pbdActive && !keepPbd) CompletePBD();

	int	 bytesleft	= bytes;
	int	 databufferpos	= 0;

	Buffer<UnsignedByte>	 stringBuffer(bytes + 1);

	while (bytesleft)
	{
		if (currentFilePos >= (origfilepos + packageSize))
		{
			{ lastError = IO_ERROR_UNKNOWN; return NIL; }
		}

		while (currentBufferPos >= packageSize)
		{
			/* If no more data is available, set lastError and return NIL.
			 */
			if (!ReadData()) { lastError = IO_ERROR_NODATA; return NIL; }
		}

		/* Get amount of data read and copy to string buffer.
		 */
		int	 amount = ((packageSize - currentBufferPos) < (bytesleft)) ? (packageSize - currentBufferPos) : (bytesleft);

		for (Int i = 0; i < amount; i++) stringBuffer[databufferpos + i] = (((UnsignedByte *) dataBuffer) + currentBufferPos)[i];

		bytesleft	 -= amount;
		databufferpos	 += amount;
		currentBufferPos += amount;
		currentFilePos	 += amount;
	}

	stringBuffer[bytes] = 0;

	return String((char *) (UnsignedByte *) stringBuffer);
}

S::String S::IO::InStream::InputLine()
{
	if (streamType == STREAM_NONE)		{ lastError = IO_ERROR_NOTOPEN; return NIL; }

	if (pbdActive && !keepPbd) CompletePBD();

	long	 inpval;
	int	 bytes = 0;

	Buffer<UnsignedByte>	 stringBuffer(1024);

	while (True)
	{
		for (int i = 0; i < 1024; i++)
		{
			inpval = InputNumber(1);

			if (inpval == -1)
			{
				stringBuffer[bytes] = 0;

				return String((char *) (UnsignedByte *) stringBuffer);
			}

			if (inpval != 13 && inpval != 10)
			{
				stringBuffer[bytes++] = (char) inpval;
			}
			else
			{
				if (inpval == 13) InputNumber(1);

				stringBuffer[bytes] = 0;

				return String((char *) (UnsignedByte *) stringBuffer);
			}
		}

		stringBuffer.Resize(bytes + 1024);
	}

	return NIL; // never reached
}

S::Int S::IO::InStream::InputData(Void *pointer, Int bytes)
{
	if (streamType == STREAM_NONE)		{ lastError = IO_ERROR_NOTOPEN; return 0; }
	if (pointer == NULL)			{ lastError = IO_ERROR_BADPARAM; return 0; }
	if (bytes < 0)				{ lastError = IO_ERROR_BADPARAM; return 0; }

	if (pbdActive && !keepPbd) CompletePBD();

	int	 bytesleft	= bytes;
	int	 databufferpos	= 0;

	while (bytesleft)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IO_ERROR_UNKNOWN; return 0; }

		while (currentBufferPos >= packageSize)
		{
			/* If no more data is available, set lastError and
			 * return the number of bytes actually read.
			 */
			if (!ReadData()) { lastError = IO_ERROR_NODATA; return bytes - bytesleft; }
		}

		/* Get amount of data read and copy to output buffer.
		 */
		int	 amount = ((packageSize - currentBufferPos) < (bytesleft)) ? (packageSize - currentBufferPos) : (bytesleft);

		memcpy((void *) ((unsigned char *) pointer + databufferpos), (void *) (((UnsignedByte *) dataBuffer) + currentBufferPos), amount);

		bytesleft	 -= amount;
		databufferpos	 += amount;
		currentBufferPos += amount;
		currentFilePos	 += amount;
	}

	return bytes;
}

S::Bool S::IO::InStream::InitPBD()
{
	pbdLength	= 0;
	pbdActive	= 1;

	return true;
}

S::Bool S::IO::InStream::CompletePBD()
{
	pbdLength	= 0;
	pbdActive	= 0;

	return true;
}

S::Bool S::IO::InStream::SetPackageSize(Int newPackageSize)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }
	if (!allowpackset)		{ lastError = IO_ERROR_OPNOTAVAIL; return false; }
	if (newPackageSize <= 0)	{ lastError = IO_ERROR_BADPARAM; return false; }

	if (pbdActive) CompletePBD();

	if (filters.Length() == 0) dataBuffer.Resize(newPackageSize);

	packageSize = newPackageSize;
	stdpacksize = packageSize;

	Seek(currentFilePos);

	return true;
}

S::Bool S::IO::InStream::AddFilter(Filter *newFilter)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return False; }

	newFilter->SetDriver(driver);

	Seek(currentFilePos);

	if (newFilter->Activate() == False) { lastError = IO_ERROR_UNKNOWN; return False; }

	allowpackset = True;

	if	(newFilter->GetPackageSize() >   0) SetPackageSize(newFilter->GetPackageSize());
	else if (newFilter->GetPackageSize() == -1) SetPackageSize(size - currentFilePos);

	allowpackset = False;

	filters.Add(newFilter);

	return True;
}

S::Bool S::IO::InStream::RemoveFilter(Filter *oldFilter)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }

	Int	 index = -1;

	for (Int i = 0; i < filters.Length(); i++) if (filters.GetNth(i) == oldFilter) index = filters.GetNthIndex(i);

	if (index == -1) { lastError = IO_ERROR_BADPARAM; return false; }

	oldFilter->Deactivate();

	filters.Remove(index);

	allowpackset = true;

	SetPackageSize(origpacksize);

	return true;
}

S::Bool S::IO::InStream::Close()
{
	if (streamType == STREAM_NONE) { lastError = IO_ERROR_NOTOPEN; return false; }

	if (pbdActive) CompletePBD();

	while (filters.Length() != 0) RemoveFilter(filters.GetLast());

	if (crosslinked)
	{
		if (closefile) outStream->closefile = true;

		outStream->crosslinked	= false;
		outStream->inStream	= NULL;

		closefile = false;
	}

	if (closefile) delete driver;

	streamType = STREAM_NONE;

	return true;
}

S::Bool S::IO::InStream::Seek(Int64 position)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }

	return RelSeek(position - currentFilePos);
}

S::Bool S::IO::InStream::RelSeek(Int64 offset)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }

	if (pbdActive) CompletePBD();

	if (currentBufferPos + offset >= 0 && currentBufferPos + offset <= packageSize)
	{
		currentFilePos	 += offset;
		currentBufferPos += offset;
	}
	else
	{
		driver->Seek(currentFilePos + offset);

		currentFilePos	 += offset;
		currentBufferPos = packageSize;
		origfilepos	 = currentFilePos;
	}

	return true;
}
