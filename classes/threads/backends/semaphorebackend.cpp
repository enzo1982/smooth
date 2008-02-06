 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/semaphorebackend.h>

#if defined __WIN32__ && defined __SMOOTH_STATIC__
	#include <smooth/threads/backends/win32/semaphorewin32.h>
#endif

S::Threads::SemaphoreBackend *CreateSemaphoreBackend(S::Int iValue, S::Void *iSemaphore)
{
	return new S::Threads::SemaphoreBackend(iValue, iSemaphore);
}

S::Threads::SemaphoreBackend *(*S::Threads::SemaphoreBackend::backend_creator)(S::Int, S::Void *) = &CreateSemaphoreBackend;

S::Int S::Threads::SemaphoreBackend::SetBackend(SemaphoreBackend *(*backend)(Int, Void *))
{
	if (backend == NIL) return Error();

	backend_creator = backend;

	return Success();
}

S::Threads::SemaphoreBackend *S::Threads::SemaphoreBackend::CreateBackendInstance(Int iValue, Void *iSemaphore)
{
	return backend_creator(iValue, iSemaphore);
}

S::Threads::SemaphoreBackend::SemaphoreBackend(Int iValue, Void *iSemaphore)
{
#if defined __WIN32__ && defined __SMOOTH_STATIC__
	volatile Bool	 null = 0;

	if (null) SemaphoreWin32(iValue);
#endif

	type = SEMAPHORE_NONE;
}

S::Threads::SemaphoreBackend::~SemaphoreBackend()
{
}

S::Int S::Threads::SemaphoreBackend::GetSemaphoreType() const
{
	return type;
}

S::Void *S::Threads::SemaphoreBackend::GetSystemSemaphore() const
{
	return NIL;
}

S::Int S::Threads::SemaphoreBackend::Wait()
{
	return Error();
}

S::Int S::Threads::SemaphoreBackend::Release()
{
	return Error();
}
