 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/files/file.h>
#include <smooth/files/directory.h>
#include <smooth/misc/math.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <stdio.h>
#	include <time.h>
#	include <sys/stat.h>
#endif

namespace smooth
{
#ifdef __WIN32__
	Int GetFileTime(const File &file, FILETIME *cTime, FILETIME *aTime, FILETIME *wTime)
#else
	Int GetFileTime(const File &file, time_t *cTime, time_t *aTime, time_t *wTime)
#endif
	{
		if (!file.Exists()) return Error();

#ifdef __WIN32__
		HANDLE	 handle;

		if (Setup::enableUnicode) handle = CreateFileW(String(Directory::GetUnicodePathPrefix(file)).Append(file), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);
		else			  handle = CreateFileA(String(file),						   GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

		::GetFileTime(handle, cTime, aTime, wTime);

		CloseHandle(handle);
#else
		struct stat	 info;

		if (stat(String(file).ConvertTo("UTF-8"), &info) != 0) return Error();

		if (cTime != NIL) *cTime = info.st_ctime;
		if (aTime != NIL) *aTime = info.st_atime;
		if (wTime != NIL) *wTime = info.st_mtime;
#endif

		return Success();
	}
};

S::File::File(const String &iFileName, const String &iFilePath)
{
	fileName = iFileName;
	filePath = iFilePath;

	fileName.Replace("/",  Directory::GetDirectoryDelimiter());
	fileName.Replace("\\", Directory::GetDirectoryDelimiter());

	filePath.Replace("/",  Directory::GetDirectoryDelimiter());
	filePath.Replace("\\", Directory::GetDirectoryDelimiter());

	if (fileName != NIL && filePath == NIL)
	{
#ifdef __WIN32__
		if (fileName.StartsWith(Directory::GetDirectoryDelimiter()) && !fileName.StartsWith("\\\\")) fileName = String(Directory::GetActiveDirectory()).Head(2).Append(fileName);
#endif

#ifdef __WIN32__
		if (fileName[1] == ':' || fileName.StartsWith("\\\\"))
#else
		if (fileName.StartsWith(Directory::GetDirectoryDelimiter()) || fileName.StartsWith("~"))
#endif
		{
			filePath = fileName;
			fileName = NIL;
		}
		else
		{
			filePath = String(Directory::GetActiveDirectory()).Append(Directory::GetDirectoryDelimiter()).Append(fileName);
			fileName = NIL;
		}
	}

	if (fileName == NIL)
	{
		Int	 lastBS = filePath.FindLast(Directory::GetDirectoryDelimiter());

		fileName = filePath.Tail(filePath.Length() - lastBS - 1);
		filePath[lastBS >= 0 ? lastBS : 0] = 0;
	}

	if (!filePath.EndsWith(Directory::GetDirectoryDelimiter())) filePath.Append(Directory::GetDirectoryDelimiter());

	while (filePath.Contains(String(Directory::GetDirectoryDelimiter()).Append("..").Append(Directory::GetDirectoryDelimiter())))
	{
		Int	 upPos	= filePath.Find(String(Directory::GetDirectoryDelimiter()).Append("..").Append(Directory::GetDirectoryDelimiter()));
		Int	 prePos	= filePath.Head(upPos).FindLast(Directory::GetDirectoryDelimiter());

		filePath.Replace(filePath.SubString(prePos, upPos - prePos + 3), String());
	}

	if (filePath.EndsWith(Directory::GetDirectoryDelimiter())) filePath[filePath.Length() - 1] = 0;
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

	if (Setup::enableUnicode) handle = CreateFileW(String(Directory::GetUnicodePathPrefix(*this)).Append(*this), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);
	else			  handle = CreateFileA(String(*this),						     GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

	DWORD	 sizeLow	= 0;
	DWORD	 sizeHigh	= 0;

	sizeLow = ::GetFileSize(handle, &sizeHigh);

	CloseHandle(handle);

	return (Int64(sizeHigh) << 32) + sizeLow;
#else
	struct stat	 info;

	if (stat(String(*this).ConvertTo("UTF-8"), &info) != 0) return 0;

	return info.st_size;
#endif
}

S::DateTime S::File::GetCreationTime() const
{
	DateTime	 dateTime;

#ifdef __WIN32__
	FILETIME	 fileTime;
	SYSTEMTIME	 time;
#else
	time_t		 fileTime;
	tm		*time;
#endif

	if (GetFileTime(*this, &fileTime, NIL, NIL) == Error()) return dateTime;

#ifdef __WIN32__
	FileTimeToSystemTime(&fileTime, &time);

	dateTime.SetYMD(time.wYear, time.wMonth, time.wDay);
	dateTime.SetHMS(time.wHour, time.wMinute, time.wSecond);
#else
	time = localtime(&fileTime);

	dateTime.SetYMD(time->tm_year, time->tm_mon, time->tm_mday);
	dateTime.SetHMS(time->tm_hour, time->tm_min, time->tm_sec);
#endif

	return dateTime;
}

S::DateTime S::File::GetAccessTime() const
{
	DateTime	 dateTime;

#ifdef __WIN32__
	FILETIME	 fileTime;
	SYSTEMTIME	 time;
#else
	time_t		 fileTime;
	tm		*time;
#endif

	if (GetFileTime(*this, NIL, &fileTime, NIL) == Error()) return dateTime;

#ifdef __WIN32__
	FileTimeToSystemTime(&fileTime, &time);

	dateTime.SetYMD(time.wYear, time.wMonth, time.wDay);
	dateTime.SetHMS(time.wHour, time.wMinute, time.wSecond);
#else
	time = localtime(&fileTime);

	dateTime.SetYMD(time->tm_year, time->tm_mon, time->tm_mday);
	dateTime.SetHMS(time->tm_hour, time->tm_min, time->tm_sec);
#endif

	return dateTime;
}

S::DateTime S::File::GetWriteTime() const
{
	DateTime	 dateTime;

#ifdef __WIN32__
	FILETIME	 fileTime;
	SYSTEMTIME	 time;
#else
	time_t		 fileTime;
	tm		*time;
#endif

	if (GetFileTime(*this, NIL, NIL, &fileTime) == Error()) return dateTime;

#ifdef __WIN32__
	FileTimeToSystemTime(&fileTime, &time);

	dateTime.SetYMD(time.wYear, time.wMonth, time.wDay);
	dateTime.SetHMS(time.wHour, time.wMinute, time.wSecond);
#else
	time = localtime(&fileTime);

	dateTime.SetYMD(time->tm_year, time->tm_mon, time->tm_mday);
	dateTime.SetHMS(time->tm_hour, time->tm_min, time->tm_sec);
#endif

	return dateTime;
}

S::Bool S::File::Exists() const
{
#ifdef __WIN32__
	HANDLE		 handle;
	WIN32_FIND_DATAW findDataW;
	WIN32_FIND_DATAA findDataA;

	if (Setup::enableUnicode) handle = FindFirstFileW(String(Directory::GetUnicodePathPrefix(*this)).Append(*this), &findDataW);
	else			  handle = FindFirstFileA(String(*this), &findDataA);

	if (handle == INVALID_HANDLE_VALUE) return False;

	FindClose(handle);

	if (Setup::enableUnicode) { if (findDataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) return False; }
	else			  { if (findDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) return False; }
#else
	struct stat	 info;

	if (stat(String(*this).ConvertTo("UTF-8"), &info) != 0) return False;

	if (!S_ISREG(info.st_mode)) return False;
#endif

	return True;
}

S::Int S::File::Create()
{
	if (Exists()) return Error();

#ifdef __WIN32__
	HANDLE	 handle;

	if (Setup::enableUnicode) handle = CreateFileW(String(Directory::GetUnicodePathPrefix(*this)).Append(*this), GENERIC_READ, FILE_SHARE_READ, NIL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NIL);
	else			  handle = CreateFileA(String(*this),						     GENERIC_READ, FILE_SHARE_READ, NIL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NIL);

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
	if (Setup::enableUnicode) result = CopyFileW(String(Directory::GetUnicodePathPrefix(*this)).Append(*this), String(Directory::GetUnicodePathPrefix(destination)).Append(destination), True);
	else			  result = CopyFileA(String(*this), destination, True);
#else
	FILE	*source	= fopen(String(*this).ConvertTo("UTF-8"), "rb");
	FILE	*dest	= fopen(destination.ConvertTo("UTF-8"), "wb");

	if (source != NIL && dest != NIL)
	{
		Int	 bytesLeft = GetFileSize();
		Int	 chunkSize = 32768;

		UnsignedByte	*buffer = new UnsignedByte [chunkSize];

		while (bytesLeft)
		{
			chunkSize = Math::Min(chunkSize, bytesLeft);

			if (fread(buffer, chunkSize, 1, source) == 1) fwrite(buffer, chunkSize, 1, dest);
			else					      break;

			bytesLeft -= chunkSize;
		}

		delete [] buffer;

		if (!bytesLeft) result = True;
	}

	if (source != NIL) fclose(source);
	if (dest != NIL) fclose(dest);
#endif

	if (result == False) return Error();
	else		     return Success();
}

S::Int S::File::Move(const String &destination)
{
	if (!Exists()) return Error();

	Bool	 result = False;

#ifdef __WIN32__
	if (Setup::enableUnicode) result = MoveFileW(String(Directory::GetUnicodePathPrefix(*this)).Append(*this), String(Directory::GetUnicodePathPrefix(destination)).Append(destination));
	else			  result = MoveFileA(String(*this), destination);
#else
	result = (rename(String(*this).ConvertTo("UTF-8"), destination.ConvertTo("UTF-8")) == 0);
#endif

	if (result == False) return Error();
	else		     return Success();
}

S::Int S::File::Delete()
{
	if (!Exists()) return Error();

	Bool	 result = False;

#ifdef __WIN32__
	if (Setup::enableUnicode) result = DeleteFileW(String(Directory::GetUnicodePathPrefix(*this)).Append(*this));
	else			  result = DeleteFileA(String(*this));
#else
	result = (remove(String(*this).ConvertTo("UTF-8")) == 0);
#endif

	if (result == False) return Error();
	else		     return Success();
}

S::Int S::File::Truncate()
{
	if (!Exists()) return Error();

	if (Delete() == Error()) return Error();

	return Create();
}
