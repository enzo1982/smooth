 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/object.h>
#include <smooth/threads/thread.h>
#include <smooth/loop.h>
#include <smooth/gui/application/background.h>
#include <smooth/objectproperties.h>

#ifdef __WIN32__
#include <smooth/threads/win32/threadwin32.h>
#else
#include <smooth/threads/posix/threadposix.h>
#endif

S::Void ThreadProcCaller(S::Threads::Thread *);

const S::Int	 S::Threads::Thread::classID = S::Object::RequestClassID();

S::Threads::Thread::Thread(Void *iThread)
{
#ifdef __WIN32__
	backend = new ThreadWin32(iThread);
#else
	backend = new ThreadPOSIX(iThread);
#endif

	type	= classID;
	status	= THREAD_CREATED;

	possibleContainers.AddEntry(GUI::Application::classID);
}

S::Threads::Thread::~Thread()
{
	if (status == THREAD_CREATED || status == THREAD_STARTME || status == THREAD_STOPPED_SELF)
	{
		status = THREAD_STOPPED;
	}

	if (status != THREAD_STOPPED)
	{
		status = THREAD_STOPPED;

		backend->Stop();
	}

	delete backend;
}

S::Int S::Threads::Thread::GetStatus()
{
	return status;
}

S::Int S::Threads::Thread::GetThreadID()
{
	return backend->GetThreadID();
}

S::Int S::Threads::Thread::Start()
{
	if ((status == THREAD_CREATED && !initializing) || status == THREAD_STARTME || (flags & THREAD_WAITFLAG_START))
	{
		backend->Start((Void (*)(Void *)) ThreadProcCaller, this);

		status = THREAD_RUNNING;

		return Success;
	}
	else if (status == THREAD_CREATED && initializing)
	{
		status = THREAD_STARTME;

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::Threads::Thread::Stop()
{
	if (status == THREAD_RUNNING)
	{
		if (backend->Self() == backend->GetSystemThread())
		{
			status = THREAD_STOPPED_SELF;

			backend->Exit();

			return Success;
		}

		status = THREAD_STOPPED;

		backend->Stop();

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::Threads::Thread::GetNOfRunningThreads()
{
	Int	 n = 0;

	for (Int i = 0; i < Object::GetNOfObjects(); i++)
	{
		Object	*object = Object::GetNthObject(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == classID)
			{
				if (((Thread *) object)->GetStatus() == THREAD_RUNNING) n++;
			}
		}
	}

	return n;
}

S::Void ThreadProcCaller(S::Threads::Thread *thread)
{
	thread->threadMain.Call(thread);

	thread->Stop();
}
