 /* The smooth Class Library
  * Copyright (C) 1998-2016 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/win32/semaphorewin32.h>

S::Threads::SemaphoreBackend *CreateSemaphoreWin32(S::Int iValue, S::Void *iSemaphore)
{
	return new S::Threads::SemaphoreWin32(iValue, iSemaphore);
}

S::Int	 semaphoreWin32Tmp = S::Threads::SemaphoreBackend::SetBackend(&CreateSemaphoreWin32);

S::Threads::SemaphoreWin32::SemaphoreWin32(Int iValue, Void *iSemaphore) : SemaphoreBackend(iValue)
{
	type = SEMAPHORE_WIN32;

	if (iSemaphore != NIL)
	{
		semaphore   = (HANDLE) iSemaphore;
		mySemaphore = False;
	}
	else
	{
		semaphore   = CreateSemaphore(NULL, iValue, iValue, NULL);
		mySemaphore = True;
	}
}

S::Threads::SemaphoreWin32::~SemaphoreWin32()
{
	if (mySemaphore && semaphore != NIL) CloseHandle(semaphore);
}

S::Void *S::Threads::SemaphoreWin32::GetSystemSemaphore() const
{
	return (Void *) semaphore;
}

S::Bool S::Threads::SemaphoreWin32::Wait()
{
	if (semaphore != NIL && WaitForSingleObject(semaphore, INFINITE) == WAIT_OBJECT_0) return True;

	return False;
}

S::Bool S::Threads::SemaphoreWin32::TryWait()
{
	if (semaphore != NIL && WaitForSingleObject(semaphore, 0) == WAIT_OBJECT_0) return True;

	return False;
}

S::Bool S::Threads::SemaphoreWin32::Release()
{
	if (semaphore != NIL && ReleaseSemaphore(semaphore, 1, NULL) == 0) return True;

	return False;
}
