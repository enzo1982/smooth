 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#define _FILE_OFFSET_BITS 64

#include <smooth/io/drivers/driver_ansi.h>

#include <smooth/io/instream.h>
#include <smooth/io/outstream.h>

#include <stdio.h>

#if defined __WIN32__
#	include <smooth/backends/win32/backendwin32.h>

#	include <io.h>

#	define fopen _wfopen

#	if !defined __MINGW32__
#		define fseeko _fseeki64
#		define ftello _ftelli64
#	endif

#	define ftruncate chsize
#else
#	include <unistd.h>
#endif

S::IO::DriverANSI::DriverANSI(const String &file, Int mode) : Driver()
{
	stream	    = NIL;
	closeStream = False;

#if defined __WIN32__
	/* Add N mode option and Unicode prefix on Windows.
	 */
	String	 options  = "N";
	String	 fileName = String(file.StartsWith("\\\\") ? "" : "\\\\?\\").Append(file);
#else
	/* Use e mode option on other systems.
	 */
	String	 options  = "e";
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
			stream = fopen(fileName, String("r+b").Append(options));

			if (stream != NIL)
			{
				Int64	 size = GetSize();

				if (size >= 0) Seek(size);
			}

			break;
		case OS_REPLACE:	   // create or overwrite a file
			stream = fopen(fileName, String("w+b").Append(options));

			break;
		case IS_READ | IS_WRITE:   // open a file for reading data
			stream = fopen(fileName, String("r+b").Append(options));

			break;
		case IS_READ:		   // open a file in read only mode
			stream = fopen(fileName, String("rb").Append(options));

			break;
	}

	/* Check if stream was opened successfully.
	 */
	if (stream == NIL)
	{
		lastError = IO_ERROR_UNEXPECTED;

		return;
	}

	streamID = fileName;

	closeStream = True;
}

S::IO::DriverANSI::DriverANSI(FILE *iStream) : Driver()
{
	stream	    = iStream;
	closeStream = False;
}

S::IO::DriverANSI::~DriverANSI()
{
	Close();
}

S::Int S::IO::DriverANSI::ReadData(UnsignedByte *data, Int dataSize)
{
	if (!stream || dataSize <= 0) return 0;

	/* Switch between read and write mode.
	 */
	if (position == -1) fseeko(stream, 0, SEEK_CUR);

	position = 1;

	/* Perform actual read operation.
	 */
	return fread(data, 1, dataSize, stream);
}

S::Int S::IO::DriverANSI::WriteData(const UnsignedByte *data, Int dataSize)
{
	if (!stream || dataSize <= 0) return 0;

	/* Switch between read and write mode.
	 */
	if (position == 1) fseeko(stream, 0, SEEK_CUR);

	position = -1;

	/* Perform actual write operation.
	 */
	return fwrite(data, 1, dataSize, stream);
}

S::Int64 S::IO::DriverANSI::Seek(Int64 newPos)
{
	if (!stream || fseeko(stream, newPos, SEEK_SET) != 0) return -1;

	/* Reset read/write mode indicator.
	 */
	position = 0;

	return GetPos();
}

S::Bool S::IO::DriverANSI::Truncate(Int64 newSize)
{
	if (!stream || fflush(stream) != 0 || ftruncate(fileno(stream), newSize) != 0) return False;

	return True;
}

S::Bool S::IO::DriverANSI::Flush()
{
	if (!stream || fflush(stream) != 0) return False;

	return True;
}

S::Bool S::IO::DriverANSI::Close()
{
	if (!stream || !closeStream || fclose(stream) != 0) return False;

	stream	    = NIL;
	closeStream = False;

	return True;
}

S::Int64 S::IO::DriverANSI::GetSize() const
{
	if (!stream) return -1;

	Int64	 oldPos = GetPos();

	if (fseeko(stream,      0, SEEK_END) != 0) return -1;

	Int64	 size = GetPos();

	if (fseeko(stream, oldPos, SEEK_SET) != 0) return -1;

	return size;
}

S::Int64 S::IO::DriverANSI::GetPos() const
{
	if (!stream) return -1;

	return ftello(stream);
}

S::Bool S::IO::DriverANSI::IsBuffered() const
{
	return True;
}

S::Bool S::IO::DriverANSI::SetBufferSize(Int size)
{
	if (!stream) return False;

	Int	 mode = size > 0 ? _IOFBF : _IONBF;

	if (setvbuf(stream, NULL, mode, size) != 0) return False;

	return True;
}
