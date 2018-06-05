 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
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
#	include <smooth/backends/win32/backendwin32.h>

#	include <io.h>

#	define open _wopen
#	define close _close

#	define read _read
#	define write _write

#	define lseek64 _lseeki64

#	define ftruncate _chsize
#	define fsync _commit
#else
#	include <unistd.h>

#	if !defined __linux__ && !defined __sun && !defined __GNU__
#		define lseek64 lseek
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
	stream	    = -1;
	closeStream = false;

#if !defined __WIN32__
	/* Set output format to UTF-8 on non-Windows systems.
	 */
	const char	*previousOutputFormat = String::SetOutputFormat("UTF-8");
#endif

	switch (mode)
	{
		default:
			lastError = IO_ERROR_BADPARAM;

			return;
		case OS_APPEND:		   // open a file for appending data
			stream = open(fileName, O_RDWR | O_BINARY | O_RANDOM | O_CREAT, 0666);

			if (stream != -1)
			{
				Int64	 size = GetSize();

				if (size >= 0) Seek(size);
			}

			break;
		case OS_REPLACE:	   // create or overwrite a file
			stream = open(fileName, O_RDWR | O_BINARY | O_RANDOM | O_CREAT | O_TRUNC, 0666);

			break;
		case IS_READ | IS_WRITE:   // open a file for reading data
			stream = open(fileName, O_RDWR | O_BINARY);

			break;
		case IS_READ:		   // open a file in read only mode
			stream = open(fileName, O_RDONLY | O_BINARY);

			break;
	}

#if !defined __WIN32__
	/* Restore original output format.
	 */
	String::SetOutputFormat(previousOutputFormat);
#endif

	/* Check if stream was opened successfully.
	 */
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
	Close();
}

S::Int S::IO::DriverPOSIX::ReadData(UnsignedByte *data, Int dataSize)
{
	if (dataSize <= 0) return 0;

	return read(stream, data, dataSize);
}

S::Int S::IO::DriverPOSIX::WriteData(UnsignedByte *data, Int dataSize)
{
	if (dataSize <= 0) return 0;

	return write(stream, data, dataSize);
}

S::Int64 S::IO::DriverPOSIX::Seek(Int64 newPos)
{
	return lseek64(stream, newPos, SEEK_SET);
}

S::Bool S::IO::DriverPOSIX::Truncate(Int64 newSize)
{
	if (ftruncate(stream, newSize) != 0) return False;

	return True;
}

S::Bool S::IO::DriverPOSIX::Flush()
{
	if (fsync(stream) != 0) return False;

	return True;
}

S::Bool S::IO::DriverPOSIX::Close()
{
	if (!closeStream || close(stream) != 0) return False;

	closeStream = False;

	return True;
}

S::Int64 S::IO::DriverPOSIX::GetSize() const
{
	Int64	 oldPos = GetPos();
	Int64	 size = lseek64(stream, 0, SEEK_END);

	lseek64(stream, oldPos, SEEK_SET);

	return size;
}

S::Int64 S::IO::DriverPOSIX::GetPos() const
{
	return lseek64(stream, 0, SEEK_CUR);
}
