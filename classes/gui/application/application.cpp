 /* The smooth Class Library
  * Copyright (C) 1998-2017 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/application/application.h>
#include <smooth/gui/window/window.h>
#include <smooth/system/event.h>
#include <smooth/system/screen.h>
#include <smooth/files/directory.h>
#include <smooth/foreach.h>

#if defined __WIN32__
#	include <windows.h>
#elif defined __HAIKU__
#	include <Application.h>
#	include <Roster.h>
#	include <Path.h>

#	include <stdlib.h>
#else
#	include <unistd.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <limits.h>

#	ifndef PATH_MAX
#		define PATH_MAX 32768
#	endif
#endif

const S::Short	 S::GUI::Application::classID = S::Object::RequestClassID();

S::String		 S::GUI::Application::command;
S::Array<S::String>	 S::GUI::Application::args;

S::String		 S::GUI::Application::startupDirectory;
S::String		 S::GUI::Application::applicationDirectory;

S::Float		 S::GUI::Application::scaleFactor = 0.0;

S::GUI::Application::Application(const String &name) : Widget(Point(0, 0), System::Screen::GetActiveScreenMetrics().GetSize())
{
	type	= classID;
	text	= name == NIL ? String("smooth Application") : name;

	Show();
}

S::GUI::Application::~Application()
{
}

S::Int S::GUI::Application::Loop()
{
	static Bool	 firstTime = True;

	if (firstTime)
	{
		firstTime = False;

		/* Show pending windows when we are called for the first time.
		 */
		for (Int i = 0; i < Window::GetNOfWindows(); i++)
		{
			Window	*window = Window::GetNthWindow(i);

			if (window != NIL && !window->IsVisibilitySet()) window->Show();
		}
	}

	/* Enter main application loop.
	 */
	System::EventProcessor	 event;

	while (GUI::Window::nOfActiveWindows > 0)
	{
		if (event.ProcessNextEvent() == Break) break;
	}

	return Success();
}

S::Void S::GUI::Application::SetArguments(const Array<String> &nArgs)
{
	foreach (const String &arg, nArgs)
	{
		/* Filter out scale factor argument.
		 */
		if (arg.StartsWith("--scale:")) { scaleFactor = arg.Tail(arg.Length() - 8).ToFloat(); continue; }

		/* Add to arguments array.
		 */
		args.Add(arg);
	}
}

S::String S::GUI::Application::GetStartupDirectory()
{
	if (startupDirectory != NIL) return startupDirectory;

#ifdef __WIN32__
	Buffer<wchar_t>	 buffer(32768 + 1);

	if (GetCurrentDirectory(buffer.Size(), buffer))	startupDirectory = buffer;

	if (!startupDirectory.EndsWith("\\")) startupDirectory.Append("\\");
#else
	Buffer<char>	 buffer(PATH_MAX + 1);

	if (getcwd(buffer, buffer.Size()) != NIL) startupDirectory = buffer;

	if (!startupDirectory.EndsWith("/")) startupDirectory.Append("/");
#endif

	return startupDirectory;
}

S::String S::GUI::Application::GetApplicationDirectory()
{
	if (applicationDirectory != NIL) return applicationDirectory;

#if defined __WIN32__
	Buffer<wchar_t>	 buffer(32768 + 1);
#else
	Buffer<char>	 buffer(PATH_MAX + 1);
#endif

	buffer.Zero();

#if defined __WIN32__
	/* In Windows, use GetModuleFileName to get the exe file name.
	 */
	GetModuleFileName(NIL, buffer, buffer.Size() - 1);

	applicationDirectory = buffer;
#elif defined __APPLE__
	/* In macOS, lsof -p <pid> always returns the path to the current binary in the first txt section.
	 */
	FILE	*pstdin = popen(String("lsof -p ").Append(String::FromInt(getpid())).Append(" | awk '$4 == \"txt\" { print substr($0, index($0, $9)) }'"), "r");

	fscanf(pstdin, String("%[^\n]").Append(String::FromInt(buffer.Size() - 1)), (char *) buffer);

	pclose(pstdin);

	applicationDirectory = buffer;
#elif defined __FreeBSD__
	/* In FreeBSD, procfs is not necessarily available, so check if it's there first.
	 */
	if (File(String("/proc/").Append(String::FromInt(getpid())).Append("/file")).Exists())
	{
		/* If procfs is available, /proc/<pid>/file links to the current binary.
		 */
		readlink(String("/proc/").Append(String::FromInt(getpid())).Append("/file"), buffer, buffer.Size() - 1);
	}
	else
	{
		/* Otherwise, procstat -b <pid> will provide the path to the current binary.
		 */
		FILE	*pstdin = popen(String("procstat -b ").Append(String::FromInt(getpid())).Append(" | awk '$1 == \"").Append(String::FromInt(getpid())).Append("\" { print $3 }'"), "r");

		fscanf(pstdin, String("%[^\n]").Append(String::FromInt(buffer.Size() - 1)), (char *) buffer);

		pclose(pstdin);
	}

	applicationDirectory = buffer;
#elif defined __sun
	/* In Solaris, /proc/<pid>/path/a.out links to the current binary.
	 */
	readlink(String("/proc/").Append(String::FromInt(getpid())).Append("/path/a.out"), buffer, buffer.Size() - 1);

	applicationDirectory = buffer;
#elif defined __linux__ || defined __NetBSD__
	/* In Linux and NetBSD, /proc/<pid>/exe links to the current binary.
	 */
	readlink(String("/proc/").Append(String::FromInt(getpid())).Append("/exe"), buffer, buffer.Size() - 1);

	applicationDirectory = buffer;
#elif defined __HAIKU__
	/* In Haiku, get the path from application info.
	 */
	app_info	 ai;

	if (be_app->GetAppInfo(&ai) == B_OK) applicationDirectory.ImportFrom("UTF-8", BPath(&ai.ref).Path());
#endif

	if (applicationDirectory == NIL)
	{
		/* No system specific way to get the current binary path.
		 * Try concatenating the startup directory and command.
		 */
		String	 binary	= (command.StartsWith("/") ? String() : GetStartupDirectory()).Append(command).Replace("/./", "/");

		if (!File(binary).Exists())
		{
			/* Search the path for command.
			 */
			String			 path  = getenv("PATH");
			const Array<String>	&paths = path.Explode(":");

			foreach (const String &path, paths)
			{
				/* Check for command in this path.
				 */
				if (File(String(path).Append("/").Append(command)).Exists())
				{
					binary = String(path).Append("/").Append(command);

					break;
				}
			}

			String::ExplodeFinish();
		}

		applicationDirectory = binary;
	}

	applicationDirectory[applicationDirectory.FindLast(Directory::GetDirectoryDelimiter()) + 1] = 0;

#if defined __APPLE__
	/* Change the returned path to Resources for macOS application bundles.
	 */
	if (applicationDirectory.EndsWith(".app/Contents/MacOS/")) applicationDirectory.Replace(".app/Contents/MacOS/", ".app/Contents/Resources/");
#endif

	return applicationDirectory;
}
