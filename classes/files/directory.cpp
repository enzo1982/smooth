 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/files/directory.h>

S::Directory::Directory(String iDirName, String iDirPath)
{
	dirName = iDirName;
	dirPath = iDirPath;

	if (dirName != NIL && dirPath == NIL)
	{
		if (dirName[1] == ':')
		{
			dirPath = dirName;
			dirName = NIL;
		}
	}

	if (dirName == NIL)
	{
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

S::String S::Directory::GetDirectoryName()
{
	return dirName;
}

S::String S::Directory::GetDirectoryPath()
{
	return dirPath;
}

S::Array<S::File> &S::Directory::GetFiles()
{
	return files;
}

S::Array<S::Directory> &S::Directory::GetDirectories()
{
	return directories;
}

S::DateTime S::Directory::GetCreateTime()
{
	DateTime	 dateTime;

	if (!Exists()) return dateTime;

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

	return dateTime;
}

S::Bool S::Directory::Exists()
{
	HANDLE			 handle;
	WIN32_FIND_DATAW	 findDataW;
	WIN32_FIND_DATAA	 findDataA;

	if (Setup::enableUnicode)	handle = FindFirstFileW(String(*this), &findDataW);
	else				handle = FindFirstFileA(String(*this), &findDataA);

	if (handle == INVALID_HANDLE_VALUE) return False;

	FindClose(handle);

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

			if (Setup::enableUnicode)	CreateDirectoryW(path, NIL);
			else				CreateDirectoryA(path, NIL);
		}
	}

	return Success();
}

S::Int S::Directory::Copy(String destination)
{
	return Error();
}

S::Int S::Directory::Move(String destination)
{
	return Error();
}

S::Int S::Directory::Delete()
{
	Bool	 result = False;

	if (Setup::enableUnicode)	result = RemoveDirectoryW(String(*this));
	else				result = RemoveDirectoryA(String(*this));

	if (result == False)	return Error();
	else			return Success();
}

S::Int S::Directory::Empty()
{
	Directory	 backupDir = GetActiveDirectory();

	if (Setup::enableUnicode)	SetCurrentDirectoryW(String(*this));
	else				SetCurrentDirectoryA(String(*this));

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

	if (Setup::enableUnicode)	SetCurrentDirectoryW(String(backupDir));
	else				SetCurrentDirectoryA(String(backupDir));

	return Success();
}

S::Directory S::Directory::GetActiveDirectory()
{
	wchar_t	*bufferw = new wchar_t [MAX_PATH];
	char	*buffera = new char [MAX_PATH];

	if (Setup::enableUnicode)	GetCurrentDirectoryW(MAX_PATH, bufferw);
	else				GetCurrentDirectoryA(MAX_PATH, buffera);

	String	 dir = Setup::enableUnicode ? String(bufferw) : String(buffera);

	delete [] bufferw;
	delete [] buffera;

	return Directory(NIL, dir);
}

S::Int S::Directory::SetActiveDirectory(const Directory &directory)
{
	Bool	 result = False;

	if (Setup::enableUnicode)	result = SetCurrentDirectoryW(String(directory));
	else				result = SetCurrentDirectoryA(String(directory));

	if (result == False)	return Error();
	else			return Success();
}
