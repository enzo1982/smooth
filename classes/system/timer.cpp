 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/timer.h>

#ifdef __WIN32__
#include <smooth/system/win32/timerwin32.h>
#else
#include <smooth/system/sdl/timersdl.h>
#endif

const S::Int	 S::System::Timer::classID = S::Object::RequestClassID();

S::System::Timer::Timer()
{
#ifdef __WIN32__
	backend = new TimerWin32();
#else
	backend = new TimerSDL();
#endif

	type = classID;

	onInterval.SetParentObject(this);
}

S::System::Timer::~Timer()
{
	Stop();

	delete backend;
}

S::Int S::System::Timer::Start(Int interval)
{
	return backend->Start(interval);
}

S::Int S::System::Timer::Stop()
{
	return backend->Stop();
}

S::Int S::System::Timer::GetID()
{
	return backend->GetID();
}
