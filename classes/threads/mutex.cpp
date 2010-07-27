 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/string.h>
#include <smooth/threads/mutex.h>
#include <smooth/threads/thread.h>
#include <smooth/threads/backends/mutexbackend.h>

S::Threads::Mutex::Mutex(Void *iMutex)
{
	backend = MutexBackend::CreateBackendInstance(iMutex);
}

S::Threads::Mutex::Mutex(const Mutex &oMutex)
{
	*this = oMutex;
}

S::Threads::Mutex::~Mutex()
{
	delete backend;
}

S::Threads::Mutex &S::Threads::Mutex::operator =(const Mutex &oMutex)
{
	if (&oMutex == this) return *this;

	backend = MutexBackend::CreateBackendInstance(NIL);

	return *this;
}

S::Int S::Threads::Mutex::GetMutexType() const
{
	return backend->GetMutexType();
}

S::Void *S::Threads::Mutex::GetSystemMutex() const
{
	return backend->GetSystemMutex();
}

S::Int S::Threads::Mutex::Lock()
{
	/* Lock the mutex only if we are running multiple Threads
	 */
	if (Threads::Thread::GetNOfRunningThreads() > 0)
	{
		return backend->Lock();
	}

	return Success();
}

S::Int S::Threads::Mutex::Release()
{
	/* This is save even if we didn't call Lock() before
	 */
	return backend->Release();
}
