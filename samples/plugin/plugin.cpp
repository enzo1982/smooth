 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smoothplugin.h>
#include "plugin.h"

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_ACTIVEAREAPLUGIN = SMOOTH::RequestObjectID();

SMOOTHActiveAreaPlugin::SMOOTHActiveAreaPlugin(SMOOTHInt color, SMOOTHPoint pos, SMOOTHSize size, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_ACTIVEAREAPLUGIN;
	areaColor			= color;
	objectProperties->pos		= pos;
	objectProperties->size		= size;
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x		= (int) (pos.x * SMOOTH::Setup::FontSize + 0.5);
	objectProperties->pos.y		= (int) (pos.y * SMOOTH::Setup::FontSize + 0.5);
	objectProperties->size.cx	= (int) (size.cx * SMOOTH::Setup::FontSize + 0.5) ;
	objectProperties->size.cy	= (int) (size.cy * SMOOTH::Setup::FontSize + 0.5);
}

SMOOTHActiveAreaPlugin::~SMOOTHActiveAreaPlugin()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHActiveAreaPlugin::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetWindowDC(wnd->hwnd);
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHRect	 frame;
	HBRUSH		 brush = CreateSolidBrush(areaColor);
	HPEN		 hpen;
	HPEN		 holdpen;
	SMOOTHPoint	 p1;
	SMOOTHPoint	 p2;
	SMOOTHPoint	 p3;
	SMOOTHPoint	 p4;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	p1.x = frame.left;
	p1.y = frame.top;
	p2.x = frame.right;
	p2.y = frame.top;
	p3.x = frame.left;
	p3.y = frame.bottom;
	p4.x = frame.right;
	p4.y = frame.bottom;

	hpen = CreatePen(PS_SOLID, 1, RGB(max((SMOOTH::Setup::BackgroundColor & 255) - 64, 0), max(((SMOOTH::Setup::BackgroundColor >> 8) & 255) - 64, 0), max(((SMOOTH::Setup::BackgroundColor >> 16) & 255) - 64, 0)));
	holdpen = (HPEN) SelectObject(dc, hpen);

	MoveToEx(dc, p1.x, p1.y, NIL);
	LineTo(dc, p2.x, p2.y);

	SelectObject(dc, holdpen);
	::DeleteObject(hpen);

	hpen = CreatePen(PS_SOLID, 1, RGB(max((SMOOTH::Setup::BackgroundColor & 255) - 64, 0), max(((SMOOTH::Setup::BackgroundColor >> 8) & 255) - 64, 0), max(((SMOOTH::Setup::BackgroundColor >> 16) & 255) - 64, 0)));
	holdpen = (HPEN) SelectObject(dc, hpen);

	MoveToEx(dc, p1.x, p1.y, NIL);
	LineTo(dc, p3.x, p3.y);

	SelectObject(dc, holdpen);
	::DeleteObject(hpen);

	hpen = CreatePen(PS_SOLID, 1, RGB(min((SMOOTH::Setup::BackgroundColor & 255) + 64, 255), min(((SMOOTH::Setup::BackgroundColor >> 8) & 255) + 64, 255), min(((SMOOTH::Setup::BackgroundColor >> 16) & 255) + 64, 255)));
	holdpen = (HPEN) SelectObject(dc, hpen);

	MoveToEx(dc, p2.x, p2.y, NIL);
	LineTo(dc, p4.x, p4.y);

	SelectObject(dc, holdpen);
	::DeleteObject(hpen);

	hpen = CreatePen(PS_SOLID, 1, RGB(min((SMOOTH::Setup::BackgroundColor & 255) + 64, 255), min(((SMOOTH::Setup::BackgroundColor >> 8) & 255) + 64, 255), min(((SMOOTH::Setup::BackgroundColor >> 16) & 255) + 64, 255)));
	holdpen = (HPEN) SelectObject(dc, hpen);

	p4.x++;

	MoveToEx(dc, p3.x, p3.y, NIL);
	LineTo(dc, p4.x, p4.y);

	SelectObject(dc, holdpen);
	::DeleteObject(hpen);

	frame.left++;
	frame.top++;

	RECT	 fRect = frame;

	FillRect(dc, &fRect, brush);

	::DeleteObject(brush);

	ReleaseDC(wnd->hwnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHActiveAreaPlugin::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
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
			SMOOTHPoint	 mousePos;
			SMOOTHRect	 winRect;

			{
				RECT	 wRect = winRect;
				POINT	 mPos = mousePos;

				GetWindowRect(wnd->hwnd, &wRect);
				GetCursorPos(&mPos);

				winRect = wRect;
				mousePos = mPos;
			}

			mousePos.x = mousePos.x - winRect.left;
			mousePos.y = mousePos.y - winRect.top;

			if ((mousePos.x >= frame.left) && (mousePos.x <= frame.right) && (mousePos.y >= frame.top) && (mousePos.y <= frame.bottom))
			{
				SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

				retVal = SMOOTH::Break;
			}

			break;
	}

	return retVal;
}
