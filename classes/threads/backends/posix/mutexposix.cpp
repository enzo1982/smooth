 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/posix/mutexposix.h>

S::Threads::MutexBackend *CreateMutexPOSIX(S::Void *iMutex)
{
	return new S::Threads::MutexPOSIX(iMutex);
}

S::Int	 mutexPOSIXTmp = S::Threads::MutexBackend::SetBackend(&CreateMutexPOSIX);

S::Threads::MutexPOSIX::MutexPOSIX(Void *iMutex)
{
	type = MUTEX_POSIX;

	if (iMutex != NIL)
	{
		attributes = NIL;
		mutex	   = (pthread_mutex_t *) iMutex;
		myMutex	   = False;
	}
	else
	{
		/* The mutex will be created once we need it
		 */
		attributes = NIL;
		mutex	   = NIL;
		myMutex	   = True;
	}
}

S::Threads::MutexPOSIX::~MutexPOSIX()
{
	if (myMutex && mutex != NIL)
	{
		pthread_mutex_destroy(mutex);

		delete mutex;

		pthread_mutexattr_destroy(attributes);

		delete attributes;
	}
}

S::Void *S::Threads::MutexPOSIX::GetSystemMutex() const
{
	return (Void *) mutex;
}

S::Int S::Threads::MutexPOSIX::Lock()
{
	/* Lazy initialization of the mutex happens here
	 */
	if (mutex == NIL)
	{
		attributes = new pthread_mutexattr_t;

		pthread_mutexattr_init(attributes);
		pthread_mutexattr_settype(attributes, PTHREAD_MUTEX_RECURSIVE);

		mutex = new pthread_mutex_t;

		pthread_mutex_init(mutex, attributes);
	}

	pthread_mutex_lock(mutex);

	return Success();
}

S::Int S::Threads::MutexPOSIX::Release()
{
	if (mutex != NIL) pthread_mutex_unlock(mutex);

	return Success();
}
