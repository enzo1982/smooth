 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/system/timer.h>
#include <smooth/system/backends/win32/timerwin32.h>

S::System::TimerBackend *CreateTimerWin32()
{
	return new S::System::TimerWin32();
}

S::Int	 timerWin32Tmp = S::System::TimerBackend::SetBackend(&CreateTimerWin32);

S::System::TimerWin32::TimerWin32()
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

	return Success();
}

S::Int S::System::TimerWin32::Stop()
{
	if (timerid == -1) return Error();

	KillTimer(NIL, timerid);

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
	for (Int i = 0; i < Object::GetNOfObjects(); i++)
	{
		Object *object = Object::GetNthObject(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == Timer::classID)
			{
				if (((Timer *) object)->GetID() == (Int) timerid)
				{
					((Timer *) object)->onInterval.Emit();

					return;
				}
			}
		}
	}
}
