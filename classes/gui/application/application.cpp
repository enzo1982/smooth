 /* The smooth Class Library
  * Copyright (C) 1998-2011 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/application/application.h>
#include <smooth/gui/window/window.h>
#include <smooth/threads/thread.h>
#include <smooth/system/event.h>
#include <smooth/system/multimonitor.h>
#include <smooth/files/directory.h>
#include <smooth/init.h>
#include <smooth/templates/nonblocking.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <unistd.h>
#	include <stdio.h>
#	include <limits.h>
#endif

S::Bool	 S::loopActive	= S::False;

const S::Short	 S::GUI::Application::classID = S::Object::RequestClassID();

S::String	 S::GUI::Application::startupDirectory;
S::String	 S::GUI::Application::applicationDirectory;

S::GUI::Application::Application(const String &name) : Widget(Point(0, 0), System::MultiMonitor::GetActiveMonitorMetrics().GetSize())
{
	type	= classID;
	text	= name == NIL ? String("smooth Application") : name;

	Show();
}

S::Int S::GUI::Application::Loop()
{
	if (!loopActive)
	{
		initializing = false;
		loopActive = true;

		for (Int i = 0; i < Window::GetNOfWindows(); i++)
		{
			Window	*window = Window::GetNthWindow(i);

			if (window != NIL)
			{
				if (!window->initshow) window->Show();
			}
		}

		/* Start waiting threads here.
		 */
		for (Int j = 0; j < Object::GetNOfObjects(); j++)
		{
			Object	*object = Object::GetNthObject(j);

			if (object != NIL)
			{
				if (object->GetObjectType() == Threads::Thread::classID)
				{
					if (((Threads::Thread *) object)->GetStatus() == Threads::THREAD_STARTME) ((Threads::Thread *) object)->Start();
				}
			}
		}
	}

#ifdef __WIN32__
	if (GUI::Window::nOfActiveWindows == 0) PostQuitMessage(0);
#endif

	System::EventProcessor	*event = new System::EventProcessor();
	Bool			 quit = False;

	while (!quit)
	{
		Int	 result = event->ProcessNextEvent();

		if (result == Break)			quit = True;
		if (GUI::Window::nOfActiveWindows == 0)	quit = True;
	}

	delete event;

	loopActive = false;

	for (int i = 0; i < Object::GetNOfObjects(); i++)
	{
		Object	*object = Object::GetNthObject(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == Threads::Thread::classID)
			{
				if (!(((Threads::Thread *) object)->GetFlags() & Threads::THREAD_KILLFLAG_WAIT)) ((Threads::Thread *) object)->Stop();
			}
		}
	}

	/* Wait for started threads to finish.
	 */
	while (Threads::Thread::GetNOfRunningThreads() > 0) System::System::Sleep(10);

	return Success();
}

S::String S::GUI::Application::GetStartupDirectory()
{
	if (startupDirectory != NIL) return startupDirectory;

#ifdef __WIN32__
	Int	 length = 0;

	if (Setup::enableUnicode)
	{
		Buffer<wchar_t>	 buffer(32768);

		length = GetCurrentDirectoryW(buffer.Size(), buffer);

		startupDirectory = buffer;
	}

	if (!Setup::enableUnicode || length == 0)
	{
		Buffer<char>	 buffer(MAX_PATH);

		GetCurrentDirectoryA(buffer.Size(), buffer);

		startupDirectory = buffer;
	}

	if (!startupDirectory.EndsWith("\\")) startupDirectory.Append("\\");
#else
	Buffer<char>	 buffer(PATH_MAX);

	if (getcwd(buffer, buffer.Size()) != NIL)
	{
		startupDirectory = buffer;
	}

	if (!startupDirectory.EndsWith("/")) startupDirectory.Append("/");
#endif

	return startupDirectory;
}

S::String S::GUI::Application::GetApplicationDirectory()
{
	if (applicationDirectory != NIL) return applicationDirectory;

#if defined __WIN32__
	Int	 length = 0;

	if (Setup::enableUnicode)
	{
		Buffer<wchar_t>	 buffer(32768 + 1);

		buffer.Zero();

		length = GetModuleFileNameW(NIL, buffer, buffer.Size() - 1);

		applicationDirectory = buffer;
	}

	if (!Setup::enableUnicode || length == 0)
	{
		Buffer<char>	 buffer(MAX_PATH + 1);

		buffer.Zero();

		GetModuleFileNameA(NIL, buffer, buffer.Size() - 1);

		applicationDirectory = buffer;
	}
#else
	Buffer<char>	 buffer(PATH_MAX + 1);

	buffer.Zero();

#if defined __APPLE__
	FILE	*stdin = popen(String("lsof -p ").Append(String::FromInt(getpid())).Append(" | awk '$4 == \"txt\" { print $9 }'"), "r");

	fscanf(stdin, String("%[^\n]").Append(String::FromInt(buffer.Size() - 1)), (char *) buffer);

	pclose(stdin);
#elif defined __linux__
	readlink(String("/proc/").Append(String::FromInt(getpid())).Append("/exe"), buffer, buffer.Size() - 1);
#elif defined __sun
	readlink(String("/proc/").Append(String::FromInt(getpid())).Append("/path/a.out"), buffer, buffer.Size() - 1);
#else
	readlink(String("/proc/").Append(String::FromInt(getpid())).Append("/file"), buffer, buffer.Size() - 1);
#endif

	applicationDirectory = buffer;
#endif

	applicationDirectory[applicationDirectory.FindLast(Directory::GetDirectoryDelimiter()) + 1] = 0;

#if defined __APPLE__
	/* Change the returned path to Resources for Mac OS X application bundles.
	 */
	if (applicationDirectory.EndsWith(".app/Contents/MacOS/")) applicationDirectory.Replace(".app/Contents/MacOS/", ".app/Contents/Resources/");
#endif

	return applicationDirectory;
}
