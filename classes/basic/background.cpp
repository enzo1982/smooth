 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/background.h>
#include <smooth/threadmanager.h>
#include <smooth/objectmanager.h>
#include <smooth/window.h>
#include <smooth/timer.h>

S::BackgroundApplication	*S::backgroundApplication = NIL;

S::BackgroundApplication::BackgroundApplication()
{
	backgroundTimer = new Timer();

	backgroundTimer->onInterval.Connect(&BackgroundApplication::TimerProc, this);
	backgroundTimer->Start(10);
}

S::BackgroundApplication::~BackgroundApplication()
{
	backgroundTimer->Stop();

	delete backgroundTimer;
}
S::Void S::BackgroundApplication::TimerProc()
{
	for (Int i = 0; i < Object::objectCount; i++)
	{
		Object	*object = mainObjectManager->RequestObject(i);

		if (object != NIL)
		{
			if (!object->IsObjectInUse())
			{
				if (object->IsObjectDeleteable())
				{
					delete object;

					i = -1;

					continue;
				}
				else if (object->GetObjectType() == GUI::Window::classID)
				{
					((GUI::Window *) object)->Process(SM_MOUSEMOVE, 1, 0);
				}
			}
		}
	}

 	for (Int j = 0; j < mainThreadManager->GetNOfThreads(); j++)
	{
		if (Setup::enableUnicode)	PostThreadMessageW(mainThreadManager->GetNthThread(j)->GetThreadID(), SM_MOUSEMOVE, 1, 0);
		else				PostThreadMessageA(mainThreadManager->GetNthThread(j)->GetThreadID(), SM_MOUSEMOVE, 1, 0);
	}
}
