 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/semaphore.h>
#include <smooth/threads/backends/semaphorebackend.h>

const S::Int	 S::Threads::Semaphore::classID = S::Object::RequestClassID();

S::Threads::Semaphore::Semaphore(Void *iSemaphore)
{
	backend = SemaphoreBackend::CreateBackendInstance(iSemaphore);

	type = classID;
}

S::Threads::Semaphore::~Semaphore()
{
	delete backend;
}

S::Int S::Threads::Semaphore::GetSemaphoreType()
{
	return backend->GetSemaphoreType();
}

S::Void *S::Threads::Semaphore::GetSystemSemaphore()
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
