 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/object.h>
#include <smooth/timer.h>
#include <smooth/toolkit.h>
#include <smooth/stk.h>
#include <smooth/system.h>
#include <smooth/objectproperties.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_TIMER = S::Object::RequestObjectID();

S::Timer::Timer()
{
	type = OBJ_TIMER;

	timerid = 0;
	timerwnd = NIL;

	objectProperties->orientation = OR_FREE;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

S::Timer::~Timer()
{
	Stop();

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::Timer::Show()
{
	return Error;
}

S::Int S::Timer::Hide()
{
	return Error;
}

S::Int S::Timer::Start(Int interval)
{
	if (!registered)	return Error;
	if (timerid != 0)	return Error;

	timerwnd	= (GUI::Window *) myContainer->GetContainerObject();
	timerid		= System::RequestGUID();

	if (timerwnd == NIL) return Error;

#ifdef __WIN32__
	SetTimer(timerwnd->hwnd, timerid, interval, NIL);

	return timerid;
#else
	return Error;
#endif
}

S::Int S::Timer::Stop()
{
	if (!registered)	return Error;
	if (timerid == 0)	return Error;

#ifdef __WIN32__
	KillTimer(timerwnd->hwnd, timerid);

	timerid = 0;

	return Success;
#else
	return Error;
#endif
}

S::Int S::Timer::GetID()
{
	if (!registered)	return Error;
	if (timerid == 0)	return Error;

	return timerid;
}

S::Int S::Timer::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)	return Error;
	if (!active)		return Success;

	EnterProtectedRegion();

	Int	 retVal = Success;

	switch (message)
	{
		case SM_TIMER:
			if (wParam == timerid)
			{
				onInterval.Emit();

				retVal = Break;
			}
			break;
	}

	LeaveProtectedRegion();

	return retVal;
}
