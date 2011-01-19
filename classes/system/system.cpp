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
#	include <limits.h>
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
#if defined __WIN32__
	if (Setup::enableUnicode) ShellExecuteW(NULL, (wchar_t *) L"open", url, NULL, NULL, 0);
	else			  ShellExecuteA(NULL, "open", url, NULL, NULL, 0);
#elif defined __APPLE__
	if (!fork())
	{
		execl("/usr/bin/open", "open", (char *) url, NULL);
		exit(0);
	}
#else
	/* Try the open commands from freedesktop.org and the Gnome and
	 * KDE desktops first, then try some widely used browsers. 
	 */
	static const char	*browsers[] = { "xdg-open", "gnome-open", "kde-open",
						"firefox", "safari", "chrome", "opera", "mozilla", "netscape", "epiphany", "konqueror",
						NIL };

	String	 command;

	for (int i = 0; browsers[i] != NIL; i++)
	{
		if (i > 0) command.Append(" || ");

		command.Append(browsers[i]).Append(" \"").Append(url).Append("\"");
	}

	if (!fork())
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
		Buffer<wchar_t>	 buffer(32768);

		GetWindowsDirectoryW(buffer, buffer.Size());

		windowsDir = buffer;
	}
	else
	{
		Buffer<char>	 buffer(MAX_PATH);

		GetWindowsDirectoryA(buffer, buffer.Size());

		windowsDir = buffer;
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
		DWORD		 size = MAX_PATH;
		Buffer<char>	 buffer(size);

		RegQueryValueExA(currentVersion, String("ProgramFilesDir"), 0, NIL, (BYTE *) (char *) buffer, &size);

		programsDir = buffer;

		RegCloseKey(currentVersion);
	}

	if (programsDir == NIL)
	{
		/* Failed to get the program files directory from the registry.
		 * Get the directory name from the environment variable.
		 */
		if (Setup::enableUnicode)
		{
			Buffer<wchar_t>	 buffer(MAX_PATH);

			ExpandEnvironmentStringsW(String("%ProgramFiles%"), buffer, buffer.Size());

			programsDir = buffer;
		}
		else
		{
			Buffer<char>	 buffer(MAX_PATH);

			ExpandEnvironmentStringsA(String("%ProgramFiles%"), buffer, buffer.Size());

			programsDir = buffer;
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
		Buffer<wchar_t>	 buffer(MAX_PATH);

		SHGetPathFromIDListW(idlist, buffer);

		configDir = buffer;
	}
	else
	{
		Buffer<char>	 buffer(MAX_PATH);

		SHGetPathFromIDListA(idlist, buffer);

		configDir = buffer;
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
		Buffer<wchar_t>	 buffer(MAX_PATH);

		SHGetPathFromIDListW(idlist, buffer);

		personalDir = buffer;
	}
	else
	{
		Buffer<char>	 buffer(MAX_PATH);

		SHGetPathFromIDListA(idlist, buffer);

		personalDir = buffer;
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
	Buffer<char>	 buffer(MAX_PATH);

	GetTempPathA(buffer.Size(), buffer);

	tempDir = buffer;
#else
	tempDir = "/var/tmp";
#endif

	if (!tempDir.EndsWith(Directory::GetDirectoryDelimiter())) tempDir.Append(Directory::GetDirectoryDelimiter());
	if (tempDir == Directory::GetDirectoryDelimiter()) tempDir = NIL;

	return tempDir;
}
