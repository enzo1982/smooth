 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/backends/backend.h>

S::Array<S::Backends::Backend *>	*S::Backends::Backend::backends = NIL;
S::Array<S::Backends::Backend *(*)()>	*S::Backends::Backend::backend_creators = NIL;

S::Int S::Backends::Backend::AddBackend(Backend *(*backend)())
{
	if (backend == NIL) return Error;

	if (backend_creators == NIL) backend_creators = new Array<Backend *(*)()>;

	backend_creators->AddEntry(backend);

	return Success;
}

S::Int S::Backends::Backend::InitBackends()
{
	if (backends == NIL)		backends = new Array<Backend *>;
	if (backend_creators == NIL)	backend_creators = new Array<Backend *(*)()>;

	for (Int i = 0; i < backend_creators->GetNOfEntries(); i++)
	{
		backends->AddEntry(backend_creators->GetNthEntry(i)());
	}

	for (Int j = 0; j < backends->GetNOfEntries(); j++)
	{
		backends->GetNthEntry(j)->Init();
	}

	return Success;
}

S::Int S::Backends::Backend::DeinitBackends()
{
	for (Int i = 0; i < backends->GetNOfEntries(); i++)
	{
		backends->GetNthEntry(i)->Deinit();

		delete backends->GetNthEntry(i);
	}

	backends->RemoveAll();

	delete backends;
	delete backend_creators;

	return Success;
}

S::Int S::Backends::Backend::Init()
{
	return Success;
}

S::Int S::Backends::Backend::Deinit()
{
	return Success;
}
