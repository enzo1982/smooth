 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_BACKGROUND_APPLICATION_
#define __OBJSMOOTH_BACKGROUND_APPLICATION_

#include <smooth/background.h>
#include <smooth/objectmanager.h>
#include <smooth/i18n.h>
#include <smooth/window.h>
#include <smooth/timer.h>
#include <smooth/stk.h>

SMOOTHBackgroundApplication	*backgroundApplication = NIL;

SMOOTHBackgroundApplication::SMOOTHBackgroundApplication()
{
	SetText(TXT_SMOOTHBACKGROUNDAPPLICATION);

	backgroundWindow	= new SMOOTHWindow(TXT_SMOOTHBACKGROUNDAPPLICATION);
	backgroundTimer		= new SMOOTHTimer();

	RegisterObject(backgroundWindow);

	backgroundWindow->RegisterObject(backgroundTimer);

	backgroundTimer->SetProc(SMOOTHProc(SMOOTHBackgroundApplication, this, TimerProc));

	backgroundWindow->Hide();

	backgroundTimer->Start(10);
}

SMOOTHBackgroundApplication::~SMOOTHBackgroundApplication()
{
	backgroundTimer->Stop();

	SMOOTH::CloseWindow(backgroundWindow);

	backgroundWindow->UnregisterObject(backgroundTimer);

	UnregisterObject(backgroundWindow);

	delete backgroundWindow;
	delete backgroundTimer;
}

SMOOTHVoid SMOOTHBackgroundApplication::TimerProc()
{
	SMOOTHObject	*object;

	for (SMOOTHInt i = 0; i < SMOOTHObject::objectCount; i++)
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

#endif
