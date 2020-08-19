 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/io/drivers/driver_win32.h>

#include <smooth/io/instream.h>
#include <smooth/io/outstream.h>

#include <smooth/backends/win32/backendwin32.h>

S::IO::DriverWin32::DriverWin32(const String &file, Int mode) : Driver()
{
	stream	    = INVALID_HANDLE_VALUE;
	closeStream = False;

	/* Build real filename to pass to CreateFile.
	 */
	String	 fileName = String(file.StartsWith("\\\\") ? "" : "\\\\?\\").Append(file);

	switch (mode)
	{
		default:
			lastError = IO_ERROR_BADPARAM;

			return;
		case OS_APPEND:			  // open a file for appending data
			stream = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			if (stream != INVALID_HANDLE_VALUE)
			{
				Int64	 size = GetSize();

				if (size >= 0) Seek(size);
			}

			break;
		case OS_REPLACE:		  // create or overwrite a file
			stream = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			break;
		case IS_READ | IS_WRITE:	  // open a file for reading data
			stream = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			break;
		case IS_READ:			  // open a file in read only mode
			stream = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

			break;
	}

	/* Check if stream was opened successfully.
	 */
	if (stream == INVALID_HANDLE_VALUE)
	{
		if (::GetLastError() == ERROR_ACCESS_DENIED) lastError = IO_ERROR_NOACCESS;
		else					     lastError = IO_ERROR_UNEXPECTED;

		return;
	}

	closeStream = True;
}

S::IO::DriverWin32::DriverWin32(HANDLE iStream) : Driver()
{
	stream	    = iStream;
	closeStream = False;
}

S::IO::DriverWin32::~DriverWin32()
{
	Close();
}

S::Int S::IO::DriverWin32::ReadData(UnsignedByte *data, Int dataSize)
{
	if (stream == INVALID_HANDLE_VALUE || dataSize <= 0) return 0;

	DWORD	 bytes;

	ReadFile(stream, data, dataSize, &bytes, NULL);

	return bytes;
}

S::Int S::IO::DriverWin32::WriteData(const UnsignedByte *data, Int dataSize)
{
	if (stream == INVALID_HANDLE_VALUE || dataSize <= 0) return 0;

	DWORD	 bytes;

	WriteFile(stream, data, dataSize, &bytes, NULL);

	return bytes;
}

S::Int64 S::IO::DriverWin32::Seek(Int64 newPos)
{
	if (stream == INVALID_HANDLE_VALUE) return -1;

	LONG	 hi32 = newPos >> 32;
	DWORD	 lo32 = SetFilePointer(stream, newPos, &hi32, FILE_BEGIN);

	if (lo32 == INVALID_SET_FILE_POINTER && ::GetLastError() != NO_ERROR) return -1;

	return (Int64) hi32 << 32 | lo32;
}

S::Bool S::IO::DriverWin32::Truncate(Int64 newSize)
{
	if (stream == INVALID_HANDLE_VALUE || Seek(newSize) == -1 || !SetEndOfFile(stream)) return False;

	return True;
}

S::Bool S::IO::DriverWin32::Flush()
{
	if (stream == INVALID_HANDLE_VALUE || !FlushFileBuffers(stream)) return False;

	return True;
}

S::Bool S::IO::DriverWin32::Close()
{
	if (stream == INVALID_HANDLE_VALUE || !closeStream || !CloseHandle(stream)) return False;

	stream	    = INVALID_HANDLE_VALUE;
	closeStream = False;

	return True;
}

S::Int64 S::IO::DriverWin32::GetSize() const
{
	if (stream == INVALID_HANDLE_VALUE) return -1;

	DWORD	 hi32 = 0;
	DWORD	 lo32 = GetFileSize(stream, &hi32);

	if (lo32 == INVALID_FILE_SIZE && ::GetLastError() != NO_ERROR) return -1;

	return (Int64) hi32 << 32 | lo32;
}

S::Int64 S::IO::DriverWin32::GetPos() const
{
	if (stream == INVALID_HANDLE_VALUE) return -1;

	LONG	 hi32 = 0;
	DWORD	 lo32 = SetFilePointer(stream, 0, &hi32, FILE_CURRENT);

	if (lo32 == INVALID_SET_FILE_POINTER && ::GetLastError() != NO_ERROR) return -1;

	return (Int64) hi32 << 32 | lo32;
}
