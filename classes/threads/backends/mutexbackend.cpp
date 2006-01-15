 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

S::Int	 mutexBackendTmp = S::Threads::MutexBackend::AddBackend(&CreateMutexBackend);

S::Array<S::Threads::MutexBackend *(*)(S::Void *), S::Void *>	*S::Threads::MutexBackend::backend_creators = NIL;

S::Int S::Threads::MutexBackend::AddBackend(MutexBackend *(*backend)(Void *))
{
	if (backend == NIL) return Error();

	if (backend_creators == NIL) backend_creators = new Array<MutexBackend *(*)(Void *), Void *>;

	backend_creators->AddEntry(backend);

	return Success();
}

S::Threads::MutexBackend *S::Threads::MutexBackend::CreateBackendInstance(Void *iMutex)
{
	if (backend_creators->GetFirstEntry() != &CreateMutexBackend)	return backend_creators->GetFirstEntry()(iMutex);
	else								return backend_creators->GetLastEntry()(iMutex);
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

S::Int S::Threads::MutexBackend::GetMutexType()
{
	return type;
}

S::Void *S::Threads::MutexBackend::GetSystemMutex()
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
