 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/object.h>
#include <smooth/thread.h>
#include <smooth/loop.h>
#include <smooth/threadmanager.h>
#include <smooth/background.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>

S::Void ThreadProcCaller(S::Thread *);

const S::Int	 S::Thread::classID = S::Object::RequestClassID();
S::Int		 S::Thread::counter = 0;

S::Thread::Thread()
{
	type		= classID;
	status		= THREAD_CREATED;
	thread		= NIL;

	possibleContainers.AddEntry(Application::classID);

	mainThreadManager->RegisterThread(this);
}

S::Thread::~Thread()
{
	if (status == THREAD_CREATED || status == THREAD_STARTME)
	{
		status = THREAD_STOPPED;
	}

	if (status == THREAD_STOPPED_SELF)
	{
		status = THREAD_STOPPED;

		LiSAThreadCloseHandle(thread);
	}

	if (status != THREAD_STOPPED)
	{
		status = THREAD_STOPPED;
		counter--;

		LiSAThreadCancel(thread);
		LiSAThreadCloseHandle(thread);
	}

	mainThreadManager->UnregisterThread(this);
}

S::Int S::Thread::GetStatus()
{
	return status;
}

S::Int S::Thread::GetThreadID()
{
	return threadID;
}

S::Int S::Thread::Start()
{
	if ((status == THREAD_CREATED && !initializing) || status == THREAD_STARTME || (flags & THREAD_WAITFLAG_START))
	{
		thread = LiSAThreadCreate((unsigned long *) &threadID, (void (*)(void *)) ThreadProcCaller, this);

		status = THREAD_RUNNING;
		counter++;

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

S::Int S::Thread::Stop()
{
	if (status == THREAD_RUNNING || status == THREAD_PAUSED)
	{
		if (LiSAThreadSelf() == thread)
		{
			status = THREAD_STOPPED_SELF;
			counter--;

			LiSAThreadExit();

			return Success;
		}

		status = THREAD_STOPPED;
		counter--;

		LiSAThreadCancel(thread);
		LiSAThreadCloseHandle(thread);

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Void ThreadProcCaller(S::Thread *thread)
{
	thread->threadMain.Call(thread);
}
