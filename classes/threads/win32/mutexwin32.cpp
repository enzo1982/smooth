 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/win32/mutexwin32.h>

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
		mutex	= CreateMutexA(NULL, false, NULL);
		myMutex	= True;
	}
}

S::Threads::MutexWin32::~MutexWin32()
{
	if (myMutex) CloseHandle(mutex);
}

S::Void *S::Threads::MutexWin32::GetSystemMutex()
{
	return (Void *) mutex;
}

S::Int S::Threads::MutexWin32::Lock()
{
	WaitForSingleObject(mutex, INFINITE);

	return Success;
}

S::Int S::Threads::MutexWin32::Release()
{
	ReleaseMutex(mutex);

	return Success;
}
