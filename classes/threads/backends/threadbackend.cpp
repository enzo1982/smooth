 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/threadbackend.h>

#if defined __WIN32__ && defined SMOOTH_STATIC
	#include <smooth/threads/backends/win32/threadwin32.h>
#endif

S::Threads::ThreadBackend *CreateThreadBackend(S::Void *iThread)
{
	return new S::Threads::ThreadBackend(iThread);
}

S::Threads::ThreadBackend *(*S::Threads::ThreadBackend::backend_creator)(S::Void *) = &CreateThreadBackend;

S::Int S::Threads::ThreadBackend::SetBackend(ThreadBackend *(*backend)(Void *))
{
	if (backend == NIL) return Error();

	backend_creator = backend;

	return Success();
}

S::Threads::ThreadBackend *S::Threads::ThreadBackend::CreateBackendInstance(Void *iThread)
{
	return backend_creator(iThread);
}

S::Threads::ThreadBackend::ThreadBackend(Void *iThread)
{
#if defined __WIN32__ && defined SMOOTH_STATIC
	volatile Bool	 null = 0;

	if (null) ThreadWin32();
#endif

	type = THREAD_NONE;
}

S::Threads::ThreadBackend::~ThreadBackend()
{
}

S::Short S::Threads::ThreadBackend::GetThreadType() const
{
	return type;
}

S::Int S::Threads::ThreadBackend::Start(Void (*threadProc)(Void *), Void *threadParam)
{
	return Error();
}

S::Int S::Threads::ThreadBackend::Stop()
{
	return Error();
}

S::Int S::Threads::ThreadBackend::Wait()
{
	return Error();
}

S::Void S::Threads::ThreadBackend::Exit()
{
}
