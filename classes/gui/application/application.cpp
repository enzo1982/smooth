 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/init.h>
#include <smooth/templates/nonblocking.h>

#ifndef __WIN32__
#	include <unistd.h>
#	include <stdio.h>
#endif

S::Bool	 S::loopActive	= S::False;

const S::Short	 S::GUI::Application::classID = S::Object::RequestClassID();

S::String	 S::GUI::Application::startupDirectory;
S::String	 S::GUI::Application::applicationDirectory;

S::GUI::Application::Application(const String &name) : Widget(Point(0, 0), Size(LiSAGetDisplaySizeX(), LiSAGetDisplaySizeY()))
{
	type	= classID;

	Show();

	text	= name == NIL ? String("smooth Application") : name;
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
		wchar_t	*buffer = new wchar_t [MAX_PATH];

		length = GetCurrentDirectoryW(MAX_PATH, buffer);

		startupDirectory = buffer;

		delete [] buffer;
	}

	if (!Setup::enableUnicode || length == 0)
	{
		char	*buffer = new char [MAX_PATH];

		GetCurrentDirectoryA(MAX_PATH, buffer);

		startupDirectory = buffer;

		delete [] buffer;
	}

	if (!startupDirectory.EndsWith("\\")) startupDirectory.Append("\\");
#else
	char	*buffer = new char [MAX_PATH];

	if (getcwd(buffer, MAX_PATH) != NIL)
	{
		startupDirectory = buffer;
	}

	delete [] buffer;

	if (!startupDirectory.EndsWith("/")) startupDirectory.Append("/");
#endif

	return startupDirectory;
}

S::String S::GUI::Application::GetApplicationDirectory()
{
	if (applicationDirectory != NIL) return applicationDirectory;

#ifdef __WIN32__
	Int	 length = 0;

	if (Setup::enableUnicode)
	{
		wchar_t	*buffer = new wchar_t [MAX_PATH];

		length = GetModuleFileNameW(NIL, buffer, MAX_PATH);

		applicationDirectory = buffer;

		delete [] buffer;
	}

	if (!Setup::enableUnicode || length == 0)
	{
		char	*buffer = new char [MAX_PATH];

		GetModuleFileNameA(NIL, buffer, MAX_PATH);

		applicationDirectory = buffer;

		delete [] buffer;
	}

	applicationDirectory[applicationDirectory.FindLast("\\") + 1] = 0;
#else
	char	 szTmp[32];

	sprintf(szTmp, "/proc/%d/exe", getpid());

	char	*buffer = new char [MAX_PATH];
	int	 bytes = readlink(szTmp, buffer, MAX_PATH);

	if (bytes >= 0) buffer[bytes] = '\0';

	applicationDirectory = buffer;

	delete [] buffer;

	applicationDirectory[applicationDirectory.FindLast("/") + 1] = 0;
#endif

	return applicationDirectory;
}
