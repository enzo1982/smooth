 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/activearea.h>
#include <smooth/definitions.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/surface.h>
#include <smooth/math.h>
#include <smooth/stk.h>
#include <smooth/toolkit.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_ACTIVEAREA = S::Object::RequestObjectID();

S::ActiveArea::ActiveArea(Int color, Point pos, Size size, ProcParam, Void *procParam)
{
	type				= OBJ_ACTIVEAREA;
	areaColor			= color;
	objectProperties->proc		= (ProcType) newProc;
	objectProperties->procParam	= procParam;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(20 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * SMOOTH::Setup::FontSize);
}

S::ActiveArea::~ActiveArea()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::ActiveArea::Paint(Int message)
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

S::Int S::ActiveArea::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Rect	 frame;

	frame.left	= realPos.x + 1;
	frame.top	= realPos.y + 1;
	frame.right	= realPos.x + objectProperties->size.cx - 2;
	frame.bottom	= realPos.y + objectProperties->size.cy - 2;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if (IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				ProcCall(objectProperties->proc, objectProperties->procParam);

				retVal = Break;
			}

			break;
	}

	return retVal;
}

S::Int S::ActiveArea::SetColor(Int newColor)
{
	areaColor = newColor;

	if (!registered)	return Success;
	if (!visible)		return Success;

	Paint(SP_PAINT);

	return Success;
}

S::Int S::ActiveArea::GetColor()
{
	return areaColor;
}
