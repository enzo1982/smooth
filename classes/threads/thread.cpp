 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_THREAD_
#define __OBJSMOOTH_THREAD_

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

SMOOTHInt	 OBJ_THREAD = SMOOTH::RequestObjectID();
SMOOTHInt	 SMOOTHThread::counter = 0;

SMOOTHThread::SMOOTHThread(SMOOTHThreadProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_THREAD;
	threadProc			= (SMOOTHThreadProcType) newProc;
	altproc				= NIL;
	objectProperties->procParam	= procParam;
	status				= THREAD_CREATED;
	thread				= NIL;
	killflag			= THREAD_KILLFLAG_KILL;
	waitflag			= THREAD_WAITFLAG_WAIT;

	possibleContainers.AddEntry(OBJ_APPLICATION);

	mainThreadManager->RegisterThread(this);
}

SMOOTHThread::SMOOTHThread(SMOOTHVoid (*proc)(SMOOTHThread *))
{
	type				= OBJ_THREAD;
	threadProc			= NIL;
	altproc				= proc;
	objectProperties->procParam	= NIL;
	status				= THREAD_CREATED;
	thread				= NIL;
	killflag			= THREAD_KILLFLAG_KILL;
	waitflag			= THREAD_WAITFLAG_WAIT;

	possibleContainers.AddEntry(OBJ_APPLICATION);

	mainThreadManager->RegisterThread(this);
}

SMOOTHThread::~SMOOTHThread()
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

SMOOTHInt SMOOTHThread::GetStatus()
{
	return status;
}

SMOOTHInt SMOOTHThread::Start()
{
	if (!registered) return SMOOTH::Error;

	if ((status == THREAD_CREATED && !initializing) || status == THREAD_STARTME || waitflag == THREAD_WAITFLAG_START)
	{
		if (threadProc != NIL)		thread = LiSAThreadCreate((void (*)(void *)) SMOOTHThreadProcCaller, this);
		else if (altproc != NIL)	thread = LiSAThreadCreate((void (*)(void *)) altproc, this);

		status = THREAD_RUNNING;
		counter++;

		return SMOOTH::Success;
	}
	else if (status == THREAD_CREATED && initializing)
	{
		status = THREAD_STARTME;

		return SMOOTH::Success;
	}
	else
	{
		return SMOOTH::Error;
	}
}

SMOOTHInt SMOOTHThread::Stop()
{
	if (!registered) return SMOOTH::Error;

	if (status == THREAD_RUNNING || status == THREAD_PAUSED)
	{
		if (LiSAThreadSelf() == thread)
		{
			status = THREAD_STOPPED_SELF;
			counter--;

			LiSAThreadExit();

			return SMOOTH::Success;
		}

		status = THREAD_STOPPED;
		counter--;

		LiSAThreadCancel(thread);
		LiSAThreadCloseHandle(thread);

		return SMOOTH::Success;
	}
	else
	{
		return SMOOTH::Error;
	}
}

SMOOTHVoid SMOOTHThread::SetWaitFlag(SMOOTHInt wf)
{
	waitflag = wf;
}

SMOOTHInt SMOOTHThread::GetWaitFlag()
{
	return waitflag;
}

SMOOTHVoid SMOOTHThread::SetKillFlag(SMOOTHInt kf)
{
	killflag = kf;
}

SMOOTHInt SMOOTHThread::GetKillFlag()
{
	return killflag;
}

SMOOTHVoid SMOOTHThreadProcCaller(SMOOTHThread *thread)
{
	SMOOTHThreadProcCall(thread->threadProc, thread->GetObjectProperties()->procParam, thread);
}

#endif
