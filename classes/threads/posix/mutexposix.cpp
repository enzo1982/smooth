 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/posix/mutexposix.h>

S::Threads::MutexPOSIX::MutexPOSIX(Void *iMutex)
{
	type = MUTEX_POSIX;

	if (iMutex != NIL)
	{
		mutex	= (pthread_mutex_t *) iMutex;
		myMutex	= False;
	}
	else
	{
		mutex	= new pthread_mutex_t;

		pthread_mutex_init(mutex, NULL);

		myMutex	= True;
	}
}

S::Threads::MutexPOSIX::~MutexPOSIX()
{
	if (myMutex)
	{
		pthread_mutex_destroy(mutex);

		delete mutex;
	}
}

S::Void *S::Threads::MutexPOSIX::GetSystemMutex()
{
	return (Void *) mutex;
}

S::Int S::Threads::MutexPOSIX::Lock()
{
	pthread_mutex_lock(mutex);

	return Success;
}

S::Int S::Threads::MutexPOSIX::Release()
{
	pthread_mutex_unlock(mutex);

	return Success;
}
