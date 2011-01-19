 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/files/directory.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <glob.h>
#	include <unistd.h>
#	include <stdio.h>
#	include <limits.h>
#	include <sys/stat.h>

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
		if (dirName[1] == ':' || dirName.StartsWith("\\\\"))
#else
		if (dirName.StartsWith(GetDirectoryDelimiter()) || dirName.StartsWith("~"))
#endif
		{
			dirPath = dirName;
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
	else if (dirPath == NIL)
	{
		dirPath = Directory::GetActiveDirectory();
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
	return GetFilesByPattern("*.*");
}

const S::Array<S::Directory> &S::Directory::GetDirectories() const
{
	directories.RemoveAll();

#ifdef __WIN32__
	HANDLE		 handle;
	WIN32_FIND_DATAW findDataW;
	WIN32_FIND_DATAA findDataA;

	if (Setup::enableUnicode)	handle = FindFirstFileW(String(GetUnicodePathPrefix()).Append(*this).Append("\\*.*"), &findDataW);
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
#else
	glob_t	*fileData = new glob_t;

	if (glob(String(*this).Append("/*").ConvertTo("UTF-8"), GLOB_MARK | GLOB_ONLYDIR, NIL, fileData) == 0)
	{
		String	 previousInputFormat = String::SetInputFormat("UTF-8");

		for (UnsignedInt i = 0; i < fileData->gl_pathc; i++)
		{
			if (String(fileData->gl_pathv[i]).EndsWith("/")) directories.Add(Directory(fileData->gl_pathv[i]));
		}

		String::SetInputFormat(previousInputFormat);
	}

	globfree(fileData);

	delete fileData;
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

	if (Setup::enableUnicode)	handle = FindFirstFileW(String(GetUnicodePathPrefix()).Append(*this).Append("\\").Append(pattern), &findDataW);
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

	if (glob(String(*this).Append("/").Append(pattern).ConvertTo("UTF-8"), GLOB_MARK, NIL, fileData) == 0)
	{
		String	 previousInputFormat = String::SetInputFormat("UTF-8");

		for (UnsignedInt i = 0; i < fileData->gl_pathc; i++)
		{
			if (!String(fileData->gl_pathv[i]).EndsWith("/")) files.Add(File(fileData->gl_pathv[i]));
		}

		String::SetInputFormat(previousInputFormat);
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

	if (Setup::enableUnicode)	handle = FindFirstFileW(String(GetUnicodePathPrefix()).Append(*this), &findDataW);
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

	if (Setup::enableUnicode)	handle = FindFirstFileW(String(GetUnicodePathPrefix()).Append(*this), &findDataW);
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

	if (stat(String(*this).ConvertTo("UTF-8"), &info) != 0) return False;

	if (!S_ISDIR(info.st_mode)) return False;
#endif

	return True;
}

S::Int S::Directory::Create()
{
	if (Exists()) return Success();

	String	 directory = *this;

	for (Int i = 0; i <= directory.Length(); i++)
	{
		if (directory[i] == '\\' || directory[i] == '/' || directory[i] == 0)
		{
			String	 path = directory;

			path[i] = 0;

#ifdef __WIN32__
			if (Setup::enableUnicode)	CreateDirectoryW(String(GetUnicodePathPrefix()).Append(path), NIL);
			else				CreateDirectoryA(path, NIL);
#else
			mkdir(path.ConvertTo("UTF-8"), 0755);
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
	if (Setup::enableUnicode)	result = MoveFileW(String(GetUnicodePathPrefix()).Append(*this), String(GetUnicodePathPrefix()).Append(destination));
	else				result = MoveFileA(String(*this), destination);
#else
	result = (rename(String(*this).ConvertTo("UTF-8"), destination.ConvertTo("UTF-8")) == 0);
#endif

	if (result == False)	return Error();
	else			return Success();
}

S::Int S::Directory::Delete()
{
	Bool	 result = False;

#ifdef __WIN32__
	if (Setup::enableUnicode)	result = RemoveDirectoryW(String(GetUnicodePathPrefix()).Append(*this));
	else				result = RemoveDirectoryA(String(*this));
#else
	result = (rmdir(String(*this).ConvertTo("UTF-8")) == 0);
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

	if (Setup::enableUnicode)	handle = FindFirstFileW(String(GetUnicodePathPrefix()).Append("*"), &findDataW);
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

const char *S::Directory::GetUnicodePathPrefix()
{
	if (unicodePathPrefix == NIL)
	{
#ifdef __WIN32__
		OSVERSIONINFOA	 vInfo;

		vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

		GetVersionExA(&vInfo);

		if (vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT) unicodePathPrefix = (char *) "\\\\?\\";
		else						 unicodePathPrefix = (char *) "";
#else
		unicodePathPrefix = (char *) "";
#endif
	}

	return unicodePathPrefix;
}

S::Directory S::Directory::GetActiveDirectory()
{
	String	 dir;

#ifdef __WIN32__
	if (Setup::enableUnicode)
	{
		Buffer<wchar_t>	 buffer(32768);

		GetCurrentDirectoryW(buffer.Size(), buffer);

		dir = buffer;
	}
	else
	{
		Buffer<char>	 buffer(MAX_PATH);

		GetCurrentDirectoryA(buffer.Size(), buffer);

		dir = buffer;
	}
#else
	Buffer<char>	 buffer(PATH_MAX);

	if (getcwd(buffer, buffer.Size()) != NIL)
	{
		dir = buffer;
	}
#endif

	return Directory(NIL, dir);
}

S::Int S::Directory::SetActiveDirectory(const Directory &directory)
{
	Bool	 result = False;

#ifdef __WIN32__
	if (Setup::enableUnicode)	result = SetCurrentDirectoryW(String(GetUnicodePathPrefix()).Append(directory));
	else				result = SetCurrentDirectoryA(String(directory));
#else
	result = (chdir(String(directory).ConvertTo("UTF-8")) == 0);
#endif

	if (result == False)	return Error();
	else			return Success();
}
