 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_ACTIVEAREA_
#define __OBJSMOOTH_ACTIVEAREA_

#include <smooth/activearea.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_ACTIVEAREA = SMOOTH::RequestObjectID();

SMOOTHActiveArea::SMOOTHActiveArea(SMOOTHInt color, SMOOTHPoint pos, SMOOTHSize size, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_ACTIVEAREA;
	areaColor			= color;
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0)	objectProperties->size.cx = roundtoint(80 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = roundtoint(20 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
}

SMOOTHActiveArea::~SMOOTHActiveArea()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHActiveArea::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHSurface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	SMOOTHRect	 frame;
	SMOOTHPoint	 realPos = GetRealPosition();

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

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHActiveArea::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHInt	 retVal = SMOOTH::Success;
	SMOOTHRect	 frame;

	frame.left	= realPos.x + 1;
	frame.top	= realPos.y + 1;
	frame.right	= realPos.x + objectProperties->size.cx - 2;
	frame.bottom	= realPos.y + objectProperties->size.cy - 2;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if (IsMouseOn(wnd->hwnd, frame, WINDOW))
			{
				SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

				retVal = SMOOTH::Break;
			}

			break;
	}

	return retVal;
}

SMOOTHInt SMOOTHActiveArea::SetColor(SMOOTHInt newColor)
{
	areaColor = newColor;

	if (!registered)	return SMOOTH::Success;
	if (!visible)		return SMOOTH::Success;

	Paint(SP_PAINT);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHActiveArea::GetColor()
{
	return areaColor;
}

#endif
