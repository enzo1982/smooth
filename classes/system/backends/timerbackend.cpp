 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/timerbackend.h>

#if defined __WIN32__ && defined __SMOOTH_STATIC__
	#include <smooth/system/backends/win32/timerwin32.h>
#endif

S::System::TimerBackend *CreateTimerBackend()
{
	return new S::System::TimerBackend();
}

S::Int	 timerBackendTmp = S::System::TimerBackend::AddBackend(&CreateTimerBackend);

S::Array<S::System::TimerBackend *(*)(), S::Void *>	*S::System::TimerBackend::backend_creators = NIL;

S::Int S::System::TimerBackend::AddBackend(TimerBackend *(*backend)())
{
	if (backend == NIL) return Error();

	if (backend_creators == NIL) backend_creators = new Array<TimerBackend *(*)(), Void *>;

	backend_creators->AddEntry(backend);

	return Success();
}

S::System::TimerBackend *S::System::TimerBackend::CreateBackendInstance()
{
	if (backend_creators->GetFirstEntry() != &CreateTimerBackend)	return backend_creators->GetFirstEntry()();
	else								return backend_creators->GetLastEntry()();
}

S::System::TimerBackend::TimerBackend()
{
#if defined __WIN32__ && defined __SMOOTH_STATIC__
	volatile Bool	 null = 0;

	if (null) TimerWin32();
#endif

	type = TIMER_NONE;
}

S::System::TimerBackend::~TimerBackend()
{
}

S::Int S::System::TimerBackend::GetTimerType() const
{
	return type;
}

S::Int S::System::TimerBackend::Start(Int interval)
{
	return Error();
}

S::Int S::System::TimerBackend::Stop()
{
	return Error();
}

S::Int S::System::TimerBackend::GetID() const
{
	return -1;
}
