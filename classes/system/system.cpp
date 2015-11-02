 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifdef __APPLE__
#	include <CoreServices/CoreServices.h>
#	include <Carbon/Carbon.h>
#endif

#include <smooth/system/system.h>
#include <smooth/files/directory.h>
#include <smooth/io/instream.h>
#include <smooth/version.h>

#ifdef __WIN32__
#	include <smooth/backends/win32/backendwin32.h>

#	include <time.h>
#	include <shlobj.h>
#	include <shellapi.h>
#else
#	include <sys/time.h>
#	include <time.h>
#	include <errno.h>
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
	return clock() * UnsignedInt64(1000) / CLOCKS_PER_SEC;
#else
	timeval	 tv;

	gettimeofday(&tv, NIL);

	return tv.tv_sec * UnsignedInt64(1000) + tv.tv_usec / 1000;
#endif
}

S::Bool S::System::System::Sleep(UnsignedInt mSeconds)
{
#ifdef __WIN32__
	::Sleep(mSeconds);
#else
	timespec	 req;
	timespec	 rem;

	req.tv_sec  = mSeconds / 1000;
	req.tv_nsec = (mSeconds % 1000) * 1000000;

	while (nanosleep(&req, &rem) == -1)
	{
		if (errno != EINTR) break;

		req.tv_sec  = rem.tv_sec;
		req.tv_nsec = rem.tv_nsec;
	}
#endif

	return True;
}

S::Void S::System::System::Reboot()
{
#ifdef __WIN32__
	/* Acquire shutdown privilege.
	 */
	LUID			 value;
	TOKEN_PRIVILEGES	 token;
	HANDLE			 htoken;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &htoken);

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &value);

	token.PrivilegeCount = 1;
	token.Privileges[0].Luid = value;
	token.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(htoken, false, &token, 0, NULL, NULL);

	/* Reboot system.
	 */
	ExitWindowsEx(EWX_REBOOT | EWX_FORCEIFHUNG, 0);
#elif defined __APPLE__
	static const ProcessSerialNumber	 kPSNOfSystemProcess = { 0, kSystemProcess };

	/* Send restart message to system process.
	 */
	AEAddressDesc	 targetDesc;
	AppleEvent	 eventReply	  = { typeNull, NULL };
	AppleEvent	 appleEventToSend = { typeNull, NULL };

	AECreateDesc(typeProcessSerialNumber, &kPSNOfSystemProcess, sizeof(kPSNOfSystemProcess), &targetDesc);

	AECreateAppleEvent(kCoreEventClass, kAERestart, &targetDesc, kAutoGenerateReturnID, kAnyTransactionID, &appleEventToSend);
	AESend(&appleEventToSend, &eventReply, kAENoReply, kAENormalPriority, kAEDefaultTimeout, NULL, NULL);

	AEDisposeDesc(&eventReply);
	AEDisposeDesc(&appleEventToSend);
	AEDisposeDesc(&targetDesc);
#else
	/* Try rebooting using D-Bus messages to ConsoleKit and systemd-logind.
	 */
	if (!fork()) { execl("/bin/sh", "sh", "-c", "dbus-send --system --dest=\"org.freedesktop.ConsoleKit\" /org/freedesktop/ConsoleKit/Manager org.freedesktop.ConsoleKit.Manager.Restart", NULL); exit(0); }
	if (!fork()) { execl("/bin/sh", "sh", "-c", "dbus-send --system --dest=\"org.freedesktop.login1\" /org/freedesktop/login1 org.freedesktop.login1.Manager.Reboot boolean:true", NULL); exit(0); }

	/* Try rebooting using the shutdown command.
	 */
	if (!fork()) { execl("/bin/sh", "sh", "-c", "shutdown -r now", NULL); exit(0); }
#endif
}

S::Void S::System::System::Shutdown()
{
#if defined __WIN32__
	/* Acquire shutdown privilege.
	 */
	LUID			 value;
	TOKEN_PRIVILEGES	 token;
	HANDLE			 htoken;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &htoken);

	LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &value);

	token.PrivilegeCount = 1;
	token.Privileges[0].Luid = value;
	token.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(htoken, false, &token, 0, NULL, NULL);

	/* Shutdown system.
	 */
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCEIFHUNG, 0);
#elif defined __APPLE__
	static const ProcessSerialNumber	 kPSNOfSystemProcess = { 0, kSystemProcess };

	/* Send shutdown message to system process.
	 */
	AEAddressDesc	 targetDesc;
	AppleEvent	 eventReply	  = { typeNull, NULL };
	AppleEvent	 appleEventToSend = { typeNull, NULL };

	AECreateDesc(typeProcessSerialNumber, &kPSNOfSystemProcess, sizeof(kPSNOfSystemProcess), &targetDesc);

	AECreateAppleEvent(kCoreEventClass, kAEShutDown, &targetDesc, kAutoGenerateReturnID, kAnyTransactionID, &appleEventToSend);
	AESend(&appleEventToSend, &eventReply, kAENoReply, kAENormalPriority, kAEDefaultTimeout, NULL, NULL);

	AEDisposeDesc(&eventReply);
	AEDisposeDesc(&appleEventToSend);
	AEDisposeDesc(&targetDesc);
#else
	/* Try shutting down using D-Bus messages to ConsoleKit and systemd-logind.
	 */
	if (!fork()) { execl("/bin/sh", "sh", "-c", "dbus-send --system --dest=\"org.freedesktop.ConsoleKit\" /org/freedesktop/ConsoleKit/Manager org.freedesktop.ConsoleKit.Manager.Stop", NULL); exit(0); }
	if (!fork()) { execl("/bin/sh", "sh", "-c", "dbus-send --system --dest=\"org.freedesktop.login1\" /org/freedesktop/login1 org.freedesktop.login1.Manager.PowerOff boolean:true", NULL); exit(0); }

	/* Try shutting down using the shutdown command.
	 */
#	if defined __linux__ || defined __GNU__
		if (!fork()) { execl("/bin/sh", "sh", "-c", "shutdown -P now", NULL); exit(0); }
#	elif defined __FreeBSD__ || defined __NetBSD__ || defined __OpenBSD__
		if (!fork()) { execl("/bin/sh", "sh", "-c", "shutdown -p now", NULL); exit(0); }
#	else
		if (!fork()) { execl("/bin/sh", "sh", "-c", "shutdown -h now", NULL); exit(0); }
#	endif
#endif
}

S::Bool S::System::System::OpenURL(const String &url)
{
#if defined __WIN32__
	ShellExecute(NULL, (wchar_t *) L"open", url, NULL, NULL, 0);
#elif defined __APPLE__
	const char	*args = url;

	if (!fork()) { execl("/usr/bin/open", "open", args, NULL); exit(0); }
#elif defined __HAIKU__
	const char	*args = url;

	if (!fork()) { execl("/boot/system/bin/open", "open", args, NULL); exit(0); }
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

	const char	*cmd = command;

	if (!fork()) { execl("/bin/sh", "sh", "-c", cmd, NULL); exit(0); }
#endif

	return True;
}

S::String S::System::System::GetWindowsRootDirectory()
{
	String	 windowsDir;

#ifdef __WIN32__
	Buffer<wchar_t>	 buffer(32768 + 1);

	GetWindowsDirectory(buffer, buffer.Size());

	windowsDir = buffer;
#endif

	if (!windowsDir.EndsWith(Directory::GetDirectoryDelimiter())) windowsDir.Append(Directory::GetDirectoryDelimiter());
	if ( windowsDir == Directory::GetDirectoryDelimiter())	      windowsDir = NIL;

	return windowsDir;
}

S::String S::System::System::GetProgramFilesDirectory()
{
	String	 programsDir;

#ifdef __WIN32__
	HKEY	 currentVersion;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion", 0, KEY_QUERY_VALUE, &currentVersion) == ERROR_SUCCESS)
	{
		DWORD		 size = 32768 + 1;
		Buffer<char>	 buffer(size);

		RegQueryValueEx(currentVersion, L"ProgramFilesDir", 0, NIL, (BYTE *) (char *) buffer, &size);

		programsDir = buffer;

		RegCloseKey(currentVersion);
	}

	if (programsDir == NIL)
	{
		/* Failed to get the program files directory from the registry.
		 * Get the directory name from the environment variable.
		 */
		Buffer<wchar_t>	 buffer(32768 + 1);

		ExpandEnvironmentStrings(String("%ProgramFiles%"), buffer, buffer.Size());

		programsDir = buffer;
	}
#endif

	if (!programsDir.EndsWith(Directory::GetDirectoryDelimiter())) programsDir.Append(Directory::GetDirectoryDelimiter());
	if ( programsDir == Directory::GetDirectoryDelimiter())	       programsDir = NIL;

	return programsDir;
}

S::String S::System::System::GetApplicationDataDirectory()
{
	String	 configDir;

#if defined __WIN32__
	ITEMIDLIST	*idlist;
	Buffer<wchar_t>	 buffer(32768 + 1);

	SHGetSpecialFolderLocation(NIL, CSIDL_APPDATA, &idlist);
	SHGetPathFromIDList(idlist, buffer);

	configDir = buffer;

	CoTaskMemFree(idlist);
#elif defined __HAIKU__
	FILE		*pstdin = popen("finddir B_USER_SETTINGS_DIRECTORY", "r");
	Buffer<char>	 buffer(PATH_MAX + 1);

	buffer.Zero();

	fscanf(pstdin, String("%[^\n]").Append(String::FromInt(buffer.Size() - 1)), (char *) buffer);

	pclose(pstdin);

	configDir = buffer;
#else
	passwd	*pw = getpwuid(getuid());

	if (pw != NIL)	configDir = pw->pw_dir;
	else		configDir = "~";
#endif

	if (!configDir.EndsWith(Directory::GetDirectoryDelimiter())) configDir.Append(Directory::GetDirectoryDelimiter());
	if ( configDir == Directory::GetDirectoryDelimiter())	     configDir = NIL;

	return configDir;
}

S::String S::System::System::GetPersonalFilesDirectory(PersonalFilesType type)
{
	String	 personalDir;

#ifdef __WIN32__
	ITEMIDLIST	*idlist;
	Buffer<wchar_t>	 buffer(32768 + 1);

	switch (type)
	{
		default:
		case PersonalFilesGeneric:
			SHGetSpecialFolderLocation(NIL, CSIDL_PERSONAL, &idlist);
			break;
		case PersonalFilesDocuments:
			SHGetSpecialFolderLocation(NIL, CSIDL_PERSONAL, &idlist);
			break;
		case PersonalFilesPictures:
			if (Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 5)   ) SHGetSpecialFolderLocation(NIL, CSIDL_MYPICTURES, &idlist);
			else										  SHGetSpecialFolderLocation(NIL, CSIDL_PERSONAL, &idlist);
			break;
		case PersonalFilesMusic:
			if (Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 5)   ) SHGetSpecialFolderLocation(NIL, CSIDL_MYMUSIC, &idlist);
			else										  SHGetSpecialFolderLocation(NIL, CSIDL_PERSONAL, &idlist);
			break;
		case PersonalFilesMovies:
			if (Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 5, 1)) SHGetSpecialFolderLocation(NIL, CSIDL_MYVIDEO, &idlist);
			else										  SHGetSpecialFolderLocation(NIL, CSIDL_PERSONAL, &idlist);
			break;
		case PersonalFilesDownloads:
			SHGetSpecialFolderLocation(NIL, CSIDL_PERSONAL, &idlist);
			break;
	}

	SHGetPathFromIDList(idlist, buffer);

	personalDir = buffer;

	CoTaskMemFree(idlist);

	if (personalDir == NIL) personalDir = "C:";
#else
	passwd	*pw = getpwuid(getuid());

	if (pw != NIL)	personalDir = pw->pw_dir;
	else		personalDir = "~";

#ifdef __APPLE__
	OSErr	 error = -1;
	FSRef	 entry;

	switch (type)
	{
		default:
		case PersonalFilesGeneric:
			break;
		case PersonalFilesDocuments:
			error = FSFindFolder(kUserDomain, kDocumentsFolderType, kDontCreateFolder, &entry);
			break;
		case PersonalFilesPictures:
			error = FSFindFolder(kUserDomain, kPictureDocumentsFolderType, kDontCreateFolder, &entry);
			break;
		case PersonalFilesMusic:
			error = FSFindFolder(kUserDomain, kMusicDocumentsFolderType, kDontCreateFolder, &entry);
			break;
		case PersonalFilesMovies:
			error = FSFindFolder(kUserDomain, kMovieDocumentsFolderType, kDontCreateFolder, &entry);
			break;
		case PersonalFilesDownloads:
			break;
	}

	if (error == noErr)
	{
		CFURLRef	 url = CFURLCreateFromFSRef(kCFAllocatorSystemDefault, &entry);

		if (url != NIL)
		{
			CFStringRef	 path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);

			if (path != NIL)
			{
				Buffer<char>	 buffer(CFStringGetLength(path) * 4 + 1);

				CFStringGetCString(path, buffer, buffer.Size(), kCFStringEncodingUTF8);

				personalDir.ImportFrom("UTF-8", buffer);

				CFRelease(path);
			}

			CFRelease(url);
		}
	}
#else
	if (File(String(personalDir).Append("/.config/user-dirs.dirs")).Exists())
	{
		String		 format = String::SetInputFormat("UTF-8");
		IO::InStream	 in(IO::STREAM_FILE, String(personalDir).Append("/.config/user-dirs.dirs"), IO::IS_READ);

		while (in.GetPos() < in.Size())
		{
			String	 entry = in.InputLine();

			if ((type == PersonalFilesDocuments && entry.StartsWith("XDG_DOCUMENTS_DIR")) ||
			    (type == PersonalFilesPictures  && entry.StartsWith("XDG_PICTURES_DIR" )) ||
			    (type == PersonalFilesMusic	    && entry.StartsWith("XDG_MUSIC_DIR"	   )) ||
			    (type == PersonalFilesMovies    && entry.StartsWith("XDG_VIDEOS_DIR"   )) ||
			    (type == PersonalFilesDownloads && entry.StartsWith("XDG_DOWNLOAD_DIR" )))
			{
				personalDir = entry.SubString(entry.Find("\"") + 1, entry.FindLast("\"") - entry.Find("\"") - 1).Replace("$HOME", personalDir);

				break;
			}
		}

		String::SetInputFormat(format);
	}
#endif
#endif

	if (personalDir != NIL && !personalDir.EndsWith(Directory::GetDirectoryDelimiter())) personalDir.Append(Directory::GetDirectoryDelimiter());

	return personalDir;
}

S::String S::System::System::GetTempDirectory()
{
	String	 tempDir;

#if defined __WIN32__
	Buffer<wchar_t>	 buffer(32768 + 1);

	GetTempPath(buffer.Size(), buffer);

	tempDir = buffer;
#elif defined __HAIKU__
	FILE		*pstdin = popen("finddir B_COMMON_TEMP_DIRECTORY", "r");
	Buffer<char>	 buffer(PATH_MAX + 1);

	buffer.Zero();

	fscanf(pstdin, String("%[^\n]").Append(String::FromInt(buffer.Size() - 1)), (char *) buffer);

	pclose(pstdin);

	tempDir = buffer;
#else
	tempDir = "/var/tmp";
#endif

	if (!tempDir.EndsWith(Directory::GetDirectoryDelimiter())) tempDir.Append(Directory::GetDirectoryDelimiter());
	if ( tempDir == Directory::GetDirectoryDelimiter())	   tempDir = NIL;

	return tempDir;
}
