 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/timer.h>
#include <smooth/system/backends/win32/timerwin32.h>

S::System::TimerBackend *CreateTimerWin32(S::System::Timer *timer)
{
	return new S::System::TimerWin32(timer);
}

S::Int	 timerWin32Tmp = S::System::TimerBackend::SetBackend(&CreateTimerWin32);

S::Array<S::Int>	 S::System::TimerWin32::handles;

S::System::TimerWin32::TimerWin32(Timer *timer) : TimerBackend(timer)
{
	type = TIMER_WIN32;

	timerid = -1;
}

S::System::TimerWin32::~TimerWin32()
{
	Stop();
}

S::Int S::System::TimerWin32::Start(Int interval)
{
	if (timerid != -1) return Error();

	timerid = SetTimer(NIL, 0, interval, &TimerProc);

	handles.Add(thisTimer->GetHandle(), timerid);

	return Success();
}

S::Int S::System::TimerWin32::Stop()
{
	if (timerid == -1) return Error();

	KillTimer(NIL, timerid);

	handles.Remove(timerid);

	timerid = -1;

	return Success();
}

S::Int S::System::TimerWin32::GetID() const
{
	if (timerid == -1) return Error();

	return timerid;
}

void WINAPI S::System::TimerWin32::TimerProc(HWND wnd, UINT message, UINT_PTR timerid, DWORD time)
{
	Timer	*timer = (Timer *) Object::GetObject(handles.Get((Int) timerid), Timer::classID);

	if (timer != NIL) timer->onInterval.Emit();
}
