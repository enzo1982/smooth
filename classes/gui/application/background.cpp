 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/application/background.h>
#include <smooth/gui/window/window.h>
#include <smooth/system/timer.h>
#include <smooth/threads/thread.h>

S::GUI::BackgroundApplication	*S::GUI::backgroundApplication = NIL;

S::GUI::BackgroundApplication::BackgroundApplication()
{
	backgroundTimer = new System::Timer();

	backgroundTimer->onInterval.Connect(&BackgroundApplication::TimerProc, this);
	backgroundTimer->Start(50);
}

S::GUI::BackgroundApplication::~BackgroundApplication()
{
	backgroundTimer->Stop();

	delete backgroundTimer;
}

S::Void S::GUI::BackgroundApplication::TimerProc()
{
	for (Int i = 0; i < Object::GetNOfObjects(); i++)
	{
		Object	*object = Object::GetNthObject(i);

		if (object != NIL)
		{
			if (!object->IsObjectInUse())
			{
				if (object->IsObjectDeletable())
				{
					delete object;

					i = -1;

					continue;
				}
			}

			if (object->GetObjectType() == GUI::Window::classID)
			{
				if (((GUI::Window *) object)->IsInUse()) ((GUI::Window *) object)->Process(SM_MOUSEMOVE, 1, 0);
			}

			if (object->GetObjectType() == Threads::Thread::classID)
			{
				if (((Threads::Thread *) object)->GetStatus() == Threads::THREAD_RUNNING)
				{
#ifdef __WIN32__
					if (Setup::enableUnicode)	PostThreadMessageW(((Threads::Thread *) object)->GetThreadID(), SM_MOUSEMOVE, 1, 0);
					else				PostThreadMessageA(((Threads::Thread *) object)->GetThreadID(), SM_MOUSEMOVE, 1, 0);
#endif
				}
			}
		}
	}
}
