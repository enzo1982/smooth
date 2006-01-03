 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/dll.h>
#include "myactivearea.h"

Void smooth::AttachDLL(Void *)
{
}

Void smooth::DetachDLL()
{
}

const Int	 MyActiveArea::classID = Object::RequestClassID();

MyActiveArea::MyActiveArea(Int color, Point iPos, Size iSize) : Widget(iPos, iSize)
{
	type		= classID;
	areaColor	= color;
}

MyActiveArea::~MyActiveArea()
{
}

Int MyActiveArea::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Window	*window		= container->GetContainerWindow();

	if (window == NIL) return Success();

	Surface	*surface	= window->GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), GetSize());

	surface->Box(frame, areaColor, FILLED);
	surface->Frame(frame, FRAME_DOWN);

	return Success();
}

Int MyActiveArea::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())		return Error();
	if (!active || !visible)	return Success();

	Window	*window		= container->GetContainerWindow();

	if (window == NIL) return Success();

	Point	 realPos	= GetRealPosition();
	Int	 retVal		= Success();

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if ((window->MouseX() > realPos.x) && (window->MouseX() < (realPos.x + GetWidth() - 1)) && (window->MouseY() > realPos.y) && (window->MouseY() < (realPos.y + GetHeight() - 1)))
			{
				onAction.Emit();

				retVal = Break;
			}

			break;
	}

	return retVal;
}
