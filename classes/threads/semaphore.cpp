 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/semaphore.h>
#include <smooth/threads/backends/semaphorebackend.h>

S::Threads::Semaphore::Semaphore(Int iValue, Void *iSemaphore)
{
	backend = SemaphoreBackend::CreateBackendInstance(iValue, iSemaphore);

	max = iValue;
}

S::Threads::Semaphore::Semaphore(const Semaphore &oSemaphore)
{
	*this = oSemaphore;
}

S::Threads::Semaphore::~Semaphore()
{
	delete backend;
}

S::Threads::Semaphore &S::Threads::Semaphore::operator =(const Semaphore &oSemaphore)
{
	if (&oSemaphore == this) return *this;

	backend = SemaphoreBackend::CreateBackendInstance(oSemaphore.max, NIL);

	max = oSemaphore.max;

	return *this;
}

S::Int S::Threads::Semaphore::GetSemaphoreType() const
{
	return backend->GetSemaphoreType();
}

S::Void *S::Threads::Semaphore::GetSystemSemaphore() const
{
	return backend->GetSystemSemaphore();
}

S::Int S::Threads::Semaphore::Wait()
{
	return backend->Wait();
}

S::Int S::Threads::Semaphore::Release()
{
	return backend->Release();
}
