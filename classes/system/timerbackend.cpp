 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/timerbackend.h>

S::System::TimerBackend *CreateTimerBackend()
{
	return new S::System::TimerBackend();
}

S::Int	 timerBackendTmp = S::System::TimerBackend::AddBackend(&CreateTimerBackend);

S::Array<S::System::TimerBackend *(*)()>	*S::System::TimerBackend::backend_creators = NIL;

S::Int S::System::TimerBackend::AddBackend(TimerBackend *(*backend)())
{
	if (backend == NIL) return Failure;

	if (backend_creators == NIL) backend_creators = new Array<TimerBackend *(*)()>;

	backend_creators->AddEntry(backend);

	return Success;
}

S::System::TimerBackend *S::System::TimerBackend::CreateBackendInstance()
{
	if (backend_creators->GetFirstEntry() != &CreateTimerBackend)	return backend_creators->GetFirstEntry()();
	else								return backend_creators->GetLastEntry()();
}

S::System::TimerBackend::TimerBackend()
{
	type = TIMER_NONE;
}

S::System::TimerBackend::~TimerBackend()
{
}

S::Int S::System::TimerBackend::GetTimerType()
{
	return type;
}

S::Int S::System::TimerBackend::Start(Int interval)
{
	return Failure;
}

S::Int S::System::TimerBackend::Stop()
{
	return Failure;
}

S::Int S::System::TimerBackend::GetID()
{
	return -1;
}
