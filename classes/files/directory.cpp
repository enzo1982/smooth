 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
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

#include <smooth/files/directory.h>

#ifdef __WIN32__
#	include <smooth/backends/win32/backendwin32.h>

#	include <windows.h>
#else
#	include <glob.h>
#	include <unistd.h>
#	include <stdio.h>
#	include <limits.h>
#	include <sys/stat.h>

#	ifndef PATH_MAX
#		define PATH_MAX 32768
#	endif

#	ifndef GLOB_ONLYDIR
#		define GLOB_ONLYDIR 0
#	endif
#endif

char	*S::Directory::directoryDelimiter = NIL;
char	*S::Directory::unicodePathPrefix  = NIL;

S::Directory::Directory(const String &iDirName, const String &iDirPath)
{
	dirName = iDirName;
	dirPath = iDirPath;

	dirName.Replace("/",  Directory::GetDirectoryDelimiter());
	dirName.Replace("\\", Directory::GetDirectoryDelimiter());

	dirPath.Replace("/",  Directory::GetDirectoryDelimiter());
	dirPath.Replace("\\", Directory::GetDirectoryDelimiter());

	if (dirName != NIL && dirPath == NIL)
	{
#ifdef __WIN32__
		if (dirName.StartsWith(Directory::GetDirectoryDelimiter()) && !dirName.StartsWith("\\\\")) dirName = String(Directory::GetActiveDirectory()).Head(2).Append(dirName);
#endif

#ifdef __WIN32__
		if (dirName[1] == ':' || dirName.StartsWith("\\\\"))
#else
		if (dirName.StartsWith(GetDirectoryDelimiter()) || dirName.StartsWith("~"))
#endif
		{
			dirPath = dirName;
			dirName = NIL;
		}
		else
		{
			dirPath = String(Directory::GetActiveDirectory()).Append(Directory::GetDirectoryDelimiter()).Append(dirName);
			dirName = NIL;
		}
	}

	if (dirName == NIL)
	{
		if (dirPath.EndsWith(GetDirectoryDelimiter())) dirPath[dirPath.Length() - 1] = 0;

		Int	 lastBS = dirPath.FindLast(GetDirectoryDelimiter());

		if (lastBS >= 0)
		{
			dirName = dirPath.Tail(dirPath.Length() - lastBS - 1);
			dirPath[lastBS] = 0;
		}
	}

	/* Replace ../ elements.
	 */
	if (!dirPath.EndsWith(GetDirectoryDelimiter())) dirPath.Append(GetDirectoryDelimiter());

	while (dirPath.Contains(String(GetDirectoryDelimiter()).Append("..").Append(GetDirectoryDelimiter())))
	{
		Int	 upPos	= dirPath.Find(String(GetDirectoryDelimiter()).Append("..").Append(GetDirectoryDelimiter()));
		Int	 prePos	= dirPath.Head(upPos).FindLast(GetDirectoryDelimiter());

		dirPath.Replace(dirPath.SubString(prePos, upPos - prePos + 3), String());
	}

	if (dirPath.EndsWith(Directory::GetDirectoryDelimiter())) dirPath[dirPath.Length() - 1] = 0;
}

S::Directory::Directory(const Directory &iDirectory)
{
	*this = iDirectory;
}

S::Directory::Directory(const int nil)
{
}

S::Directory::~Directory()
{
}

S::Directory &S::Directory::operator =(const Directory &nDirectory)
{
	if (&nDirectory == this) return *this;

	dirName = nDirectory.dirName;
	dirPath = nDirectory.dirPath;

	return *this;
}

S::Directory::operator S::String() const
{
	return String(dirPath).Append(dirName == NIL ? String() : String(Directory::GetDirectoryDelimiter())).Append(dirName);
}

const S::String &S::Directory::GetDirectoryName() const
{
	return dirName;
}

const S::String &S::Directory::GetDirectoryPath() const
{
	return dirPath;
}

const S::Array<S::File> &S::Directory::GetFiles() const
{
	return GetFilesByPattern("*");
}

const S::Array<S::Directory> &S::Directory::GetDirectories() const
{
	return GetDirectoriesByPattern("*");
}

const S::Array<S::File> &S::Directory::GetFilesByPattern(const String &pattern) const
{
	files.RemoveAll();

#ifdef __WIN32__
	WIN32_FIND_DATA	 findData;
	HANDLE		 handle = FindFirstFile(String(GetUnicodePathPrefix(*this)).Append(*this).Append("\\").Append(pattern), &findData);

	Bool	 success = (handle != INVALID_HANDLE_VALUE);

	while (success)
	{
		if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) files.Add(File(findData.cFileName, *this));

		success = FindNextFile(handle, &findData);
	}

	FindClose(handle);
#else
	String	 path	  = String(*this).Replace("*", "\\*").Replace("?", "\\?").
					  Replace("[", "\\[").Replace("]", "\\]");
	glob_t	 fileData = { 0 };

	if (glob(path.Append("/").Append(pattern).ConvertTo("UTF-8"), GLOB_MARK, NIL, &fileData) == 0)
	{
		String::InputFormat	 inputFormat("UTF-8");

		for (size_t i = 0; i < fileData.gl_pathc; i++)
		{
			if (!String(fileData.gl_pathv[i]).EndsWith("/")) files.Add(File(fileData.gl_pathv[i]));
		}

		globfree(&fileData);
	}
#endif

	return files;
}

const S::Array<S::Directory> &S::Directory::GetDirectoriesByPattern(const String &pattern) const
{
	directories.RemoveAll();

#ifdef __WIN32__
	WIN32_FIND_DATA	 findData;
	HANDLE		 handle = FindFirstFile(String(GetUnicodePathPrefix(*this)).Append(*this).Append("\\").Append(pattern), &findData);

	Bool	 success = (handle != INVALID_HANDLE_VALUE);

	while (success)
	{
		if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && String(findData.cFileName) != "." && String(findData.cFileName) != "..") directories.Add(Directory(findData.cFileName, *this));

		success = FindNextFile(handle, &findData);
	}

	FindClose(handle);
#else
	String	 path	  = String(*this).Replace("*", "\\*").Replace("?", "\\?").
					  Replace("[", "\\[").Replace("]", "\\]");
	glob_t	 fileData = { 0 };

	if (glob(path.Append("/").Append(pattern).ConvertTo("UTF-8"), GLOB_MARK | GLOB_ONLYDIR, NIL, &fileData) == 0)
	{
		String::InputFormat	 inputFormat("UTF-8");

		for (size_t i = 0; i < fileData.gl_pathc; i++)
		{
			if (String(fileData.gl_pathv[i]).EndsWith("/")) directories.Add(Directory(fileData.gl_pathv[i]));
		}

		globfree(&fileData);
	}
#endif

	return directories;
}

S::DateTime S::Directory::GetCreateTime() const
{
	DateTime	 dateTime;

	if (!Exists()) return dateTime;

#ifdef __WIN32__
	WIN32_FIND_DATA findData;
	HANDLE		 handle = FindFirstFile(String(GetUnicodePathPrefix(*this)).Append(*this), &findData);

	FindClose(handle);

	SYSTEMTIME	 time;

	FileTimeToSystemTime(&findData.ftCreationTime, &time);

	dateTime.SetYMD(time.wYear, time.wMonth, time.wDay);
	dateTime.SetHMS(time.wHour, time.wMinute, time.wSecond);
#endif

	return dateTime;
}

S::Bool S::Directory::Exists() const
{
#ifdef __WIN32__
	/* Check if root directory of a drive
	 */
	if (dirPath[dirPath.Length() - 1] == ':' && dirName == NIL)
	{
		DWORD	 drives = GetLogicalDrives();

		if ((drives >> (dirPath.ToUpper()[0] - 'A')) & 1) return True;
		else						  return False;
	}

	WIN32_FIND_DATAW	 findData;
	HANDLE			 handle = FindFirstFile(String(GetUnicodePathPrefix(*this)).Append(*this), &findData);

	if (handle == INVALID_HANDLE_VALUE) return False;

	FindClose(handle);

	if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) return False;
#else
	struct stat	 info;

	if (stat(String(*this).ConvertTo("UTF-8"), &info) != 0) return False;

	if (!S_ISDIR(info.st_mode)) return False;
#endif

	return True;
}

S::Int S::Directory::Create()
{
	if (Exists()) return Success();

	Bool	 result	   = False;
	String	 directory = *this;

	for (Int i = 1; i <= directory.Length(); i++)
	{
		if (directory[i] == '\\' || directory[i] == '/' || directory[i] == 0)
		{
			String	 path = directory.Head(i);

#ifdef __WIN32__
			result = CreateDirectory(String(GetUnicodePathPrefix(path)).Append(path), NIL);
#else
			if (mkdir(path.ConvertTo("UTF-8"), 0777) == 0) result = True;
			else					       result = False;
#endif
		}
	}

	if (result == False) return Error();
	else		     return Success();
}

S::Int S::Directory::Copy(const Directory &destination)
{
	return Error();
}

S::Int S::Directory::Move(const Directory &destination)
{
	if (!Exists()) return Error();

#ifdef __WIN32__
	Bool	 result = MoveFile(String(GetUnicodePathPrefix(*this)).Append(*this), String(GetUnicodePathPrefix(destination)).Append(destination));
#else
	Bool	 result = (rename(String(*this).ConvertTo("UTF-8"), String(destination).ConvertTo("UTF-8")) == 0);
#endif

	if (result == False) return Error();
	else		     return Success();
}

S::Int S::Directory::Delete()
{
#ifdef __WIN32__
	Bool	 result = RemoveDirectory(String(GetUnicodePathPrefix(*this)).Append(*this));
#else
	Bool	 result = (rmdir(String(*this).ConvertTo("UTF-8")) == 0);
#endif

	if (result == False) return Error();
	else		     return Success();
}

S::Int S::Directory::Empty()
{
	Directory	 backupDir = GetActiveDirectory();

	SetActiveDirectory(*this);

#ifdef __WIN32__
	WIN32_FIND_DATA	 findData;
	HANDLE		 handle = FindFirstFile(String(GetUnicodePathPrefix("*")).Append("*"), &findData);

	if (handle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (String(findData.cFileName) != "." && String(findData.cFileName) != "..")
			{
				if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					Directory	 dir(NIL, findData.cFileName);

					dir.Empty();
					dir.Delete();
				}
				else
				{
					DeleteFile(findData.cFileName);
				}
			}
		}
		while (FindNextFile(handle, &findData));

		FindClose(handle);
	}
#endif

	SetActiveDirectory(backupDir);

	return Success();
}

const char *S::Directory::GetDirectoryDelimiter()
{
	if (directoryDelimiter == NIL)
	{
#ifdef __WIN32__
		directoryDelimiter = (char *) "\\";
#else
		directoryDelimiter = (char *) "/";
#endif
	}

	return directoryDelimiter;
}

const char *S::Directory::GetUnicodePathPrefix(const String &path)
{
#ifdef __WIN32__
	if (path.StartsWith("\\\\")) return "";
#endif

	if (unicodePathPrefix == NIL)
	{
#ifdef __WIN32__
		unicodePathPrefix = (char *) "\\\\?\\";
#else
		unicodePathPrefix = (char *) "";
#endif
	}

	return unicodePathPrefix;
}

S::Directory S::Directory::GetActiveDirectory()
{
#ifdef __WIN32__
	Buffer<wchar_t>	 buffer(32768 + 1);

	GetCurrentDirectory(buffer.Size(), buffer);

	String		 dir = (wchar_t *) buffer;
#else
	Buffer<char>	 buffer(PATH_MAX + 1);
	String		 dir;

	if (getcwd(buffer, buffer.Size()) != NIL) dir = buffer;
#endif

	return Directory(NIL, dir);
}

S::Int S::Directory::SetActiveDirectory(const Directory &directory)
{
#ifdef __WIN32__
	Bool	 result = SetCurrentDirectory(String(GetUnicodePathPrefix(directory)).Append(directory).Append("\\"));
#else
	Bool	 result = (chdir(String(directory).ConvertTo("UTF-8")) == 0);
#endif

	if (result == False) return Error();
	else		     return Success();
}
