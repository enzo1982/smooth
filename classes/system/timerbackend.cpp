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
	return Error;
}

S::Int S::System::TimerBackend::Stop()
{
	return Error;
}

S::Int S::System::TimerBackend::GetID()
{
	return -1;
}
