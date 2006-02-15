 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

#include <stdio.h>
#include <stdarg.h>

S::IO::InStream::InStream(Int type, Driver *iDriver)
{
	if (type == STREAM_DRIVER)
	{
		driver = iDriver;

		if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentBufferPos= defaultPackageSize;
		origsize	= size;
		data		= new unsigned char [packageSize];
		closefile	= false;

		return;
	}
	else
	{
		{ lastError = IO_ERROR_BADPARAM; return; }
	}
}

S::IO::InStream::InStream(Int type, const String &filename, Int mode)
{
	if (type == STREAM_FILE)
	{
		driver = new DriverPOSIX(filename, mode);

		if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentBufferPos= defaultPackageSize;
		origsize	= size;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IO_ERROR_BADPARAM; return; }
	}
}

S::IO::InStream::InStream(Int type, FILE *openfile)
{
	if (type == STREAM_ANSI)
	{
		driver = new DriverANSI(openfile);

		if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

		streamType	= STREAM_DRIVER;
		size		= driver->GetSize();
		currentFilePos	= driver->GetPos();
		currentBufferPos= defaultPackageSize;
		origsize	= size;
		packageSize	= 1;
		stdpacksize	= packageSize;
		origpacksize	= packageSize;
		data		= new unsigned char [packageSize];

		return;
	}
	else
	{
		{ lastError = IO_ERROR_BADPARAM; return; }
	}
}

S::IO::InStream::InStream(Int type, Void *inbuffer, Long bufsize)
{
	if (type == STREAM_BUFFER)
	{
		driver = new DriverMemory(inbuffer, bufsize);

		if (driver->GetLastError() != IO_ERROR_OK) { lastError = driver->GetLastError(); delete driver; return; }

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
		{ lastError = IO_ERROR_BADPARAM; return; }
	}
}

S::IO::InStream::InStream(Int type, OutStream *out)
{
	if (type != STREAM_STREAM)				{ lastError = IO_ERROR_BADPARAM; return; }
	if (out->streamType == STREAM_NONE || out->crosslinked)	{ lastError = IO_ERROR_OPNOTAVAIL; return; }

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

S::IO::InStream::~InStream()
{
	Close();	// close stream
}

S::Bool S::IO::InStream::ReadData()
{
	if (streamType == STREAM_NONE) { lastError = IO_ERROR_NOTOPEN; return false; }

	int decsize = 0;

	if (streamType == STREAM_DRIVER)
	{
		if (filters.GetNOfEntries() > 0)
		{
			if (filters.GetFirstEntry()->GetPackageSize() > 0)	packageSize = filters.GetFirstEntry()->GetPackageSize();
			else							packageSize = stdpacksize;
		}
		else	packageSize = stdpacksize;

		size		= origsize;
		currentFilePos	= origfilepos;

		if (filters.GetNOfEntries() == 0)
		{
			delete [] data;

			data = new unsigned char [packageSize];

			driver->Seek(currentFilePos);

			if (size != -1)	decsize = driver->ReadData(data, ((packageSize)<(size-currentFilePos)?(packageSize):(size-currentFilePos)));
			else		decsize = driver->ReadData(data, packageSize);
		}
		else
		{
			if (size != -1)	decsize = filters.GetFirstEntry()->ReadData(&data, ((packageSize)<(size-currentFilePos)?(packageSize):(size-currentFilePos)));
			else		decsize = filters.GetFirstEntry()->ReadData(&data, packageSize);
		}

		if (packageSize <= size-currentFilePos || filters.GetNOfEntries() > 0 || size == -1)
		{
			origfilepos = currentFilePos + packageSize;

			if (decsize == -1)
			{
				packageSize = 0;

				return false;
			}

			packageSize	= decsize;
			origsize	= size;

			if (packageSize + currentFilePos > size && size != -1) size = packageSize + currentFilePos;
		}

		currentBufferPos = 0;

		if (packageSize == 0) return ReadData();
	}

	return true;
}

S::Long S::IO::InStream::InputNumber(Int bytes)	// Intel byte order DCBA
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return -1; }
	if (bytes > 4 || bytes < 0)	{ lastError = IO_ERROR_BADPARAM; return -1; }

	if (pbdActive && !keepPbd) CompletePBD();

	long	 rval = 0;

	for (int i = 0; i < bytes; i++)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IO_ERROR_UNKNOWN; return -1; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData())	{ lastError = IO_ERROR_UNKNOWN; return -1; }
			if (packageSize == 0)	{ lastError = IO_ERROR_NODATA; return -1; }
		}

		rval += data[currentBufferPos] * (1 << (i * 8));
		currentBufferPos++;
		currentFilePos++;
	}

	return rval;
}

S::Long S::IO::InStream::InputNumberRaw(Int bytes)	// Raw byte order ABCD
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return -1; }
	if (bytes > 4 || bytes < 0)	{ lastError = IO_ERROR_BADPARAM; return -1; }

	if (pbdActive && !keepPbd) CompletePBD();

	long	 rval = 0;

	for (int i = bytes - 1; i >= 0; i--)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IO_ERROR_UNKNOWN; return -1; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData())	{ lastError = IO_ERROR_UNKNOWN; return -1; }
			if (packageSize == 0)	{ lastError = IO_ERROR_NODATA; return -1; }
		}

		rval += data[currentBufferPos] * (1 << (i * 8));
		currentBufferPos++;
		currentFilePos++;
	}

	return rval;
}

S::Long S::IO::InStream::InputNumberPDP(Int bytes)	// PDP byte order BADC
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return -1; }
	if (bytes > 4 || bytes < 0)	{ lastError = IO_ERROR_BADPARAM; return -1; }

	if (pbdActive && !keepPbd) CompletePBD();

	long	 rval = 0;

	for (int i = 0; i < 4; i++)
	{
		if (bytes >= (i ^ 1) + 1)
		{
			if (currentFilePos >= (origfilepos + packageSize)) { lastError = IO_ERROR_UNKNOWN; return -1; }

			while (currentBufferPos >= packageSize)
			{
				if (!ReadData())	{ lastError = IO_ERROR_UNKNOWN; return -1; }
				if (packageSize == 0)	{ lastError = IO_ERROR_NODATA; return -1; }
			}

			rval += (data[currentBufferPos] << (((3 - i) ^ 1) * 8)) >> (8 * (4 - bytes));
			currentBufferPos++;
			currentFilePos++;
		}
	}

	return rval;
}

S::Long S::IO::InStream::InputNumberPBD(Int bits)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return -1; }
	if (bits > 32 || bits < 0)	{ lastError = IO_ERROR_BADPARAM; return -1; }

	if (!pbdActive) InitPBD();

	unsigned char	 inp;
	long		 rval = 0;

	while (pbdLength < bits)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IO_ERROR_UNKNOWN; return -1; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData())	{ lastError = IO_ERROR_UNKNOWN; return -1; }
			if (packageSize == 0)	{ lastError = IO_ERROR_NODATA; return -1; }
		}

		inp = data[currentBufferPos];
		currentBufferPos++;
		currentFilePos++;

		for (int i = 0; i < 8; i++)
		{
			pbdBuffer[pbdLength] = IOGetBit(inp, i);
			pbdLength++;
		}
	}

	for (int i = 0; i < bits; i++)
	{
		rval = rval | (pbdBuffer[i] << i);
	}

	pbdLength = pbdLength - bits;

	for (int j = 0; j < pbdLength; j++)
	{
		pbdBuffer[j] = pbdBuffer[j + bits];
	}

	return rval;
}

S::String S::IO::InStream::InputString(Int bytes)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return NIL; }
	if (bytes <= 0)			{ lastError = IO_ERROR_BADPARAM; return NIL; }

	if (pbdActive && !keepPbd) CompletePBD();

	int	 bytesleft = bytes;
	int	 databufferpos = 0;
	int	 amount = 0;
	char	*buffer = new char [bytes + 1];
	String	 rval;

	while (bytesleft)
	{
		if (currentFilePos >= (origfilepos + packageSize))
		{
			{ lastError = IO_ERROR_UNKNOWN; delete [] buffer; return NIL; }
		}

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData())	{ lastError = IO_ERROR_UNKNOWN; delete [] buffer; return NIL; }
			if (packageSize == 0)	{ lastError = IO_ERROR_NODATA; delete [] buffer; return NIL; }
		}

		amount = ((packageSize - currentBufferPos)<(bytesleft))?(packageSize - currentBufferPos):(bytesleft);

		for (Int i = 0; i < amount; i++) buffer[databufferpos + i] = ((char *) (data + currentBufferPos))[i];

		bytesleft -= amount;
		databufferpos += amount;
		currentBufferPos += amount;
		currentFilePos += amount;
	}

	buffer[bytes] = 0;

	rval = buffer;

	delete [] buffer;

	return rval;
}

S::String S::IO::InStream::InputLine()
{
	if (streamType == STREAM_NONE)		{ lastError = IO_ERROR_NOTOPEN; return NIL; }

	if (pbdActive && !keepPbd) CompletePBD();

	long	 inpval;
	int	 bytes = 0;
	String	 rval;
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

				rval = linebuffer1;

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

				rval = linebuffer1;

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

	return NIL; // never reached
}

S::Void *S::IO::InStream::InputData(Void *pointer, Int bytes)
{
	if (streamType == STREAM_NONE)		{ lastError = IO_ERROR_NOTOPEN; return NULL; }
	if (pointer == NULL)			{ lastError = IO_ERROR_BADPARAM; return NULL; }
	if (bytes < 0)				{ lastError = IO_ERROR_BADPARAM; return NULL; }

	if (pbdActive && !keepPbd) CompletePBD();

	int	 bytesleft = bytes;
	int	 databufferpos = 0;
	int	 amount = 0;

	while (bytesleft)
	{
		if (currentFilePos >= (origfilepos + packageSize)) { lastError = IO_ERROR_UNKNOWN; return NULL; }

		while (currentBufferPos >= packageSize)
		{
			if (!ReadData())	{ lastError = IO_ERROR_UNKNOWN; return NULL; }
			if (packageSize == 0)	{ lastError = IO_ERROR_NODATA; return NULL; }
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

S::Bool S::IO::InStream::SetPackageSize(Int newPackagesize)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }
	if (!allowpackset)		{ lastError = IO_ERROR_OPNOTAVAIL; return false; }
	if (newPackagesize <= 0)	{ lastError = IO_ERROR_BADPARAM; return false; }

	if (pbdActive) CompletePBD();

	if (filters.GetNOfEntries() == 0)
	{
		delete [] data;

		data = new unsigned char [newPackagesize];
	}

	packageSize = newPackagesize;
	stdpacksize = packageSize;

	Seek(currentFilePos);

	return true;
}

S::Bool S::IO::InStream::AddFilter(Filter *newFilter)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }

	newFilter->SetDriver(driver);

	Seek(currentFilePos);

	if (newFilter->Activate() == false) { lastError = IO_ERROR_UNKNOWN; return false; }

	allowpackset = true;

	if (newFilter->GetPackageSize() > 0)		SetPackageSize(newFilter->GetPackageSize());
	else if (newFilter->GetPackageSize() == -1)	SetPackageSize(size - currentFilePos);

	allowpackset = false;

	delete [] data;

	data = NULL;

	filters.AddEntry(newFilter);

	while (currentBufferPos >= packageSize)
	{
		if (!ReadData()) { lastError = IO_ERROR_UNKNOWN; return false; }
	}

	return true;
}

S::Bool S::IO::InStream::RemoveFilter(Filter *oldFilter)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }

	Int	 index = -1;

	for (Int i = 0; i < filters.GetNOfEntries(); i++) if (filters.GetNthEntry(i) == oldFilter) index = filters.GetNthEntryIndex(i);

	if (index == -1) { lastError = IO_ERROR_BADPARAM; return false; }

	oldFilter->Deactivate();

	filters.RemoveEntry(index);

	allowpackset = true;

	data = new unsigned char [origpacksize];

	SetPackageSize(origpacksize);

	return true;
}

S::Bool S::IO::InStream::Close()
{
	if (streamType == STREAM_NONE) { lastError = IO_ERROR_NOTOPEN; return false; }

	if (pbdActive) CompletePBD();

	while (filters.GetNOfEntries() != 0) RemoveFilter(filters.GetLastEntry());

	if (crosslinked)
	{
		if (closefile) outStream->closefile = true;

		outStream->crosslinked	= false;
		outStream->inStream	= NULL;

		closefile = false;
	}

	if (closefile) delete driver;

	delete [] data;
	data = NULL;

	streamType = STREAM_NONE;

	return true;
}

S::Bool S::IO::InStream::Seek(Int64 position)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }

	if (pbdActive) CompletePBD();

	driver->Seek(position);

	currentFilePos		= position;
	currentBufferPos	= packageSize;
	origfilepos		= position;

	return true;
}

S::Bool S::IO::InStream::RelSeek(Int64 offset)
{
	if (streamType == STREAM_NONE)	{ lastError = IO_ERROR_NOTOPEN; return false; }

	if (pbdActive) CompletePBD();

	driver->Seek(currentFilePos + offset);

	currentFilePos		+= offset;
	currentBufferPos	= packageSize;
	origfilepos		= currentFilePos;

	return true;
}
