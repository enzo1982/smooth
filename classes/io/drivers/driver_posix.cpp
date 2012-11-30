 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_posix.h>

#include <smooth/io/instream.h>
#include <smooth/io/outstream.h>

#include <stdio.h>
#include <fcntl.h>

#if defined __WIN32__
#	include <windows.h>
#	include <io.h>
#else
#	include <unistd.h>

#	define _open open
#	define _wopen open
#	define _close close
#	define _read read
#	define _write write

#	ifdef __linux__
#		define _lseeki64 lseek64
#	else
#		define _lseeki64 lseek
#	endif

#	ifndef O_BINARY
#		define O_BINARY	0
#	endif

#	ifndef O_RANDOM
#		define O_RANDOM	0
#	endif
#endif

S::IO::DriverPOSIX::DriverPOSIX(const String &fileName, Int mode) : Driver()
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
			if (enableUnicode) stream = _wopen(fileName, O_RDWR | O_BINARY | O_RANDOM | O_CREAT, 0600);
			else		   stream =   open(fileName, O_RDWR | O_BINARY | O_RANDOM | O_CREAT, 0600);

			Seek(GetSize());

			break;
		case OS_REPLACE:	   // create or overwrite a file
			if (enableUnicode) stream = _wopen(fileName, O_RDWR | O_BINARY | O_RANDOM | O_CREAT | O_TRUNC, 0600);
			else		   stream =   open(fileName, O_RDWR | O_BINARY | O_RANDOM | O_CREAT | O_TRUNC, 0600);

			break;
		case IS_READ | IS_WRITE:   // open a file for reading data
			if (enableUnicode) stream = _wopen(fileName, O_RDWR | O_BINARY);
			else		   stream =   open(fileName, O_RDWR | O_BINARY);

			break;
		case IS_READ:		   // open a file in read only mode
			if (enableUnicode) stream = _wopen(fileName, O_RDONLY | O_BINARY);
			else		   stream =   open(fileName, O_RDONLY | O_BINARY);

			break;
	}

#if !defined __WIN32__
	/* Restore original output format.
	 */
	String::SetOutputFormat(previousOutputFormat);
#endif

	if (stream == -1)
	{
		lastError = IO_ERROR_UNEXPECTED;

		return;
	}

	streamID = fileName;

	closeStream = true;
}

S::IO::DriverPOSIX::DriverPOSIX(Int iStream) : Driver()
{
	stream		= iStream;
	closeStream	= false;
}

S::IO::DriverPOSIX::~DriverPOSIX()
{
	if (closeStream) _close(stream);
}

S::Int S::IO::DriverPOSIX::ReadData(UnsignedByte *data, Int dataSize)
{
	return _read(stream, data, (dataSize < (GetSize() - GetPos()) ? dataSize : (GetSize() - GetPos())));
}

S::Int S::IO::DriverPOSIX::WriteData(UnsignedByte *data, Int dataSize)
{
	return _write(stream, data, dataSize);
}

S::Int64 S::IO::DriverPOSIX::Seek(Int64 newPos)
{
	return _lseeki64(stream, newPos, SEEK_SET);
}

S::Int64 S::IO::DriverPOSIX::GetSize() const
{
	Int64	 oldPos = GetPos();
	Int64	 size = _lseeki64(stream, 0, SEEK_END);

	_lseeki64(stream, oldPos, SEEK_SET);

	return size;
}

S::Int64 S::IO::DriverPOSIX::GetPos() const
{
	return _lseeki64(stream, 0, SEEK_CUR);
}
