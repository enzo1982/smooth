 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/mutex.h>
#include <smooth/system/system.h>

S::Threads::RWLock::RWLock()
{
	readLocked  = 0;
	writeLocked = 0;

	exclusiveAccessMutex = new Mutex();
	sharedAccessMutex    = new Mutex();
}

S::Threads::RWLock::RWLock(const RWLock &oRWLock)
{
	*this = oRWLock;
}

S::Threads::RWLock::~RWLock()
{
	delete exclusiveAccessMutex;
	delete sharedAccessMutex;
}

S::Threads::RWLock &S::Threads::RWLock::operator =(const RWLock &oRWLock)
{
	if (&oRWLock == this) return *this;

	readLocked  = 0;
	writeLocked = 0;

	exclusiveAccessMutex = new Mutex();
	sharedAccessMutex    = new Mutex();

	return *this;
}

S::Bool S::Threads::RWLock::LockForRead()
{
	/* Acquire exclusive lock.
	 */
	exclusiveAccessMutex->Lock();

	/* Increase read lock counter by one.
	 */
	sharedAccessMutex->Lock();

	readLocked++;

	sharedAccessMutex->Release();

	/* Allow other read and write locks.
	 */
	exclusiveAccessMutex->Release();

	return True;
}

S::Bool S::Threads::RWLock::LockForWrite()
{
	/* Acquire exclusive lock.
	 */
	exclusiveAccessMutex->Lock();

	/* Wait for read operations to finish.
	 */
	while (readLocked)
	{
		exclusiveAccessMutex->Release();

		S::System::System::Sleep(0);

		exclusiveAccessMutex->Lock();
	}

	/* Increase write lock counter.
	 */
	writeLocked++;

	return True;
}

S::Bool S::Threads::RWLock::Release()
{
	/* Check if we are locked for write.
	 */
	if (writeLocked && !readLocked)
	{
		/* Decrease write lock counter.
		 */
		writeLocked--;

		/* Allow new read and write locks again.
		 */
		exclusiveAccessMutex->Release();

		return True;
	}

	/* Decrease read lock counter by one.
	 */
	sharedAccessMutex->Lock();

	readLocked--;

	sharedAccessMutex->Release();

	return True;
}
