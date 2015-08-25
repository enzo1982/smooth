 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
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
	closefile	 = False;

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

	crosslinked		= True;
	outStream		= out;
	outStream->inStream	= this;
	outStream->crosslinked	= True;

	if (outStream->streamType == STREAM_DRIVER)
	{
		driver		 = outStream->driver;

		streamType	 = STREAM_DRIVER;
		size		 = outStream->size;
		currentFilePos	 = outStream->currentFilePos;
		closefile	 = False;

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
	/* Close stream.
	 */
	Close();
}

S::Bool S::IO::InStream::ReadData()
{
	if (streamType == STREAM_NONE) { lastError = IO_ERROR_NOTOPEN; return False; }

	if (streamType != STREAM_DRIVER) return True;

	Int	 decsize = 0;

	do
	{
		size	       = origsize;
		currentFilePos = origfilepos;

		if (filter == NIL)
		{
			/* Read unfiltered data.
			 */
			packageSize = stdpacksize;

			dataBuffer.Resize(packageSize);

			driver->Seek(currentFilePos);

			if (size != -1)	dataBuffer.Resize(packageSize < size - currentFilePos ? packageSize : size - currentFilePos);
			else		dataBuffer.Resize(packageSize);

			decsize = driver->ReadData(dataBuffer, dataBuffer.Size());
		}
		else
		{
			/* Read filtered data.
			 */
			if (filter->GetPackageSize() > 0) packageSize = filter->GetPackageSize();
			else				  packageSize = stdpacksize;

			dataBuffer.Resize(packageSize);

			decsize = filter->ReadData(dataBuffer);
		}

		/* Update package size and buffer positions.
		 */
		if (packageSize <= size - currentFilePos || filter != NIL || size == -1)
		{
			if (decsize == -1) { packageSize = 0; return False; }

			packageSize = decsize;
			origsize    = size;
			origfilepos = currentFilePos + packageSize;

			if (packageSize + currentFilePos > size && size != -1) size = packageSize + currentFilePos;
		}
		else
		{
			if (decsize <= 0) { packageSize = 0; return False; }
		}

		currentBufferPos = 0;

		/* Bail out if more data is requested, but we do not have any more.
		 */
		if (packageSize == 0 && driver->GetPos() == driver->GetSize()) return False;
	}
	while (packageSize == 0);

	return True;
}

S::Int64 S::IO::InStream::InputNumber(Int bytes)	// Intel byte order DCBA
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return -1; }
	if (bytes > 8 || bytes < 0)	{ lastError = IO_ERROR_BADPARAM; return -1; }

	if (bitstreamActive && !keepBits) CompleteBitstream();

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

	if (bitstreamActive && !keepBits) CompleteBitstream();

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

S::Int64 S::IO::InStream::InputBits(Int bits)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return -1; }
	if (bits > 64 || bits < 0)	{ lastError = IO_ERROR_BADPARAM; return -1; }

	if (!bitstreamActive) InitBitstream();

	Int64	 rval = 0;

	while (bitLength < bits)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IO_ERROR_UNKNOWN; return -1; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData()) { lastError = IO_ERROR_NODATA; return -1; }
		}

		for (Int i = 0; i < 8; i++)
		{
			bitBuffer[bitLength] = IOGetBit(dataBuffer[currentBufferPos], i);
			bitLength++;
		}

		currentBufferPos++;
		currentFilePos++;
	}

	for (Int i = 0; i < bits; i++)
	{
		rval = rval | (bitBuffer[i] << i);
	}

	bitLength = bitLength - bits;

	for (Int i = 0; i < bitLength; i++)
	{
		bitBuffer[i] = bitBuffer[i + bits];
	}

	return rval;
}

S::String S::IO::InStream::InputString(Int bytes)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return NIL; }
	if (bytes <= 0)			{ lastError = IO_ERROR_BADPARAM; return NIL; }

	if (bitstreamActive && !keepBits) CompleteBitstream();

	Int	 bytesleft	= bytes;
	Int	 databufferpos	= 0;

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
		Int	 amount = ((packageSize - currentBufferPos) < (bytesleft)) ? (packageSize - currentBufferPos) : (bytesleft);

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

	if (bitstreamActive && !keepBits) CompleteBitstream();

	Long	 inpval;
	Int	 bytes = 0;

	Buffer<UnsignedByte>	 stringBuffer(1024);

	while (True)
	{
		for (Int i = 0; i < 1024; i++)
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
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return 0; }
	if (pointer == NIL)		{ lastError = IO_ERROR_BADPARAM; return 0; }
	if (bytes < 0)			{ lastError = IO_ERROR_BADPARAM; return 0; }

	if (bitstreamActive && !keepBits) CompleteBitstream();

	Int	 bytesleft	= bytes;
	Int	 databufferpos	= 0;

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
		Int	 amount = ((packageSize - currentBufferPos) < (bytesleft)) ? (packageSize - currentBufferPos) : (bytesleft);

		memcpy((void *) ((unsigned char *) pointer + databufferpos), (void *) (((UnsignedByte *) dataBuffer) + currentBufferPos), amount);

		bytesleft	 -= amount;
		databufferpos	 += amount;
		currentBufferPos += amount;
		currentFilePos	 += amount;
	}

	return bytes;
}

S::Bool S::IO::InStream::InitBitstream()
{
	bitLength	= 0;
	bitstreamActive	= 1;

	return True;
}

S::Bool S::IO::InStream::CompleteBitstream()
{
	bitLength	= 0;
	bitstreamActive	= 0;

	return True;
}

S::Bool S::IO::InStream::SetPackageSize(Int newPackageSize)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return False; }
	if (!allowpackset)		{ lastError = IO_ERROR_OPNOTAVAIL; return False; }
	if (newPackageSize <= 0)	{ lastError = IO_ERROR_BADPARAM; return False; }

	if (bitstreamActive) CompleteBitstream();

	if (filter == NIL) dataBuffer.Resize(newPackageSize);

	packageSize = newPackageSize;
	stdpacksize = packageSize;

	Seek(currentFilePos);

	return True;
}

S::Bool S::IO::InStream::SetFilter(Filter *newFilter)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return False; }
	if (filter != NIL)		{ lastError = IO_ERROR_BADPARAM; return False; }

	newFilter->SetDriver(driver);

	Seek(currentFilePos);

	if (newFilter->Activate() == False) { lastError = IO_ERROR_UNKNOWN; return False; }

	allowpackset = True;

	if	(newFilter->GetPackageSize() >   0) SetPackageSize(newFilter->GetPackageSize());
	else if (newFilter->GetPackageSize() == -1) SetPackageSize(size - currentFilePos);

	allowpackset = False;

	filter = newFilter;

	return True;
}

S::Bool S::IO::InStream::RemoveFilter()
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return False; }
	if (filter == NIL)		{ lastError = IO_ERROR_BADPARAM; return False; }

	filter->Deactivate();
	filter = NIL;

	allowpackset = True;

	SetPackageSize(origpacksize);

	return True;
}

S::Bool S::IO::InStream::Close()
{
	if (streamType == STREAM_NONE) { lastError = IO_ERROR_NOTOPEN; return False; }

	if (bitstreamActive) CompleteBitstream();

	while (filter != NIL) RemoveFilter();

	if (crosslinked)
	{
		if (closefile) outStream->closefile = True;

		outStream->crosslinked	= False;
		outStream->inStream	= NIL;

		closefile = False;
	}

	if (closefile) delete driver;

	streamType = STREAM_NONE;

	return True;
}

S::Bool S::IO::InStream::Seek(Int64 position)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return False; }

	return RelSeek(position - currentFilePos);
}

S::Bool S::IO::InStream::RelSeek(Int64 offset)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return False; }

	if (bitstreamActive) CompleteBitstream();

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

	return True;
}
