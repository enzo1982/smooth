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
#include <smooth/system/win32/timerwin32.h>
#include <smooth/objectmanager.h>

S::System::TimerBackend *CreateTimerWin32()
{
	return new S::System::TimerWin32();
}

S::Int	 timerWin32Tmp = S::System::TimerBackend::AddBackend(&CreateTimerWin32);

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
	if (timerid != -1) return Error;

	timerid = SetTimer(NIL, 0, interval, &TimerProc);

	return Success;
}

S::Int S::System::TimerWin32::Stop()
{
	if (timerid == -1) return Error;

	KillTimer(NIL, timerid);

	timerid = -1;

	return Success;
}

S::Int S::System::TimerWin32::GetID()
{
	if (timerid == -1) return Error;

	return timerid;
}

S::Void WINAPI S::System::TimerWin32::TimerProc(HWND wnd, unsigned int message, unsigned int timerid, unsigned long time)
{
	for (S::Int i = 0; i < S::mainObjectManager->GetNOfObjects(); i++)
	{
		S::Object *object = S::mainObjectManager->GetNthObject(i);

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
