 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_TIMER_
#define __OBJSMOOTH_TIMER_

#include <smooth/object.h>
#include <smooth/timer.h>
#include <smooth/toolkit.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_TIMER = SMOOTH::RequestObjectID();

SMOOTHTimer::SMOOTHTimer()
{
	type = OBJ_TIMER;

	timerid = 0;
	timerwnd = NIL;

	objectProperties->orientation = OR_FREE;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

SMOOTHTimer::~SMOOTHTimer()
{
	Stop();

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHTimer::Show()
{
	return SMOOTH::Error;
}

SMOOTHInt SMOOTHTimer::Hide()
{
	return SMOOTH::Error;
}

SMOOTHInt SMOOTHTimer::Start(SMOOTHInt interval)
{
	if (!registered)	return SMOOTH::Error;
	if (timerid != 0)	return SMOOTH::Error;

	timerwnd	= (SMOOTHWindow *) myContainer->GetContainerObject();
	timerid		= SMOOTH::RequestGUID();

	if (timerwnd == NIL) return SMOOTH::Error;

#ifdef __WIN32__
	SetTimer(timerwnd->hwnd, timerid, interval, NIL);

	return timerid;
#else
	return SMOOTH::Error;
#endif
}

SMOOTHInt SMOOTHTimer::Stop()
{
	if (!registered)	return SMOOTH::Error;
	if (timerid == 0)	return SMOOTH::Error;

#ifdef __WIN32__
	KillTimer(timerwnd->hwnd, timerid);

	timerid = 0;

	return SMOOTH::Success;
#else
	return SMOOTH::Error;
#endif
}

SMOOTHInt SMOOTHTimer::GetID()
{
	if (!registered)	return SMOOTH::Error;
	if (timerid == 0)	return SMOOTH::Error;

	return timerid;
}

SMOOTHInt SMOOTHTimer::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)	return SMOOTH::Error;
	if (!active)		return SMOOTH::Success;

	EnterProtectedRegion();

	SMOOTHInt	 retVal = SMOOTH::Success;

	switch (message)
	{
		case SM_TIMER:
			if (wParam == timerid)
			{
				SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

				retVal = SMOOTH::Break;
			}
			break;
	}

	LeaveProtectedRegion();

	return retVal;
}

#endif
