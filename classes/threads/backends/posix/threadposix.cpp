 /* The smooth Class Library
  * Copyright (C) 1998-2016 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/posix/threadposix.h>

S::Threads::ThreadBackend *CreateThreadPOSIX(S::Void *iThread)
{
	return new S::Threads::ThreadPOSIX(iThread);
}

S::Int	 threadPOSIXTmp = S::Threads::ThreadBackend::SetBackend(&CreateThreadPOSIX);

S::Threads::ThreadPOSIX::ThreadPOSIX(Void *iThread)
{
	type	 = THREAD_POSIX;

	thread	 = NIL;
	myThread = False;

	if (iThread != NIL) thread = (pthread_t *) iThread;
}

S::Threads::ThreadPOSIX::~ThreadPOSIX()
{
	if (myThread) Stop();
}

S::Void *S::Threads::ThreadPOSIX::GetSystemThread() const
{
	return (Void *) thread;
}

S::Int S::Threads::ThreadPOSIX::GetThreadID() const
{
	return thread - (pthread_t *) NIL;
}

S::Bool S::Threads::ThreadPOSIX::IsCurrentThread() const
{
	if (thread == NIL) return False;

	return pthread_equal(pthread_self(), *thread);
}

S::Int S::Threads::ThreadPOSIX::Start(Void (*threadProc)(Void *), Void *threadParam)
{
	Stop();

	info.threadProc	 = threadProc;
	info.threadParam = threadParam;

	thread	 = new pthread_t;
	myThread = True;

	pthread_create(thread, NULL, Caller, &info);

	return Success();
}

S::Int S::Threads::ThreadPOSIX::Stop()
{
	if (thread == NIL) return Error();

	pthread_cancel(*thread);
	pthread_detach(*thread);

	if (myThread) delete thread;

	thread = NIL;

	return Success();
}

S::Int S::Threads::ThreadPOSIX::Wait()
{
	if (thread == NIL) return Error();

	pthread_join(*thread, NIL);

	if (myThread) delete thread;

	thread = NIL;

	return Success();
}

S::Void S::Threads::ThreadPOSIX::Exit()
{
	if (!IsCurrentThread()) return;

	pthread_detach(*thread);

	delete thread;

	thread = NIL;
}

void *S::Threads::ThreadPOSIX::Caller(void *param)
{
	ThreadInfo	*info = (ThreadInfo *) param;

	info->threadProc(info->threadParam);

	return NULL;
}
