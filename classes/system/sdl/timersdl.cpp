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
#include <smooth/system/sdl/timersdl.h>
#include <smooth/objectmanager.h>

#include <SDL/SDL.h>

S::System::TimerBackend *CreateTimerSDL()
{
	return new S::System::TimerSDL();
}

S::Int	 timerSDLTmp = S::System::TimerBackend::AddBackend(&CreateTimerSDL);

S::System::TimerSDL::TimerSDL()
{
	type = TIMER_SDL;

	timerid = -1;
}

S::System::TimerSDL::~TimerSDL()
{
	Stop();
}

S::Int S::System::TimerSDL::Start(Int interval)
{
	if (timerid != -1) return Error;

	timerid = (Int) SDL_AddTimer(interval, &TimerProc, &timerid);

	return Success;
}

S::Int S::System::TimerSDL::Stop()
{
	if (timerid == -1) return Error;

	SDL_RemoveTimer((SDL_TimerID) timerid);

	timerid = -1;

	return Success;
}

S::Int S::System::TimerSDL::GetID()
{
	if (timerid == -1) return Error;

	return timerid;
}

unsigned int S::System::TimerSDL::TimerProc(unsigned int interval, void *timerid)
{
	for (S::Int i = 0; i < S::mainObjectManager->GetNOfObjects(); i++)
	{
		S::Object *object = S::mainObjectManager->GetNthObject(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == Timer::classID)
			{
				if (((Timer *) object)->GetID() == *((Int *) timerid))
				{
					((Timer *) object)->onInterval.Emit();

					return interval;
				}
			}
		}
	}

	return interval;
}
