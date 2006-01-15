 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/eventbackend.h>

#if defined __WIN32__ && defined __SMOOTH_STATIC__
	#include <smooth/system/backends/win32/eventwin32.h>
#endif

S::System::EventBackend *CreateEventBackend()
{
	return new S::System::EventBackend();
}

S::Int	 eventBackendTmp = S::System::EventBackend::AddBackend(&CreateEventBackend);

S::Array<S::System::EventBackend *(*)(), S::Void *>	*S::System::EventBackend::backend_creators = NIL;

S::Int S::System::EventBackend::AddBackend(EventBackend *(*backend)())
{
	if (backend == NIL) return Error();

	if (backend_creators == NIL) backend_creators = new Array<EventBackend *(*)(), Void *>;

	backend_creators->AddEntry(backend);

	return Success();
}

S::System::EventBackend *S::System::EventBackend::CreateBackendInstance()
{
	if (backend_creators->GetFirstEntry() != &CreateEventBackend)	return backend_creators->GetFirstEntry()();
	else								return backend_creators->GetLastEntry()();
}

S::System::EventBackend::EventBackend()
{
#if defined __WIN32__ && defined __SMOOTH_STATIC__
	volatile Bool	 null = 0;

	if (null) EventWin32();
#endif

	type = EVENT_NONE;
}

S::System::EventBackend::~EventBackend()
{
}

S::Int S::System::EventBackend::GetEventType()
{
	return type;
}

S::Int S::System::EventBackend::ProcessNextEvent(Bool block)
{
	return Error();
}
