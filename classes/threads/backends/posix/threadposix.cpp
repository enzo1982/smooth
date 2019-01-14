 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
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
	type		 = THREAD_POSIX;

	thread		 = NIL;
	myThread	 = False;

	info.threadProc	 = NIL;
	info.threadParam = NIL;

	if (iThread != NIL) thread = (pthread_t *) iThread;
}

S::Threads::ThreadPOSIX::~ThreadPOSIX()
{
	if (myThread) Stop();
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

	pthread_t	*thread	  = this->thread;
	Bool		 myThread = this->myThread;

	this->thread = NIL;

	pthread_cancel(*thread);
	pthread_detach(*thread);

	if (myThread) delete thread;

	return Success();
}

S::Int S::Threads::ThreadPOSIX::Wait()
{
	if (thread == NIL) return Error();

	pthread_t	*thread	  = this->thread;
	Bool		 myThread = this->myThread;

	this->thread = NIL;

	pthread_join(*thread, NIL);

	if (myThread) delete thread;

	return Success();
}

S::Void S::Threads::ThreadPOSIX::Exit()
{
	if (!this->thread || !pthread_equal(pthread_self(), *this->thread)) return;

	pthread_t	*thread	  = this->thread;
	Bool		 myThread = this->myThread;

	this->thread = NIL;

	pthread_detach(*thread);

	if (myThread) delete thread;
}

void *S::Threads::ThreadPOSIX::Caller(void *param)
{
	ThreadInfo	*info = (ThreadInfo *) param;

	info->threadProc(info->threadParam);

	return NULL;
}
