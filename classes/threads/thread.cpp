 /* The smooth Class Library
  * Copyright (C) 1998-2021 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/thread.h>
#include <smooth/threads/backends/threadbackend.h>
#include <smooth/templates/threadlocal.h>
#include <smooth/init.h>

#include <stdint.h>

namespace smooth
{
	static multithread (intptr_t)	 threadID = Threads::MainThreadID;
}

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
	if (status == THREAD_CREATED || status == THREAD_STARTME)
	{
		status = THREAD_STOPPED;
	}

	if (status != THREAD_STOPPED)
	{
		status = THREAD_STOPPED;

		Access::Decrement(nOfRunningThreads);

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

S::UnsignedInt32 S::Threads::Thread::GetThreadID() const
{
	return GetHandle();
}

S::UnsignedInt32 S::Threads::Thread::GetCurrentThreadID()
{
	return threadID;
}

S::Bool S::Threads::Thread::IsCurrentThread() const
{
	if (GetCurrentThreadID() == GetThreadID()) return True;

	return False;
}

S::Int S::Threads::Thread::Start()
{
	if ((status == THREAD_CREATED || status == THREAD_STARTME) && !initializing)
	{
		Access::Increment(nOfRunningThreads);

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
	if (backend->Stop() == Success())
	{
		status = THREAD_STOPPED;

		Access::Decrement(nOfRunningThreads);

		return Success();
	}

	return Error();
}

S::Int S::Threads::Thread::Wait()
{
	return backend->Wait();
}

S::Void S::Threads::Thread::MainCaller(Thread *thread)
{
	/* Set thread ID and call thread callback.
	 */
	threadID = thread->GetHandle();

	thread->threadMain.Call(thread);

	/* Clean up and exit thread.
	 */
	String::DeleteTemporaryBuffers(True);

	thread->status = THREAD_STOPPED;

	Access::Decrement(nOfRunningThreads);

	thread->backend->Exit();
}
