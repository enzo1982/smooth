 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/activearea.h>
#include <smooth/definitions.h>
#include <smooth/objectproperties.h>
#include <smooth/graphics/surface.h>
#include <smooth/math.h>
#include <smooth/stk.h>
#include <smooth/layer.h>
#include <smooth/toolkit.h>

const S::Int	 S::GUI::ActiveArea::classID = S::Object::RequestClassID();

S::GUI::ActiveArea::ActiveArea(Int color, Point pos, Size size)
{
	type		= classID;
	areaColor	= color;

	possibleContainers.AddEntry(Layer::classID);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(20 * Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
}

S::GUI::ActiveArea::~ActiveArea()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::ActiveArea::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 frame;
	Point	 realPos = GetRealPosition();

	switch (message)
	{
		default:
		case SP_PAINT:
			frame.left	= realPos.x;
			frame.top	= realPos.y;
			frame.right	= realPos.x + objectProperties->size.cx - 1;
			frame.bottom	= realPos.y + objectProperties->size.cy - 1;

			surface->Frame(frame, FRAME_DOWN);

			frame.left++;
			frame.top++;

			surface->Box(frame, areaColor, FILLED);

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::ActiveArea::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Int	 retVal = Success;
	Point	 realPos = GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x + 1;
	frame.top	= realPos.y + 1;
	frame.right	= realPos.x + objectProperties->size.cx - 2;
	frame.bottom	= realPos.y + objectProperties->size.cy - 2;

	switch (message)
	{
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			if (wnd->IsMouseOn(frame))
			{
				onClick.Emit(wnd->MouseX(), wnd->MouseY());

				retVal = Break;
			}

			break;
	}

	return retVal;
}

S::Int S::GUI::ActiveArea::SetColor(Int newColor)
{
	areaColor = newColor;

	if (!registered)	return Success;
	if (!visible)		return Success;

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::ActiveArea::GetColor()
{
	return areaColor;
}
