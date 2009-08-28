 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/files/file.h>
#include <smooth/files/directory.h>

#ifndef __WIN32__
#	include <stdio.h>
#	include <sys/stat.h>
#endif

S::File::File(const String &iFileName, const String &iFilePath)
{
	fileName = iFileName;
	filePath = iFilePath;

	if (fileName != NIL && filePath == NIL)
	{
#ifdef __WIN32__
		if (fileName[1] == ':' || fileName.StartsWith("\\\\"))
#else
		if (fileName.StartsWith(Directory::GetDirectoryDelimiter()) || fileName.StartsWith("~"))
#endif
		{
			filePath = fileName;
			fileName = NIL;
		}
	}

	if (fileName == NIL)
	{
		for (Int lastBS = filePath.Length() - 1; lastBS >= 0; lastBS--)
		{
			if (filePath[lastBS] == Directory::GetDirectoryDelimiter()[0])
			{
				for (Int i = lastBS + 1; i < filePath.Length(); i++) fileName[i - lastBS - 1] = filePath[i];

				filePath[lastBS] = 0;

				break;
			}
		}
	}
	else if (filePath == NIL)
	{
		filePath = Directory::GetActiveDirectory();
	}
}

S::File::File(const File &iFile)
{
	fileName = iFile.fileName;
	filePath = iFile.filePath;
}

S::File::File(const int nil)
{
}

S::File::~File()
{
}

S::File::operator S::String() const
{
	return String(filePath).Append(Directory::GetDirectoryDelimiter()).Append(fileName);
}

const S::String &S::File::GetFileName() const
{
	return fileName;
}

const S::String &S::File::GetFilePath() const
{
	return filePath;
}

S::Int64 S::File::GetFileSize() const
{
	if (!Exists()) return -1;

#ifdef __WIN32__
	HANDLE	 handle;

	if (Setup::enableUnicode)	handle = CreateFileW(String("\\\\?\\").Append(*this), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);
	else				handle = CreateFileA(String(*this),		      GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

	DWORD	 sizeLow	= 0;
	DWORD	 sizeHigh	= 0;

	sizeLow = ::GetFileSize(handle, &sizeHigh);

	CloseHandle(handle);

	return (Int64(sizeHigh) << 32) + sizeLow;
#else
	return 0;
#endif
}

#ifdef __WIN32__
	S::Int S::File::GetFileTime(FILETIME *cT, FILETIME *aT, FILETIME *wT) const
	{
		if (!Exists()) return Error();

		HANDLE	 handle;

		if (Setup::enableUnicode)	handle = CreateFileW(String("\\\\?\\").Append(*this), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);
		else				handle = CreateFileA(String(*this),		      GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

		::GetFileTime(handle, cT, aT, wT);

		CloseHandle(handle);

		return Success();
	}
#endif

S::DateTime S::File::GetCreationTime() const
{
	DateTime	 dateTime;

#ifdef __WIN32__
	FILETIME	 fileTime;
	SYSTEMTIME	 time;

	if (GetFileTime(&fileTime, NIL, NIL) == Error()) return dateTime;

	FileTimeToSystemTime(&fileTime, &time);

	dateTime.SetYMD(time.wYear, time.wMonth, time.wDay);
	dateTime.SetHMS(time.wHour, time.wMinute, time.wSecond);
#endif

	return dateTime;
}

S::DateTime S::File::GetAccessTime() const
{
	DateTime	 dateTime;

#ifdef __WIN32__
	FILETIME	 fileTime;
	SYSTEMTIME	 time;

	if (GetFileTime(NIL, &fileTime, NIL) == Error()) return dateTime;

	FileTimeToSystemTime(&fileTime, &time);

	dateTime.SetYMD(time.wYear, time.wMonth, time.wDay);
	dateTime.SetHMS(time.wHour, time.wMinute, time.wSecond);
#endif

	return dateTime;
}

S::DateTime S::File::GetWriteTime() const
{
	DateTime	 dateTime;

#ifdef __WIN32__
	FILETIME	 fileTime;
	SYSTEMTIME	 time;

	if (GetFileTime(NIL, NIL, &fileTime) == Error()) return dateTime;

	FileTimeToSystemTime(&fileTime, &time);

	dateTime.SetYMD(time.wYear, time.wMonth, time.wDay);
	dateTime.SetHMS(time.wHour, time.wMinute, time.wSecond);
#endif

	return dateTime;
}

S::Bool S::File::Exists() const
{
#ifdef __WIN32__
	HANDLE	 handle;

	if (Setup::enableUnicode)	handle = CreateFileW(String("\\\\?\\").Append(*this), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);
	else				handle = CreateFileA(String(*this),		      GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

	if (handle == INVALID_HANDLE_VALUE) return False;

	CloseHandle(handle);
#else
	struct stat	 info;

	if (stat(String(*this), &info) != 0) return False;

	if (!S_ISREG(info.st_mode)) return False;
#endif

	return True;
}

S::Int S::File::Create()
{
	if (Exists()) return Error();

#ifdef __WIN32__
	HANDLE	 handle;

	if (Setup::enableUnicode)	handle = CreateFileW(String("\\\\?\\").Append(*this), GENERIC_READ, FILE_SHARE_READ, NIL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NIL);
	else				handle = CreateFileA(String(*this),		      GENERIC_READ, FILE_SHARE_READ, NIL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NIL);

	if (handle == INVALID_HANDLE_VALUE) return Error();

	CloseHandle(handle);
#endif

	return Success();
}

S::Int S::File::Copy(const String &destination)
{
	if (!Exists()) return Error();
	
	Bool	 result = False;

#ifdef __WIN32__
	if (Setup::enableUnicode)	result = CopyFileW(String("\\\\?\\").Append(*this), String("\\\\?\\").Append(destination), True);
	else				result = CopyFileA(String(*this), destination, True);
#endif

	if (result == False)	return Error();
	else			return Success();
}

S::Int S::File::Move(const String &destination)
{
	if (!Exists()) return Error();

	Bool	 result = False;

#ifdef __WIN32__
	if (Setup::enableUnicode)	result = MoveFileW(String("\\\\?\\").Append(*this), String("\\\\?\\").Append(destination));
	else				result = MoveFileA(String(*this), destination);
#else
	result = (rename(String(*this), destination) == 0);
#endif

	if (result == False)	return Error();
	else			return Success();
}

S::Int S::File::Delete()
{
	if (!Exists()) return Error();

	Bool	 result = False;

#ifdef __WIN32__
	if (Setup::enableUnicode)	result = DeleteFileW(String("\\\\?\\").Append(*this));
	else				result = DeleteFileA(String(*this));
#else
	result = (remove(String(*this)) == 0);
#endif

	if (result == False)	return Error();
	else			return Success();
}

S::Int S::File::Truncate()
{
	if (!Exists()) return Error();

	if (Delete() == Error()) return Error();

	return Create();
}
