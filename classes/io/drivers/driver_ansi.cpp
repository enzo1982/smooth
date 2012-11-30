 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_ansi.h>

#include <smooth/io/instream.h>
#include <smooth/io/outstream.h>

#include <stdio.h>

#if defined __WIN32__
#	include <windows.h>
#else
#	define _wfopen fopen
#endif

S::IO::DriverANSI::DriverANSI(const String &fileName, Int mode) : Driver()
{
	closeStream = false;

	static Bool	 enableUnicode = Setup::enableUnicode;

#if defined __WIN32__
	/* Disable Unicode functions on Windows 9x even if we
	 * have Unicows as it does not work correctly there.
	 */
	static Bool	 enableUnicodeInitialized = False;

	if (!enableUnicodeInitialized)
	{
		OSVERSIONINFOA	 vInfo;

		vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

		GetVersionExA(&vInfo);

		if (vInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) enableUnicode = False;

		enableUnicodeInitialized = True;
	}
#else
	/* Set output format to UTF-8 on non Windows systems.
	 */
	const char	*previousOutputFormat = String::SetOutputFormat("UTF-8");
#endif

	switch (mode)
	{
		default:
			lastError = IO_ERROR_BADPARAM;

			return;
		case OS_APPEND:		   // open a file for appending data
			if (enableUnicode) stream = _wfopen(fileName, String("r+b"));
			else		   stream =   fopen(fileName,	     "r+b" );

			Seek(GetSize());

			break;
		case OS_REPLACE:	   // create or overwrite a file
			if (enableUnicode) stream = _wfopen(fileName, String("w+b"));
			else		   stream =   fopen(fileName,	     "w+b" );

			break;
		case IS_READ | IS_WRITE:   // open a file for reading data
			if (enableUnicode) stream = _wfopen(fileName, String("r+b"));
			else		   stream =   fopen(fileName,	     "r+b" );

			break;
		case IS_READ:		   // open a file in read only mode
			if (enableUnicode) stream = _wfopen(fileName, String("rb"));
			else		   stream =   fopen(fileName,	     "rb" );

			break;
	}

#if !defined __WIN32__
	/* Restore original output format.
	 */
	String::SetOutputFormat(previousOutputFormat);
#endif

	if (stream == NULL)
	{
		lastError = IO_ERROR_UNEXPECTED;

		return;
	}

	streamID = fileName;

	closeStream = true;
}

S::IO::DriverANSI::DriverANSI(FILE *iStream) : Driver()
{
	stream		= iStream;
	closeStream	= false;
}

S::IO::DriverANSI::~DriverANSI()
{
	if (closeStream) fclose(stream);
}

S::Int S::IO::DriverANSI::ReadData(UnsignedByte *data, Int dataSize)
{
	return fread((Void *) data, 1, (dataSize < (GetSize() - GetPos()) ? dataSize : (GetSize() - GetPos())), stream);
}

S::Int S::IO::DriverANSI::WriteData(UnsignedByte *data, Int dataSize)
{
	return fwrite((Void *) data, 1, dataSize, stream);
}

S::Int64 S::IO::DriverANSI::Seek(Int64 newPos)
{
	return fseek(stream, newPos, SEEK_SET);
}

S::Int64 S::IO::DriverANSI::GetSize() const
{
	Int64	 oldPos = GetPos();

	fseek(stream, 0, SEEK_END);

	Int64	 size = GetPos();

	fseek(stream, oldPos, SEEK_SET);

	return size;
}

S::Int64 S::IO::DriverANSI::GetPos() const
{
	return ftell(stream);
}
