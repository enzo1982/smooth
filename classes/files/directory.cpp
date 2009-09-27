 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/files/directory.h>

#ifndef __WIN32__
#	include <glob.h>
#	include <unistd.h>
#	include <stdio.h>
#	include <sys/stat.h>
#endif

#ifdef __WIN32__
S::String	 S::Directory::dirDelimiter = "\\";
#else
S::String	 S::Directory::dirDelimiter = "/";
#endif

S::Directory::Directory(const String &iDirName, const String &iDirPath)
{
	dirName = iDirName;
	dirPath = iDirPath;

	if (dirName != NIL && dirPath == NIL)
	{
#ifdef __WIN32__
		if (dirName[1] == ':' || dirName.StartsWith("\\\\"))
#else
		if (dirName.StartsWith(dirDelimiter) || dirName.StartsWith("~"))
#endif
		{
			dirPath = dirName;
			dirName = NIL;
		}
	}

	if (dirName == NIL)
	{
		if (dirPath.EndsWith(dirDelimiter)) dirPath[dirPath.Length() - 1] = 0;

		for (Int lastBS = dirPath.Length() - 1; lastBS >= 0; lastBS--)
		{
			if (dirPath[lastBS] == dirDelimiter[0])
			{
				for (Int i = lastBS + 1; i < dirPath.Length(); i++) dirName[i - lastBS - 1] = dirPath[i];

				dirPath[lastBS] = 0;

				break;
			}
		}
	}
	else if (dirPath == NIL)
	{
		dirPath = Directory::GetActiveDirectory();
	}

	dirPath.Replace("/",  Directory::GetDirectoryDelimiter());
	dirPath.Replace("\\", Directory::GetDirectoryDelimiter());

	dirName.Replace("/",  Directory::GetDirectoryDelimiter());
	dirName.Replace("\\", Directory::GetDirectoryDelimiter());
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
	return String(dirPath).Append(dirName == NIL ? String() : String(dirDelimiter).Append(dirName));
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
	return GetFilesByPattern("*.*");
}

const S::Array<S::Directory> &S::Directory::GetDirectories() const
{
	directories.RemoveAll();

#ifdef __WIN32__
	HANDLE		 handle;
	WIN32_FIND_DATAW findDataW;
	WIN32_FIND_DATAA findDataA;

	if (Setup::enableUnicode)	handle = FindFirstFileW(String("\\\\?\\").Append(*this).Append("\\*.*"), &findDataW);
	else				handle = FindFirstFileA(String(*this).Append("\\*.*"), &findDataA);

	Bool	 success = (handle != INVALID_HANDLE_VALUE);

	while (success)
	{
		if (Setup::enableUnicode)
		{
			if ((findDataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && String(findDataW.cFileName) != "." && String(findDataW.cFileName) != "..") directories.Add(Directory(findDataW.cFileName, *this));

			success = FindNextFileW(handle, &findDataW);
		}
		else
		{
			if ((findDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && String(findDataA.cFileName) != "." && String(findDataA.cFileName) != "..") directories.Add(Directory(findDataA.cFileName, *this));

			success = FindNextFileA(handle, &findDataA);
		}
	}

	FindClose(handle);
#endif

	return directories;
}

const S::Array<S::File> &S::Directory::GetFilesByPattern(const String &pattern) const
{
	files.RemoveAll();

#ifdef __WIN32__
	HANDLE		 handle;
	WIN32_FIND_DATAW findDataW;
	WIN32_FIND_DATAA findDataA;

	if (Setup::enableUnicode)	handle = FindFirstFileW(String("\\\\?\\").Append(*this).Append("\\").Append(pattern), &findDataW);
	else				handle = FindFirstFileA(String(*this).Append("\\").Append(pattern), &findDataA);

	Bool	 success = (handle != INVALID_HANDLE_VALUE);

	while (success)
	{
		if (Setup::enableUnicode)
		{
			if (!(findDataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) files.Add(File(findDataW.cFileName, *this));

			success = FindNextFileW(handle, &findDataW);
		}
		else
		{
			if (!(findDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) files.Add(File(findDataA.cFileName, *this));

			success = FindNextFileA(handle, &findDataA);
		}
	}

	FindClose(handle);
#else
	glob_t	*fileData = new glob_t;

	if (glob(String(*this).Append("/").Append(pattern), GLOB_NOSORT, NIL, fileData) == 0)
	{
		for (Int i = 0; i < fileData->gl_pathc; i++)
		{
			files.Add(File(fileData->gl_pathv[i]));
		}
	}

	globfree(fileData);

	delete fileData;
#endif

	return files;
}

S::DateTime S::Directory::GetCreateTime() const
{
	DateTime	 dateTime;

	if (!Exists()) return dateTime;

#ifdef __WIN32__
	HANDLE		 handle;
	WIN32_FIND_DATAW findDataW;
	WIN32_FIND_DATAA findDataA;

	if (Setup::enableUnicode)	handle = FindFirstFileW(String("\\\\?\\").Append(*this), &findDataW);
	else				handle = FindFirstFileA(String(*this), &findDataA);

	FindClose(handle);

	SYSTEMTIME	 time;

	if (Setup::enableUnicode)	FileTimeToSystemTime(&findDataW.ftCreationTime, &time);
	else				FileTimeToSystemTime(&findDataA.ftCreationTime, &time);

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

	HANDLE			 handle;
	WIN32_FIND_DATAW	 findDataW;
	WIN32_FIND_DATAA	 findDataA;

	if (Setup::enableUnicode)	handle = FindFirstFileW(String("\\\\?\\").Append(*this), &findDataW);
	else				handle = FindFirstFileA(String(*this), &findDataA);

	if (handle == INVALID_HANDLE_VALUE) return False;

	FindClose(handle);

	if (Setup::enableUnicode)
	{
		if (!(findDataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) return False;
	}
	else
	{
		if (!(findDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) return False;
	}
#else
	struct stat	 info;

	if (stat(String(*this), &info) != 0) return False;

	if (!S_ISDIR(info.st_mode)) return False;
#endif

	return True;
}

S::Int S::Directory::Create()
{
	if (Exists()) return Error();

	String	 directory = *this;

	for (Int i = 5; i <= directory.Length(); i++)
	{
		if (directory[i] == '\\' || directory[i] == '/' || directory[i] == 0)
		{
			String	 path = directory;

			path[i] = 0;

#ifdef __WIN32__
			if (Setup::enableUnicode)	CreateDirectoryW(String("\\\\?\\").Append(path), NIL);
			else				CreateDirectoryA(path, NIL);
#else
			mkdir(path, 0755);
#endif
		}
	}

	return Success();
}

S::Int S::Directory::Copy(const String &destination)
{
	return Error();
}

S::Int S::Directory::Move(const String &destination)
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

S::Int S::Directory::Delete()
{
	Bool	 result = False;

#ifdef __WIN32__
	if (Setup::enableUnicode)	result = RemoveDirectoryW(String("\\\\?\\").Append(*this));
	else				result = RemoveDirectoryA(String(*this));
#else
	result = (rmdir(String(*this)) == 0);
#endif

	if (result == False)	return Error();
	else			return Success();
}

S::Int S::Directory::Empty()
{
	Directory	 backupDir = GetActiveDirectory();

	SetActiveDirectory(*this);

#ifdef __WIN32__
	WIN32_FIND_DATAW	 findDataW;
	WIN32_FIND_DATAA	 findDataA;
	HANDLE			 handle;

	if (Setup::enableUnicode)	handle = FindFirstFileW(String("\\\\?\\").Append("*"), &findDataW);
	else				handle = FindFirstFileA(String("*"), &findDataA);

	if (handle != INVALID_HANDLE_VALUE)
	{
		if (Setup::enableUnicode)
		{
			do
			{
				if (String(findDataW.cFileName) != "." && String(findDataW.cFileName) != "..")
				{
					if (findDataW.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Directory	 dir(NIL, findDataW.cFileName);

						dir.Empty();
						dir.Delete();
					}
					else
					{
						DeleteFileW(findDataW.cFileName);
					}
				}
			}
			while (FindNextFileW(handle, &findDataW));
		}
		else
		{
			do
			{
				if (String(findDataA.cFileName) != "." && String(findDataA.cFileName) != "..")
				{
					if (findDataA.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					{
						Directory	 dir(NIL, findDataA.cFileName);

						dir.Empty();
						dir.Delete();
					}
					else
					{
						DeleteFileA(findDataA.cFileName);
					}
				}
			}
			while (FindNextFileA(handle, &findDataA));
		}

		FindClose(handle);
	}
#endif

	SetActiveDirectory(backupDir);

	return Success();
}

const S::String &S::Directory::GetDirectoryDelimiter()
{
	return dirDelimiter;
}

S::Directory S::Directory::GetActiveDirectory()
{
	String	 dir;

#ifdef __WIN32__
	if (Setup::enableUnicode)
	{
		wchar_t	*bufferw = new wchar_t [32000];

		GetCurrentDirectoryW(32000, bufferw);

		dir = bufferw;

		delete [] bufferw;
	}
	else
	{
		char	*buffera = new char [MAX_PATH];

		GetCurrentDirectoryA(MAX_PATH, buffera);

		dir = buffera;

		delete [] buffera;
	}
#else
	char	*buffer = new char [MAX_PATH];

	getcwd(buffer, MAX_PATH);

	dir = buffer;

	delete [] buffer;
#endif

	return Directory(NIL, dir);
}

S::Int S::Directory::SetActiveDirectory(const Directory &directory)
{
	Bool	 result = False;

#ifdef __WIN32__
	if (Setup::enableUnicode)	result = SetCurrentDirectoryW(String("\\\\?\\").Append(directory));
	else				result = SetCurrentDirectoryA(String(directory));
#else
	result = (chdir(String(directory)) == 0);
#endif

	if (result == False)	return Error();
	else			return Success();
}
