 /* The smooth Class Library
  * Copyright (C) 1998-2016 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/posix/semaphoreposix.h>

S::Threads::SemaphoreBackend *CreateSemaphorePOSIX(S::Int iValue, S::Void *iSemaphore)
{
	return new S::Threads::SemaphorePOSIX(iValue, iSemaphore);
}

S::Int	 semaphorePOSIXTmp = S::Threads::SemaphoreBackend::SetBackend(&CreateSemaphorePOSIX);

S::Threads::SemaphorePOSIX::SemaphorePOSIX(Int iValue, Void *iSemaphore) : SemaphoreBackend(iValue)
{
	type = SEMAPHORE_POSIX;

	if (iSemaphore != NIL)
	{
		semaphore   = (sem_t *) iSemaphore;
		mySemaphore = False;
	}
	else
	{
		semaphore   = new sem_t;
		mySemaphore = True;

		if (sem_init(semaphore, 0, iValue) == -1)
		{
			delete semaphore;

			semaphore = NIL;
		}
	}
}

S::Threads::SemaphorePOSIX::~SemaphorePOSIX()
{
	if (mySemaphore && semaphore != NIL)
	{
		sem_destroy(semaphore);

		delete semaphore;
	}
}

S::Void *S::Threads::SemaphorePOSIX::GetSystemSemaphore() const
{
	return (Void *) semaphore;
}

S::Bool S::Threads::SemaphorePOSIX::Wait()
{
	if (semaphore != NIL && sem_wait(semaphore) == 0) return True;

	return False;
}

S::Bool S::Threads::SemaphorePOSIX::TryWait()
{
	if (semaphore != NIL && sem_trywait(semaphore) == 0) return True;

	return False;
}

S::Bool S::Threads::SemaphorePOSIX::Release()
{
	if (semaphore != NIL && sem_post(semaphore) == 0) return True;

	return False;
}
