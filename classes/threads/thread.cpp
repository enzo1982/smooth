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

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_THREAD = S::Object::RequestObjectID();
S::Int	 S::Thread::counter = 0;

S::Thread::Thread(ThreadProcParam, Void *procParam)
{
	type		= OBJ_THREAD;
	threadProc	= (ThreadProcType) newProc;
	altproc		= NIL;
	instance	= procParam;
	status		= THREAD_CREATED;
	thread		= NIL;
	killflag	= THREAD_KILLFLAG_KILL;
	waitflag	= THREAD_WAITFLAG_WAIT;

	possibleContainers.AddEntry(OBJ_APPLICATION);

	mainThreadManager->RegisterThread(this);
}

S::Thread::Thread(Void (*proc)(Thread *))
{
	type		= OBJ_THREAD;
	threadProc	= NIL;
	altproc		= proc;
	instance	= NIL;
	status		= THREAD_CREATED;
	thread		= NIL;
	killflag	= THREAD_KILLFLAG_KILL;
	waitflag	= THREAD_WAITFLAG_WAIT;

	possibleContainers.AddEntry(OBJ_APPLICATION);

	mainThreadManager->RegisterThread(this);
}

S::Thread::~Thread()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);

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

S::Int S::Thread::Start()
{
	if (!registered) return Error;

	if ((status == THREAD_CREATED && !initializing) || status == THREAD_STARTME || waitflag == THREAD_WAITFLAG_START)
	{
		if (threadProc != NIL)		thread = LiSAThreadCreate((void (*)(void *)) ThreadProcCaller, this);
		else if (altproc != NIL)	thread = LiSAThreadCreate((void (*)(void *)) altproc, this);

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
	if (!registered) return Error;

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

S::Void S::Thread::SetWaitFlag(Int wf)
{
	waitflag = wf;
}

S::Int S::Thread::GetWaitFlag()
{
	return waitflag;
}

S::Void S::Thread::SetKillFlag(Int kf)
{
	killflag = kf;
}

S::Int S::Thread::GetKillFlag()
{
	return killflag;
}

S::Void S::ThreadProcCaller(Thread *thread)
{
	ThreadProcCall(thread->threadProc, thread->instance, thread);
}
