 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
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
#endif

S::IO::DriverPOSIX::DriverPOSIX(const String &file, Int mode) : Driver()
{
	stream	    = -1;
	closeStream = False;

#if defined __WIN32__
	/* Add O_NOINHERIT and O_BINARY options and Unicode prefix on Windows.
	 */
	Int	 options  = O_NOINHERIT | O_BINARY;
	String	 fileName = String(file.StartsWith("\\\\") ? "" : "\\\\?\\").Append(file);
#else
	/* No special options on other systems.
	 */
	Int	 options  = 0;
	String	 fileName = file;

	/* Set output format to UTF-8 on non-Windows systems.
	 */
	String::OutputFormat	 outputFormat("UTF-8");
#endif

	switch (mode)
	{
		default:
			lastError = IO_ERROR_BADPARAM;

			return;
		case OS_APPEND:		   // open a file for appending data
			stream = open(fileName, options | O_RDWR | O_CREAT, 0666);

			if (stream != -1)
			{
				Int64	 size = GetSize();

				if (size >= 0) Seek(size);
			}

			break;
		case OS_REPLACE:	   // create or overwrite a file
			stream = open(fileName, options | O_RDWR | O_CREAT | O_TRUNC, 0666);

			break;
		case IS_READ | IS_WRITE:   // open a file for reading data
			stream = open(fileName, options | O_RDWR);

			break;
		case IS_READ:		   // open a file in read only mode
			stream = open(fileName, options | O_RDONLY);

			break;
	}

	/* Check if stream was opened successfully.
	 */
	if (stream == -1)
	{
		lastError = IO_ERROR_UNEXPECTED;

		return;
	}

	streamID = fileName;

	closeStream = True;
}

S::IO::DriverPOSIX::DriverPOSIX(Int iStream) : Driver()
{
	stream	    = iStream;
	closeStream = False;
}

S::IO::DriverPOSIX::~DriverPOSIX()
{
	Close();
}

S::Int S::IO::DriverPOSIX::ReadData(UnsignedByte *data, Int dataSize)
{
	if (stream == -1 || dataSize <= 0) return 0;

	return read(stream, data, dataSize);
}

S::Int S::IO::DriverPOSIX::WriteData(const UnsignedByte *data, Int dataSize)
{
	if (stream == -1 || dataSize <= 0) return 0;

	return write(stream, data, dataSize);
}

S::Int64 S::IO::DriverPOSIX::Seek(Int64 newPos)
{
	if (stream == -1) return -1;

	return lseek64(stream, newPos, SEEK_SET);
}

S::Bool S::IO::DriverPOSIX::Truncate(Int64 newSize)
{
	if (stream == -1 || ftruncate(stream, newSize) != 0) return False;

	return True;
}

S::Bool S::IO::DriverPOSIX::Flush()
{
	if (stream == -1 || fsync(stream) != 0) return False;

	return True;
}

S::Bool S::IO::DriverPOSIX::Close()
{
	if (stream == -1 || !closeStream || close(stream) != 0) return False;

	stream	    = -1;
	closeStream = False;

	return True;
}

S::Int64 S::IO::DriverPOSIX::GetSize() const
{
	if (stream == -1) return -1;

	Int64	 oldPos = GetPos();
	Int64	 size = lseek64(stream, 0, SEEK_END);

	lseek64(stream, oldPos, SEEK_SET);

	return size;
}

S::Int64 S::IO::DriverPOSIX::GetPos() const
{
	if (stream == -1) return -1;

	return lseek64(stream, 0, SEEK_CUR);
}
