 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/thread.h>
#include <smooth/threads/backends/threadbackend.h>
#include <smooth/init.h>

const S::Short	 S::Threads::Thread::classID = S::Object::RequestClassID();

S::Short	 S::Threads::Thread::nOfRunningThreads = 0;

S::Threads::Thread::Thread(Void *iThread)
{
	backend = ThreadBackend::CreateBackendInstance(iThread);

	type	= classID;
	status	= THREAD_CREATED;
}

S::Threads::Thread::Thread(const Thread &oThread)
{
	*this = oThread;
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

		nOfRunningThreads--;

		backend->Stop();
	}

	delete backend;
}

S::Threads::Thread &S::Threads::Thread::operator =(const Thread &oThread)
{
	if (&oThread == this) return *this;

	backend = ThreadBackend::CreateBackendInstance(NIL);

	type	= classID;
	status	= THREAD_CREATED;

	threadMain = oThread.threadMain;

	return *this;
}

S::Short S::Threads::Thread::GetStatus() const
{
	return status;
}

S::Int S::Threads::Thread::GetThreadID() const
{
	return backend->GetThreadID();
}

S::Int S::Threads::Thread::Start()
{
	if ((status == THREAD_CREATED && !initializing) || status == THREAD_STARTME || (flags & THREAD_WAITFLAG_START))
	{
		nOfRunningThreads++;

		status = THREAD_RUNNING;

		backend->Start((Void (*)(Void *)) MainCaller, this);

		return Success();
	}
	else if (status == THREAD_CREATED && initializing)
	{
		status = THREAD_STARTME;

		return Success();
	}

	return Error();
}

S::Int S::Threads::Thread::Stop()
{
	if (status == THREAD_RUNNING)
	{
		if (backend->Self() == backend->GetSystemThread())
		{
			status = THREAD_STOPPED_SELF;

			nOfRunningThreads--;

			backend->Exit();

			return Success();
		}

		status = THREAD_STOPPED;

		nOfRunningThreads--;

		backend->Stop();

		return Success();
	}

	return Error();
}

S::Void S::Threads::Thread::MainCaller(Thread *thread)
{
	thread->threadMain.Call(thread);
	thread->status = THREAD_STOPPED;

	nOfRunningThreads--;

	thread->backend->Exit();
}
