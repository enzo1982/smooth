 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/system.h>
#include <smooth/files/directory.h>
#include <smooth/version.h>

#ifdef __WIN32__
#	include <time.h>
#	include <shlobj.h>
#	include <shellapi.h>
#else
#	include <sys/time.h>
#	include <unistd.h>
#	include <pwd.h>
#	include <stdlib.h>
#	include <string.h>
#endif

S::Int S::System::System::nextGUID	= 0;

S::System::System::System()
{
}

S::System::System::System(const System &)
{
}

S::Int S::System::System::RequestGUID()
{
	return nextGUID++;
}

S::String S::System::System::GetVersionString()
{
	return SMOOTH_VERSION;
}

S::String S::System::System::GetAPIVersion()
{
	return SMOOTH_APIVERSION;
}

S::UnsignedInt64 S::System::System::Clock()
{
#ifdef __WIN32__
	return clock() * 1000 / CLOCKS_PER_SEC;
#else
	timeval	 tv;

	gettimeofday(&tv, NIL);

	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

S::Bool S::System::System::Sleep(UnsignedInt mSeconds)
{
#ifdef __WIN32__
	::Sleep(mSeconds);
#else
	usleep(mSeconds * 1000);
#endif

	return True;
}

S::Bool S::System::System::OpenURL(const String &url)
{
#ifdef __WIN32__
	if (Setup::enableUnicode) ShellExecuteW(NULL, (wchar_t *) L"open", url, NULL, NULL, 0);
	else			  ShellExecuteA(NULL, "open", url, NULL, NULL, 0);
#else
	static const char	*browsers[] = { "firefox", "konqueror", "opera", "epiphany", "mozilla", "netscape" };

	String	 command;

	for (int i = 0; i < 6; i++)
	{
		if (i > 0) command.Append(" || ");

		command.Append(browsers[i]).Append(" \"").Append(url).Append("\"");
	}

	pid_t	 pid = fork();

	if (pid == 0)
	{
		execl("/bin/sh", "sh", "-c", (char *) command, NULL);
		exit(0);
	}
#endif

	return True;
}

S::String S::System::System::GetWindowsRootDirectory()
{
	String	 windowsDir;

#ifdef __WIN32__
	if (Setup::enableUnicode)
	{
		wchar_t	*bufferw = new wchar_t [MAX_PATH];

		GetWindowsDirectoryW(bufferw, MAX_PATH);

		windowsDir = bufferw;

		delete [] bufferw;
	}
	else
	{
		char	*buffera = new char [MAX_PATH];

		GetWindowsDirectoryA(buffera, MAX_PATH);

		windowsDir = buffera;

		delete [] buffera;
	}
#else
	windowsDir = NIL;
#endif

	if (!windowsDir.EndsWith(Directory::GetDirectoryDelimiter())) windowsDir.Append(Directory::GetDirectoryDelimiter());
	if (windowsDir == Directory::GetDirectoryDelimiter()) windowsDir = NIL;

	return windowsDir;
}

S::String S::System::System::GetProgramFilesDirectory()
{
	String	 programsDir;

#ifdef __WIN32__
	HKEY	 currentVersion;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion", 0, KEY_QUERY_VALUE, &currentVersion) == ERROR_SUCCESS)
	{
		/* We need to use the ANSI version of RegQueryValueEx, because
		 * the Unicode version is not compatible with MSLU.
		 */
		DWORD	 size = MAX_PATH;
		char	*buffer = new char [size];

		RegQueryValueExA(currentVersion, String("ProgramFilesDir"), 0, NIL, (BYTE *) buffer, &size);

		programsDir = buffer;

		delete [] buffer;

		RegCloseKey(currentVersion);
	}

	if (programsDir == NIL)
	{
		/* Failed to get the program files directory from the registry.
		 * Get the directory name from the environment variable.
		 */
		if (Setup::enableUnicode)
		{
			wchar_t	*bufferw = new wchar_t [MAX_PATH];

			ExpandEnvironmentStringsW(String("%ProgramFiles%"), bufferw, MAX_PATH);

			programsDir = bufferw;

			delete [] bufferw;
		}
		else
		{
			char	*buffera = new char [MAX_PATH];

			ExpandEnvironmentStringsA(String("%ProgramFiles%"), buffera, MAX_PATH);

			programsDir = buffera;

			delete [] buffera;
		}
	}
#else
	programsDir = NIL;
#endif

	if (!programsDir.EndsWith(Directory::GetDirectoryDelimiter())) programsDir.Append(Directory::GetDirectoryDelimiter());
	if (programsDir == Directory::GetDirectoryDelimiter()) programsDir = NIL;

	return programsDir;
}

S::String S::System::System::GetApplicationDataDirectory()
{
	String	 configDir;

#ifdef __WIN32__
	ITEMIDLIST	*idlist;


	SHGetSpecialFolderLocation(NIL, CSIDL_APPDATA, &idlist);

	if (Setup::enableUnicode)
	{
		wchar_t	*bufferw = new wchar_t [MAX_PATH];

		SHGetPathFromIDListW(idlist, bufferw);

		configDir = bufferw;

		delete [] bufferw;
	}
	else
	{
		char	*buffera = new char [MAX_PATH];

		SHGetPathFromIDListA(idlist, buffera);

		configDir = buffera;

		delete [] buffera;
	}

	CoTaskMemFree(idlist);
#else
	passwd	*pw = getpwuid(getuid());

	if (pw != NIL)	configDir = pw->pw_dir;
	else		configDir = "~";
#endif

	if (!configDir.EndsWith(Directory::GetDirectoryDelimiter())) configDir.Append(Directory::GetDirectoryDelimiter());
	if (configDir == Directory::GetDirectoryDelimiter()) configDir = NIL;

	return configDir;
}

S::String S::System::System::GetPersonalFilesDirectory()
{
	String	 personalDir;

#ifdef __WIN32__
	ITEMIDLIST	*idlist;

	SHGetSpecialFolderLocation(NIL, CSIDL_PERSONAL, &idlist);

	if (Setup::enableUnicode)
	{
		wchar_t	*bufferw = new wchar_t [MAX_PATH];

		SHGetPathFromIDListW(idlist, bufferw);

		personalDir = bufferw;

		delete [] bufferw;
	}
	else
	{
		char	*buffera = new char [MAX_PATH];

		SHGetPathFromIDListA(idlist, buffera);

		personalDir = buffera;

		delete [] buffera;
	}

	CoTaskMemFree(idlist);

	if (personalDir == NIL) personalDir = "C:";
#else
	passwd	*pw = getpwuid(getuid());

	if (pw != NIL)	personalDir = pw->pw_dir;
	else		personalDir = "~";
#endif

	if (!personalDir.EndsWith(Directory::GetDirectoryDelimiter())) personalDir.Append(Directory::GetDirectoryDelimiter());
	if (personalDir == Directory::GetDirectoryDelimiter()) personalDir = NIL;

	return personalDir;
}

S::String S::System::System::GetTempDirectory()
{
	String	 tempDir;

#ifdef __WIN32__
	/* We need to use the ANSI version of GetTempPath, because
	 * the Unicode version is not compatible with MSLU.
	 */
	char	*buffera = new char [MAX_PATH];

	GetTempPathA(MAX_PATH, buffera);

	tempDir = buffera;

	delete [] buffera;
#else
	tempDir = "/var/tmp";
#endif

	if (!tempDir.EndsWith(Directory::GetDirectoryDelimiter())) tempDir.Append(Directory::GetDirectoryDelimiter());
	if (tempDir == Directory::GetDirectoryDelimiter()) tempDir = NIL;

	return tempDir;
}
