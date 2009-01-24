 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/threads/mutex.h>
#include <smooth/threads/semaphore.h>
#include <smooth/system/system.h>

const S::Int	 S::Threads::RWLock::MAX_READ_LOCKS = 16;

S::Threads::RWLock::RWLock()
{
	writeLocked = False;
	readLocks = 0;

	exclusiveAccessMutex	= new Mutex();
	sharedAccessSemaphore	= new Semaphore(MAX_READ_LOCKS);
}

S::Threads::RWLock::RWLock(const RWLock &oRWLock)
{
	*this = oRWLock;
}

S::Threads::RWLock::~RWLock()
{
	delete exclusiveAccessMutex;
	delete sharedAccessSemaphore;
}

S::Threads::RWLock &S::Threads::RWLock::operator =(const RWLock &oRWLock)
{
	writeLocked = False;
	readLocks = 0;

	exclusiveAccessMutex	= new Mutex();
	sharedAccessSemaphore	= new Semaphore(MAX_READ_LOCKS);

	return *this;
}

S::Int S::Threads::RWLock::LockForRead()
{
	/* Increase shared access counter by one.
	 */
	exclusiveAccessMutex->Lock();

	sharedAccessSemaphore->Wait();
	readLocks++;

	exclusiveAccessMutex->Release();

	return Success();
}

S::Int S::Threads::RWLock::LockForWrite()
{
	/* Acquire exclusive lock.
	 */
	exclusiveAccessMutex->Lock();

	/* Wait for read operations to finish.
	 */
	while (readLocks > 0)
	{
		System::System::Sleep(0);
	}

	/* Mark ourself locked for write.
	 */
	writeLocked = True;

	return Success();
}

S::Int S::Threads::RWLock::Release()
{
	/* Check if we are locked for write.
	 */
	if (writeLocked)
	{
		/* Release write lock.
		 */
		writeLocked = False;

		return exclusiveAccessMutex->Release();
	}

	/* Decrease shared access counter by one.
	 */
	exclusiveAccessMutex->Lock();

	readLocks--;
	sharedAccessSemaphore->Release();

	exclusiveAccessMutex->Release();

	return Success();
}
