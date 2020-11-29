 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
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
		SemaphorePOSIX	*oSemaphore = (SemaphorePOSIX *) iSemaphore;

		mutex	    = oSemaphore->mutex;
		condition   = oSemaphore->condition;

		count	    = oSemaphore->count;
		max	    = oSemaphore->max;

		mySemaphore = False;
	}
	else
	{
		mutex	    = new pthread_mutex_t;
		condition   = new pthread_cond_t;

		count	    = iValue;
		max	    = iValue;

		mySemaphore = True;

		if (pthread_mutex_init(mutex, NIL) != 0)
		{
			delete mutex;
			delete condition;

			mutex	  = NIL;
			condition = NIL;
		}
		else if (pthread_cond_init(condition, NIL) != 0)
		{
			pthread_mutex_destroy(mutex);

			delete mutex;
			delete condition;

			mutex	  = NIL;
			condition = NIL;
		}
	}
}

S::Threads::SemaphorePOSIX::~SemaphorePOSIX()
{
	if (mySemaphore && mutex != NIL)
	{
		pthread_mutex_destroy(mutex);
		pthread_cond_destroy(condition);

		delete mutex;
		delete condition;
	}
}

S::Void *S::Threads::SemaphorePOSIX::GetSystemSemaphore() const
{
	return (Void *) this;
}

S::Bool S::Threads::SemaphorePOSIX::Wait()
{
	if (mutex == NIL) return False;

	pthread_mutex_lock(mutex);

	while (count <= 0) pthread_cond_wait(condition, mutex);

	count -= 1;

	pthread_mutex_unlock(mutex);

	return True;
}

S::Bool S::Threads::SemaphorePOSIX::TryWait()
{
	if (mutex == NIL) return False;

	pthread_mutex_lock(mutex);

	if (count > 0)
	{
		count -= 1;

		pthread_mutex_unlock(mutex);

		return True;
	}

	pthread_mutex_unlock(mutex);

	return False;
}

S::Bool S::Threads::SemaphorePOSIX::Release()
{
	if (mutex == NIL) return False;

	pthread_mutex_lock(mutex);

	if (count < max)
	{
		count += 1;

		pthread_cond_signal(condition);
		pthread_mutex_unlock(mutex);

		return True;
	}

	pthread_mutex_unlock(mutex);

	return False;
}
