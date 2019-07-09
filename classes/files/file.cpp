 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

/* Define to get 64 bit stat call.
 */
#ifndef __WIN32__
#	define _FILE_OFFSET_BITS 64
#endif

#include <smooth/files/file.h>
#include <smooth/files/directory.h>
#include <smooth/misc/math.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <stdio.h>
#	include <time.h>
#	include <utime.h>
#	include <sys/stat.h>
#endif

namespace smooth
{
#ifdef __WIN32__
	typedef FILETIME FileTime;

	DateTime FileTimeToDateTime(const FileTime &fileTime)
	{
		SYSTEMTIME	 time;

		FileTimeToSystemTime(&fileTime, &time);

		DateTime	 dateTime;

		dateTime.SetYMD(time.wYear, time.wMonth, time.wDay);
		dateTime.SetHMS(time.wHour, time.wMinute, time.wSecond);

		return dateTime;
	}

	FileTime DateTimeToFileTime(const DateTime &dateTime)
	{
		SYSTEMTIME	 time;

		memset(&time, 0, sizeof(time));

		time.wYear   = dateTime.GetYear();
		time.wMonth  = dateTime.GetMonth();
		time.wDay    = dateTime.GetDay();

		time.wHour   = dateTime.GetHour();
		time.wMinute = dateTime.GetMinute();
		time.wSecond = dateTime.GetSecond();

		FileTime	 fileTime;

		SystemTimeToFileTime(&time, &fileTime);

		return fileTime;
	}

	Bool GetFileTime(const File &file, FileTime *cTime, FileTime *aTime, FileTime *wTime)
	{
		if (!file.Exists()) return False;

		HANDLE	 handle = CreateFile(String(Directory::GetUnicodePathPrefix(file)).Append(file), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

		::GetFileTime(handle, cTime, aTime, wTime);

		CloseHandle(handle);

		return True;
	}

	Bool SetFileTime(const File &file, FileTime *cTime, FileTime *aTime, FileTime *wTime)
	{
		if (!file.Exists()) return False;

		HANDLE	 handle = CreateFile(String(Directory::GetUnicodePathPrefix(file)).Append(file), GENERIC_WRITE, 0, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

		::SetFileTime(handle, cTime, aTime, wTime);

		CloseHandle(handle);

		return True;
	}
#else
	typedef time_t FileTime;

	DateTime FileTimeToDateTime(const FileTime &fileTime)
	{
		tm	 time;

		localtime_r(&fileTime, &time);

		DateTime	 dateTime;

		dateTime.SetYMD(time.tm_year, time.tm_mon, time.tm_mday);
		dateTime.SetHMS(time.tm_hour, time.tm_min, time.tm_sec);

		return dateTime;
	}

	FileTime DateTimeToFileTime(const DateTime &dateTime)
	{
		tm	 time;

		memset(&time, 0, sizeof(time));

		time.tm_year  = dateTime.GetYear();
		time.tm_mon   = dateTime.GetMonth();
		time.tm_mday  = dateTime.GetDay();

		time.tm_hour  = dateTime.GetHour();
		time.tm_min   = dateTime.GetMinute();
		time.tm_sec   = dateTime.GetSecond();

		time.tm_isdst = -1;

		return mktime(&time);
	}

	Bool GetFileTime(const File &file, FileTime *cTime, FileTime *aTime, FileTime *wTime)
	{
		if (!file.Exists()) return False;

		struct stat	 info;

		if (stat(String(file).ConvertTo("UTF-8"), &info) != 0) return False;

		if (cTime != NIL) *cTime = info.st_mtime;
		if (aTime != NIL) *aTime = info.st_atime;
		if (wTime != NIL) *wTime = info.st_mtime;

		return True;
	}

	Bool SetFileTime(const File &file, FileTime *cTime, FileTime *aTime, FileTime *wTime)
	{
		if (!file.Exists()) return False;

		struct stat	 info;
		struct utimbuf	 times;

		if (stat(String(file).ConvertTo("UTF-8"), &info) != 0) return False;

		times.actime  = info.st_atime;
		times.modtime = info.st_mtime;

		if (cTime != NIL) times.modtime = *cTime;
		if (aTime != NIL) times.actime  = *aTime;
		if (wTime != NIL) times.modtime = *wTime;

		if (utime(String(file).ConvertTo("UTF-8"), &times) != 0) return False;

		return True;
	}
#endif
};

S::File::File()
{
}

S::File::File(const String &iFileName, const String &iFilePath)
{
	const String	&delimiter = Directory::GetDirectoryDelimiter();

	fileName = iFileName;
	filePath = iFilePath;

	fileName.Replace("/",  delimiter);
	fileName.Replace("\\", delimiter);

	filePath.Replace("/",  delimiter);
	filePath.Replace("\\", delimiter);

	if (fileName != NIL && filePath == NIL)
	{
#ifdef __WIN32__
		if (fileName.StartsWith(delimiter) && !fileName.StartsWith("\\\\")) fileName = String(Directory::GetActiveDirectory()).Head(2).Append(fileName);
#endif

#ifdef __WIN32__
		if (fileName[1] == ':' || fileName.StartsWith("\\\\"))
#else
		if (fileName.StartsWith(delimiter) || fileName.StartsWith("~"))
#endif
		{
			filePath = fileName;
			fileName = NIL;
		}
		else
		{
			filePath = String(Directory::GetActiveDirectory()).Append(delimiter).Append(fileName);
			fileName = NIL;
		}
	}

	if (fileName == NIL)
	{
		Int	 lastBS = filePath.FindLast(delimiter);

		fileName = filePath.Tail(filePath.Length() - lastBS - 1);
		filePath[lastBS >= 0 ? lastBS : 0] = 0;
	}

	/* Replace ./ elements.
	 */
	if (!filePath.EndsWith(delimiter)) filePath.Append(delimiter);

	filePath.Replace(String(delimiter).Append(".").Append(delimiter), delimiter);

	if (filePath.StartsWith(String(".").Append(delimiter))) filePath = String(Directory::GetActiveDirectory()).Append(filePath.Tail(filePath.Length() - 2));

	/* Replace ../ elements.
	 */
	while (filePath.Contains(String(delimiter).Append("..").Append(delimiter)))
	{
		Int	 upPos	= filePath.Find(String(delimiter).Append("..").Append(delimiter));
		Int	 prePos	= filePath.Head(upPos).FindLast(delimiter);

		filePath.Replace(filePath.SubString(prePos, upPos - prePos + 3), String());
	}

	if (filePath.EndsWith(delimiter)) filePath[filePath.Length() - 1] = 0;
}

S::File::File(const File &iFile)
{
	fileName = iFile.fileName;
	filePath = iFile.filePath;
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
	HANDLE	 handle	  = CreateFile(String(Directory::GetUnicodePathPrefix(*this)).Append(*this), GENERIC_READ, FILE_SHARE_READ, NIL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NIL);

	DWORD	 sizeLow  = 0;
	DWORD	 sizeHigh = 0;

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
	FileTime	 fileTime;

	if (!GetFileTime(*this, &fileTime, NIL, NIL)) return DateTime();

	return FileTimeToDateTime(fileTime);
}

S::DateTime S::File::GetAccessTime() const
{
	FileTime	 fileTime;

	if (!GetFileTime(*this, NIL, &fileTime, NIL)) return DateTime();

	return FileTimeToDateTime(fileTime);
}

S::DateTime S::File::GetWriteTime() const
{
	FileTime	 fileTime;

	if (!GetFileTime(*this, NIL, NIL, &fileTime)) return DateTime();

	return FileTimeToDateTime(fileTime);
}

S::Bool S::File::SetAccessTime(const DateTime &dateTime)
{
	FileTime	 fileTime = DateTimeToFileTime(dateTime);

	return SetFileTime(*this, NIL, &fileTime, NIL);
}

S::Bool S::File::SetWriteTime(const DateTime &dateTime)
{
	FileTime	 fileTime = DateTimeToFileTime(dateTime);

	return SetFileTime(*this, NIL, NIL, &fileTime);
}

S::Bool S::File::Exists() const
{
#ifdef __WIN32__
	WIN32_FIND_DATA	 findData;
	HANDLE		 handle = FindFirstFile(String(Directory::GetUnicodePathPrefix(*this)).Append(*this), &findData);

	if (handle == INVALID_HANDLE_VALUE) return False;

	FindClose(handle);

	if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) return False;
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
	HANDLE	 handle = CreateFile(String(Directory::GetUnicodePathPrefix(*this)).Append(*this), GENERIC_READ, FILE_SHARE_READ, NIL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NIL);

	if (handle == INVALID_HANDLE_VALUE) return Error();

	CloseHandle(handle);
#endif

	return Success();
}

S::Int S::File::Copy(const File &destination)
{
	if (!Exists()) return Error();

#ifdef __WIN32__
	Bool	 result = CopyFile(String(Directory::GetUnicodePathPrefix(*this)).Append(*this), String(Directory::GetUnicodePathPrefix(destination)).Append(destination), True);
#else
	Bool	 result = False;

	FILE	*source	= fopen(String(*this).ConvertTo("UTF-8"), "rb");
	FILE	*dest	= fopen(String(destination).ConvertTo("UTF-8"), "wb");

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

S::Int S::File::Move(const File &destination)
{
	if (!Exists()) return Error();

#ifdef __WIN32__
	String		 fileName	= String(Directory::GetUnicodePathPrefix(*this)).Append(*this);
	UnsignedInt	 fileAttributes	= GetFileAttributes(fileName);

	SetFileAttributes(fileName, fileAttributes & ~FILE_ATTRIBUTE_READONLY);

	Bool	 result = MoveFile(fileName, String(Directory::GetUnicodePathPrefix(destination)).Append(destination));

	SetFileAttributes(fileName, fileAttributes);
#else
	Bool	 result = (rename(String(*this).ConvertTo("UTF-8"), String(destination).ConvertTo("UTF-8")) == 0);
#endif

	if (result == False) return Error();
	else		     return Success();
}

S::Int S::File::Delete()
{
	if (!Exists()) return Error();

#ifdef __WIN32__
	String	 fileName = String(Directory::GetUnicodePathPrefix(*this)).Append(*this);

	SetFileAttributes(fileName, GetFileAttributes(fileName) & ~FILE_ATTRIBUTE_READONLY);

	Bool	 result = DeleteFile(fileName);
#else
	Bool	 result = (remove(String(*this).ConvertTo("UTF-8")) == 0);
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
