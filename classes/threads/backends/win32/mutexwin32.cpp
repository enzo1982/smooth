 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/win32/mutexwin32.h>

S::Threads::MutexBackend *CreateMutexWin32(S::Void *iMutex)
{
	return new S::Threads::MutexWin32(iMutex);
}

S::Int	 mutexWin32Tmp = S::Threads::MutexBackend::SetBackend(&CreateMutexWin32);

S::Threads::MutexWin32::MutexWin32(Void *iMutex)
{
	type = MUTEX_WIN32;

	if (iMutex != NIL)
	{
		mutex	= (HANDLE) iMutex;
		myMutex	= False;
	}
	else
	{
		/* The mutex will be created once we need it
		 */
		mutex	= NIL;
		myMutex	= True;
	}
}

S::Threads::MutexWin32::~MutexWin32()
{
	if (myMutex && mutex != NIL) CloseHandle(mutex);
}

S::Void *S::Threads::MutexWin32::GetSystemMutex() const
{
	return (Void *) mutex;
}

S::Int S::Threads::MutexWin32::Lock()
{
	/* Lazy initialization of the mutex happens here
	 */
	if (mutex == NIL) mutex	= CreateMutexA(NULL, false, NULL);

	WaitForSingleObject(mutex, INFINITE);

	return Success();
}

S::Int S::Threads::MutexWin32::Release()
{
	if (mutex != NIL) ReleaseMutex(mutex);

	return Success();
}
