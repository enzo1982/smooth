 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/posix/semaphoreposix.h>

S::Threads::SemaphoreBackend *CreateSemaphorePOSIX(S::Void *iSemaphore)
{
	return new S::Threads::SemaphorePOSIX(iSemaphore);
}

S::Int	 semaphorePOSIXTmp = S::Threads::SemaphoreBackend::AddBackend(&CreateSemaphorePOSIX);

S::Threads::SemaphorePOSIX::SemaphorePOSIX(Void *iSemaphore)
{
	type = SEMAPHORE_POSIX;

	if (iSemaphore != NIL)
	{
		semaphore	= (sem_t *) iSemaphore;
		mySemaphore	= False;
	}
	else
	{
		semaphore	= new sem_t;

		sem_init(semaphore, 0, 1);

		mySemaphore	= True;
	}
}

S::Threads::SemaphorePOSIX::~SemaphorePOSIX()
{
	if (mySemaphore)
	{
		sem_destroy(semaphore);

		delete semaphore;
	}
}

S::Void *S::Threads::SemaphorePOSIX::GetSystemSemaphore() const
{
	return (Void *) semaphore;
}

S::Int S::Threads::SemaphorePOSIX::Wait()
{
	sem_wait(semaphore);

	return Success();
}

S::Int S::Threads::SemaphorePOSIX::Release()
{
	sem_post(semaphore);

	return Success();
}
