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
#include <smooth/objectmanager.h>
#include <smooth/i18n.h>
#include <smooth/window.h>
#include <smooth/timer.h>
#include <smooth/stk.h>

S::BackgroundApplication	*S::backgroundApplication = NIL;

S::BackgroundApplication::BackgroundApplication()
{
	SetText(TXT_SMOOTHBACKGROUNDAPPLICATION);

	backgroundWindow	= new Window(TXT_SMOOTHBACKGROUNDAPPLICATION);
	backgroundTimer		= new Timer();

	RegisterObject(backgroundWindow);

	backgroundWindow->RegisterObject(backgroundTimer);

	backgroundTimer->onInterval.Connect(&BackgroundApplication::TimerProc, this);

	backgroundWindow->Hide();

	backgroundTimer->Start(10);
}

S::BackgroundApplication::~BackgroundApplication()
{
	backgroundTimer->Stop();

	backgroundWindow->Close();
	backgroundWindow->UnregisterObject(backgroundTimer);

	UnregisterObject(backgroundWindow);

	delete backgroundWindow;
	delete backgroundTimer;
}

S::Void S::BackgroundApplication::TimerProc()
{
	Object	*object;

	for (Int i = 0; i < Object::objectCount; i++)
	{
		object = mainObjectManager->RequestObject(i);

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
				else if (object->GetObjectType() == OBJ_WINDOW)
				{
					object->Process(SM_MOUSEMOVE, 1, 0);
				}
			}
		}
	}
}
