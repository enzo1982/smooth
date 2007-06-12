 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/files/directory.h>

S::Directory::Directory(const String &iDirName, const String &iDirPath)
{
	dirName = iDirName;
	dirPath = iDirPath;

	if (dirName != NIL && dirPath == NIL)
	{
		if (dirName[1] == ':' || dirName.StartsWith("\\\\"))
		{
			dirPath = dirName;
			dirName = NIL;
		}
	}

	if (dirName == NIL)
	{
		if (dirPath[dirPath.Length() - 1] == '\\') dirPath[dirPath.Length() - 1] = 0;

		for (Int lastBS = dirPath.Length() - 1; lastBS >= 0; lastBS--)
		{
			if (dirPath[lastBS] == '\\')
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
}

S::Directory::Directory(const Directory &iDirectory)
{
	dirName = iDirectory.dirName;
	dirPath = iDirectory.dirPath;
}

S::Directory::Directory(const int nil)
{
}

S::Directory::~Directory()
{
}

S::Directory::operator S::String() const
{
	return String(dirPath).Append("\\").Append(dirName);
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

	if (Setup::enableUnicode)	handle = FindFirstFileW(String(*this).Append("\\*.*"), &findDataW);
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

	if (Setup::enableUnicode)	handle = FindFirstFileW(String(*this).Append("\\").Append(pattern), &findDataW);
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

	if (Setup::enableUnicode)	handle = FindFirstFileW(String(*this), &findDataW);
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
	// Check if root directory of a drive
	if (dirPath[dirPath.Length() - 1] == ':' && dirName == NIL) return True;

#ifdef __WIN32__
	HANDLE			 handle;
	WIN32_FIND_DATAW	 findDataW;
	WIN32_FIND_DATAA	 findDataA;

	if (Setup::enableUnicode)	handle = FindFirstFileW(String(*this), &findDataW);
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
			if (Setup::enableUnicode)	CreateDirectoryW(path, NIL);
			else				CreateDirectoryA(path, NIL);
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
	if (Setup::enableUnicode)	result = MoveFileW(String(dirPath).Append("\\").Append(dirName), destination);
	else				result = MoveFileA(String(dirPath).Append("\\").Append(dirName), destination);
#endif

	if (result == False)	return Error();
	else			return Success();
}

S::Int S::Directory::Delete()
{
	Bool	 result = False;

#ifdef __WIN32__
	if (Setup::enableUnicode)	result = RemoveDirectoryW(String(*this));
	else				result = RemoveDirectoryA(String(*this));
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

	if (Setup::enableUnicode)	handle = FindFirstFileW(String("*"), &findDataW);
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

S::Directory S::Directory::GetActiveDirectory()
{
	wchar_t	*bufferw = new wchar_t [MAX_PATH];
	char	*buffera = new char [MAX_PATH];

#ifdef __WIN32__
	if (Setup::enableUnicode)	GetCurrentDirectoryW(MAX_PATH, bufferw);
	else				GetCurrentDirectoryA(MAX_PATH, buffera);
#endif

	String	 dir = Setup::enableUnicode ? String(bufferw) : String(buffera);

	delete [] bufferw;
	delete [] buffera;

	return Directory(NIL, dir);
}

S::Int S::Directory::SetActiveDirectory(const Directory &directory)
{
	Bool	 result = False;

#ifdef __WIN32__
	if (Setup::enableUnicode)	result = SetCurrentDirectoryW(String(directory));
	else				result = SetCurrentDirectoryA(String(directory));
#endif

	if (result == False)	return Error();
	else			return Success();
}
