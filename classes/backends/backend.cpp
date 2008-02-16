 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/backend.h>

#if defined __WIN32__ && defined __SMOOTH_STATIC__
	#include <smooth/backends/win32/backendwin32.h>
#endif

S::Array<S::Backends::Backend *, S::Void *>		*S::Backends::Backend::backends = NIL;
S::Array<S::Backends::Backend *(*)(), S::Void *>	*S::Backends::Backend::backend_creators = NIL;

S::Int S::Backends::Backend::AddBackend(Backend *(*backend)())
{
	if (backend == NIL) return Error();

	if (backend_creators == NIL) backend_creators = new Array<Backend *(*)(), Void *>;

	backend_creators->Add(backend);

	return Success();
}

S::Int S::Backends::Backend::GetNOfBackends()
{
	return backends->Length();
}

S::Backends::Backend *S::Backends::Backend::GetNthBackend(Int n)
{
	return backends->GetNth(n);
}

S::Int S::Backends::Backend::InitBackends()
{
	if (backends == NIL)		backends = new Array<Backend *, Void *>;
	if (backend_creators == NIL)	backend_creators = new Array<Backend *(*)(), Void *>;

	for (Int i = 0; i < backend_creators->Length(); i++)
	{
		backends->Add(backend_creators->GetNth(i)());
	}

	for (Int j = 0; j < backends->Length(); j++)
	{
		backends->GetNth(j)->Init();
	}

	return Success();
}

S::Int S::Backends::Backend::DeinitBackends()
{
	for (Int i = 0; i < backends->Length(); i++)
	{
		backends->GetNth(i)->Deinit();

		delete backends->GetNth(i);
	}

	backends->RemoveAll();

	delete backends;
	delete backend_creators;

	return Success();
}

S::Backends::Backend::Backend()
{
#if defined __WIN32__ && defined __SMOOTH_STATIC__
	volatile Bool	 null = 0;

	if (null) BackendWin32();
#endif

	type = BACKEND_NONE;
}

S::Backends::Backend::~Backend()
{
}

S::Int S::Backends::Backend::Init()
{
	return Success();
}

S::Int S::Backends::Backend::Deinit()
{
	return Success();
}

S::Int S::Backends::Backend::GetBackendType() const
{
	return type;
}
