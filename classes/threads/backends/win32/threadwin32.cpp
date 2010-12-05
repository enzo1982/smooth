 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/win32/threadwin32.h>

S::Threads::ThreadBackend *CreateThreadWin32(S::Void *iThread)
{
	return new S::Threads::ThreadWin32(iThread);
}

S::Int	 threadWin32Tmp = S::Threads::ThreadBackend::SetBackend(&CreateThreadWin32);

S::Threads::ThreadWin32::ThreadWin32(Void *iThread)
{
	type = THREAD_WIN32;

	if (iThread != NIL)
	{
		thread		= (HANDLE) iThread;
		threadID	= -1;
		myThread	= False;
	}
	else
	{
		thread		= NIL;
		threadID	= -1;
		myThread	= True;
	}
}

S::Threads::ThreadWin32::~ThreadWin32()
{
	if (myThread) Stop();
}

S::Void *S::Threads::ThreadWin32::GetSystemThread() const
{
	return (Void *) threadID;
}

S::Int S::Threads::ThreadWin32::GetThreadID() const
{
	return threadID;
}

S::Bool S::Threads::ThreadWin32::IsCurrentThread() const
{
	return (threadID == GetCurrentThreadId());
}

S::Int S::Threads::ThreadWin32::Start(Void (*threadProc)(Void *), Void *threadParam)
{
	Stop();

	thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) threadProc, threadParam, 0, (DWORD *) &threadID);

	return Success();
}

S::Int S::Threads::ThreadWin32::Stop()
{
	if (thread == NIL) return Error();

	HANDLE	 self = thread;
	Bool	 running = IsRunning();

	thread = NIL;

	if (running) TerminateThread(self, 0);

	if (myThread)
	{
		CloseHandle(self);

		myThread = False;
	}

	return Success();
}

S::Bool S::Threads::ThreadWin32::IsRunning() const
{
	DWORD	 exitCode = 0;

	GetExitCodeThread(thread, &exitCode);

	return (exitCode == STILL_ACTIVE);
}

S::Void S::Threads::ThreadWin32::Exit()
{
	if (thread == NIL) return;

	CloseHandle(thread);

	thread = NIL;

	ExitThread(0);
}
