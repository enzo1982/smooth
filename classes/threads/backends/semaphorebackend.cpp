 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/threads/backends/semaphorebackend.h>

S::Threads::SemaphoreBackend *CreateSemaphoreBackend(S::Void *iSemaphore)
{
	return new S::Threads::SemaphoreBackend(iSemaphore);
}

S::Int	 semaphoreBackendTmp = S::Threads::SemaphoreBackend::AddBackend(&CreateSemaphoreBackend);

S::Array<S::Threads::SemaphoreBackend *(*)(S::Void *), S::Void *>	*S::Threads::SemaphoreBackend::backend_creators = NIL;

S::Int S::Threads::SemaphoreBackend::AddBackend(SemaphoreBackend *(*backend)(Void *))
{
	if (backend == NIL) return Error();

	if (backend_creators == NIL) backend_creators = new Array<SemaphoreBackend *(*)(Void *), Void *>;

	backend_creators->AddEntry(backend);

	return Success();
}

S::Threads::SemaphoreBackend *S::Threads::SemaphoreBackend::CreateBackendInstance(Void *iSemaphore)
{
	if (backend_creators->GetFirstEntry() != &CreateSemaphoreBackend)	return backend_creators->GetFirstEntry()(iSemaphore);
	else									return backend_creators->GetLastEntry()(iSemaphore);
}

S::Threads::SemaphoreBackend::SemaphoreBackend(Void *iSemaphore)
{
	type = SEMAPHORE_NONE;
}

S::Threads::SemaphoreBackend::~SemaphoreBackend()
{
}

S::Int S::Threads::SemaphoreBackend::GetSemaphoreType()
{
	return type;
}

S::Void *S::Threads::SemaphoreBackend::GetSystemSemaphore()
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
