 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/mutexbackend.h>

#if defined __WIN32__ && defined __SMOOTH_STATIC__
	#include <smooth/threads/backends/win32/mutexwin32.h>
#endif

S::Threads::MutexBackend *CreateMutexBackend(S::Void *iMutex)
{
	return new S::Threads::MutexBackend(iMutex);
}

S::Threads::MutexBackend *(*S::Threads::MutexBackend::backend_creator)(S::Void *) = &CreateMutexBackend;

S::Int S::Threads::MutexBackend::SetBackend(MutexBackend *(*backend)(Void *))
{
	if (backend == NIL) return Error();

	backend_creator = backend;

	return Success();
}

S::Threads::MutexBackend *S::Threads::MutexBackend::CreateBackendInstance(Void *iMutex)
{
	return backend_creator(iMutex);
}

S::Threads::MutexBackend::MutexBackend(Void *iMutex)
{
#if defined __WIN32__ && defined __SMOOTH_STATIC__
	volatile Bool	 null = 0;

	if (null) MutexWin32();
#endif

	type = MUTEX_NONE;
}

S::Threads::MutexBackend::~MutexBackend()
{
}

S::Int S::Threads::MutexBackend::GetMutexType() const
{
	return type;
}

S::Void *S::Threads::MutexBackend::GetSystemMutex() const
{
	return NIL;
}

S::Int S::Threads::MutexBackend::Lock()
{
	return Error();
}

S::Int S::Threads::MutexBackend::Release()
{
	return Error();
}
