 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/backends/timerbackend.h>

#if defined __WIN32__ && defined SMOOTH_STATIC
	#include <smooth/system/backends/win32/timerwin32.h>
#endif

S::System::TimerBackend *CreateTimerBackend(S::System::Timer *timer)
{
	return new S::System::TimerBackend(timer);
}

S::System::TimerBackend *(*S::System::TimerBackend::backend_creator)(S::System::Timer *) = &CreateTimerBackend;

S::Int S::System::TimerBackend::SetBackend(TimerBackend *(*backend)(Timer *))
{
	if (backend == NIL) return Error();

	backend_creator = backend;

	return Success();
}

S::System::TimerBackend *S::System::TimerBackend::CreateBackendInstance(Timer *timer)
{
	return backend_creator(timer);
}

S::System::TimerBackend::TimerBackend(Timer *iTimer)
{
#if defined __WIN32__ && defined SMOOTH_STATIC
	volatile Bool	 null = 0;

	if (null) TimerWin32();
#endif

	type = TIMER_NONE;

	thisTimer = iTimer;
}

S::System::TimerBackend::~TimerBackend()
{
}

S::Short S::System::TimerBackend::GetTimerType() const
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
