 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/files/file.h>
#include <smooth/files/directory.h>

S::File::File(String iFileName, String iFilePath)
{
	fileName = iFileName;
	filePath = iFilePath;

	if (fileName != NIL && filePath == NIL)
	{
		if (fileName[1] == ':')
		{
			filePath = fileName;
			fileName = NIL;
		}
	}

	if (fileName == NIL)
	{
		for (Int lastBS = filePath.Length() - 1; lastBS >= 0; lastBS--)
		{
			if (filePath[lastBS] == '\\')
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
	return String(filePath).Append("\\").Append(fileName);
}

S::String S::File::GetFileName()
{
	return fileName;
}

S::String S::File::GetFilePath()
{
	return filePath;
}

S::Int64 S::File::GetFileSize()
{
	if (!Exists()) return -1;

	HANDLE	 handle;

	if (Setup::enableUnicode)	handle = CreateFileW(String(filePath).Append("\\").Append(fileName), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);
	else				handle = CreateFileA(String(filePath).Append("\\").Append(fileName), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

	UnsignedInt32	 sizeLow	= 0;
	UnsignedInt32	 sizeHigh	= 0;

	sizeLow = ::GetFileSize(handle, &sizeHigh);

	CloseHandle(handle);

	return (Int64(sizeHigh) << 32) + sizeLow;
}

S::Int S::File::GetFileTime(FILETIME *cT, FILETIME *aT, FILETIME *wT)
{
	if (!Exists()) return Error();

	HANDLE	 handle;

	if (Setup::enableUnicode)	handle = CreateFileW(String(filePath).Append("\\").Append(fileName), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);
	else				handle = CreateFileA(String(filePath).Append("\\").Append(fileName), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

	::GetFileTime(handle, cT, aT, wT);

	CloseHandle(handle);

	return Success();
}

S::DateTime S::File::GetCreationTime()
{
	FILETIME	 fileTime;
	SYSTEMTIME	 time;

	DateTime	 dateTime;

	if (GetFileTime(&fileTime, NIL, NIL) == Error()) return dateTime;

	FileTimeToSystemTime(&fileTime, &time);

	dateTime.SetYMD(time.wYear, time.wMonth, time.wDay);
	dateTime.SetHMS(time.wHour, time.wMinute, time.wSecond);

	return dateTime;
}

S::DateTime S::File::GetAccessTime()
{
	FILETIME	 fileTime;
	SYSTEMTIME	 time;

	DateTime	 dateTime;

	if (GetFileTime(NIL, &fileTime, NIL) == Error()) return dateTime;

	FileTimeToSystemTime(&fileTime, &time);

	dateTime.SetYMD(time.wYear, time.wMonth, time.wDay);
	dateTime.SetHMS(time.wHour, time.wMinute, time.wSecond);

	return dateTime;
}

S::DateTime S::File::GetWriteTime()
{
	FILETIME	 fileTime;
	SYSTEMTIME	 time;

	DateTime	 dateTime;

	if (GetFileTime(NIL, NIL, &fileTime) == Error()) return dateTime;

	FileTimeToSystemTime(&fileTime, &time);

	dateTime.SetYMD(time.wYear, time.wMonth, time.wDay);
	dateTime.SetHMS(time.wHour, time.wMinute, time.wSecond);

	return dateTime;
}

S::Bool S::File::Exists()
{
	HANDLE	 handle;

	if (Setup::enableUnicode)	handle = CreateFileW(String(filePath).Append("\\").Append(fileName), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);
	else				handle = CreateFileA(String(filePath).Append("\\").Append(fileName), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

	if (handle == INVALID_HANDLE_VALUE) return False;

	CloseHandle(handle);

	return True;
}

S::Int S::File::Create()
{
	if (Exists()) return Error();

	HANDLE	 handle;

	if (Setup::enableUnicode)	handle = CreateFileW(String(filePath).Append("\\").Append(fileName), GENERIC_READ, FILE_SHARE_READ, NIL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NIL);
	else				handle = CreateFileA(String(filePath).Append("\\").Append(fileName), GENERIC_READ, FILE_SHARE_READ, NIL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NIL);

	if (handle == INVALID_HANDLE_VALUE) return Error();

	CloseHandle(handle);

	return Success();
}

S::Int S::File::Copy(String destination)
{
	if (!Exists()) return Error();
	
	Bool	 result = False;

	if (Setup::enableUnicode)	result = CopyFileW(String(filePath).Append("\\").Append(fileName), destination, True);
	else				result = CopyFileA(String(filePath).Append("\\").Append(fileName), destination, True);

	if (result == False)	return Error();
	else			return Success();
}

S::Int S::File::Move(String destination)
{
	if (!Exists()) return Error();
	
	Bool	 result = False;

	if (Setup::enableUnicode)	result = MoveFileW(String(filePath).Append("\\").Append(fileName), destination);
	else				result = MoveFileA(String(filePath).Append("\\").Append(fileName), destination);

	if (result == False)	return Error();
	else			return Success();
}

S::Int S::File::Delete()
{
	if (!Exists()) return Error();

	Bool	 result = False;

	if (Setup::enableUnicode)	result = DeleteFileW(String(filePath).Append("\\").Append(fileName));
	else				result = DeleteFileA(String(filePath).Append("\\").Append(fileName));

	if (result == False)	return Error();
	else			return Success();
}

S::Int S::File::Truncate()
{
	if (!Exists()) return Error();

	if (Delete() == Error()) return Error();

	return Create();
}
