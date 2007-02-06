 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/i18n/i18n.h>
#include <smooth/system/event.h>
#include <smooth/init.h>

S::Bool	 S::loopActive		= S::False;
S::Int	 S::peekLoop		= 0;

const S::Int	 S::GUI::Application::classID = S::Object::RequestClassID();

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

		for (Int i = 0; i < Object::GetNOfObjects(); i++)
		{
			Object	*object = Object::GetNthObject(i);

			if (object != NIL)
			{
				if (object->GetObjectType() == GUI::Window::classID)
				{
					if (!((GUI::Window *) object)->initshow) ((GUI::Window *) object)->Show();
				}
			}
		}

		// start waiting threads here

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
		Int	 result = Success();

		if (peekLoop > 0)	result = event->ProcessNextEvent(False);
		else			result = event->ProcessNextEvent(True);

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

	/* This is the same as NonBlocking::WaitForRunningCalls(),
	   so we don't need to call that.			   */

	while (Threads::Thread::GetNOfRunningThreads() > 0) LiSASleep(10);

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

	Int	 len = startupDirectory.Length() - 1;

	if (startupDirectory[len] != '\\') startupDirectory[++len] = '\\';
#endif

	return startupDirectory;
}

S::String S::GUI::Application::GetApplicationDirectory()
{
	if (applicationDirectory != NIL) return applicationDirectory;

#ifdef __WIN32__
	String	 path;
	Int	 lastBs = 0;
	Int	 length = 0;

	if (Setup::enableUnicode)
	{
		wchar_t	*buffer = new wchar_t [MAX_PATH];

		length = GetModuleFileNameW(NIL, buffer, MAX_PATH);

		path = buffer;

		delete [] buffer;
	}

	if (!Setup::enableUnicode || length == 0)
	{
		char	*buffer = new char [MAX_PATH];

		GetModuleFileNameA(NIL, buffer, MAX_PATH);

		path = buffer;

		delete [] buffer;
	}

	for (Int i = 0; i < path.Length(); i++) if (path[i] == '\\') lastBs = i;

	path[lastBs + 1] = 0;

	applicationDirectory = path;
#endif

	return applicationDirectory;
}
