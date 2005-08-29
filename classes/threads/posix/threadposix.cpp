 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/posix/threadposix.h>

S::Threads::ThreadBackend *CreateThreadPOSIX(S::Void *iThread)
{
	return new S::Threads::ThreadPOSIX(iThread);
}

S::Int	 threadPOSIXTmp = S::Threads::ThreadBackend::AddBackend(&CreateThreadPOSIX);

S::Threads::ThreadPOSIX::ThreadPOSIX(Void *iThread)
{
	type = THREAD_POSIX;

	if (iThread != NIL)
	{
		thread		= (pthread_t *) iThread;
		myThread	= False;
	}
	else
	{
		thread		= NIL;
		myThread	= True;
	}
}

S::Threads::ThreadPOSIX::~ThreadPOSIX()
{
	if (myThread) Stop();
}

S::Void *S::Threads::ThreadPOSIX::GetSystemThread()
{
	return (Void *) thread;
}

S::Int S::Threads::ThreadPOSIX::GetThreadID()
{
	return thread - (pthread_t *) NIL;
}

S::Int S::Threads::ThreadPOSIX::Start(Void (*threadProc)(Void *), Void *threadParam)
{
	Stop();

	thread = new pthread_t;

	pthread_create(thread, NULL, (void *(*)(void *)) threadProc, threadParam);

	return Success;
}

S::Int S::Threads::ThreadPOSIX::Stop()
{
	if (thread == NIL) return Failure;

	pthread_cancel(*thread);

	if (myThread)
	{
		delete thread;

		myThread = False;
	}

	thread = NIL;
	
	return Success;
}

S::Void S::Threads::ThreadPOSIX::Exit()
{
	if (thread == NIL) return;

	delete thread;

	thread = NIL;

	pthread_exit(0);
}

S::Void *S::Threads::ThreadPOSIX::Self()
{
	pthread_t	 *thisThread = NULL;
	
	*thisThread = pthread_self();

	return thisThread;
}
