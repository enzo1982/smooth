 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/dll.h>
#include "plugin.h"

Void smooth::AttachDLL()
{
}

Void smooth::DetachDLL()
{
}

#ifdef __WIN32__
__declspec (dllexport)
#endif

Int	 OBJ_ACTIVEAREAPLUGIN = Object::RequestObjectID();

ActiveAreaPlugin::ActiveAreaPlugin(Int color, Point pos, Size size)
{
	type			= OBJ_ACTIVEAREAPLUGIN;
	areaColor		= color;
	objectProperties->pos	= pos;
	objectProperties->size	= size;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x		= Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y		= Math::Round(pos.y * Setup::FontSize);
	objectProperties->size.cx	= Math::Round(size.cx * Setup::FontSize);
	objectProperties->size.cy	= Math::Round(size.cy * Setup::FontSize);
}

ActiveAreaPlugin::~ActiveAreaPlugin()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

Int ActiveAreaPlugin::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Surface	*surface = wnd->GetDrawSurface();
	HDC	 dc = ((SurfaceGDI *) surface)->GetContext();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	HBRUSH	 brush = CreateSolidBrush(areaColor);
	HPEN	 hpen;
	HPEN	 holdpen;
	Point	 p1;
	Point	 p2;
	Point	 p3;
	Point	 p4;

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

	hpen = CreatePen(PS_SOLID, 1, RGB(max((Setup::BackgroundColor & 255) - 64, 0), max(((Setup::BackgroundColor >> 8) & 255) - 64, 0), max(((Setup::BackgroundColor >> 16) & 255) - 64, 0)));
	holdpen = (HPEN) SelectObject(dc, hpen);

	MoveToEx(dc, p1.x, p1.y, NIL);
	LineTo(dc, p2.x, p2.y);

	SelectObject(dc, holdpen);
	::DeleteObject(hpen);

	hpen = CreatePen(PS_SOLID, 1, RGB(max((Setup::BackgroundColor & 255) - 64, 0), max(((Setup::BackgroundColor >> 8) & 255) - 64, 0), max(((Setup::BackgroundColor >> 16) & 255) - 64, 0)));
	holdpen = (HPEN) SelectObject(dc, hpen);

	MoveToEx(dc, p1.x, p1.y, NIL);
	LineTo(dc, p3.x, p3.y);

	SelectObject(dc, holdpen);
	::DeleteObject(hpen);

	hpen = CreatePen(PS_SOLID, 1, RGB(min((Setup::BackgroundColor & 255) + 64, 255), min(((Setup::BackgroundColor >> 8) & 255) + 64, 255), min(((Setup::BackgroundColor >> 16) & 255) + 64, 255)));
	holdpen = (HPEN) SelectObject(dc, hpen);

	MoveToEx(dc, p2.x, p2.y, NIL);
	LineTo(dc, p4.x, p4.y);

	SelectObject(dc, holdpen);
	::DeleteObject(hpen);

	hpen = CreatePen(PS_SOLID, 1, RGB(min((Setup::BackgroundColor & 255) + 64, 255), min(((Setup::BackgroundColor >> 8) & 255) + 64, 255), min(((Setup::BackgroundColor >> 16) & 255) + 64, 255)));
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

	return Success;
}

Int ActiveAreaPlugin::Process(Int message, Int wParam, Int lParam)
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
			Point	 mousePos;
			Rect	 winRect;

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
				onClick.Emit();

				retVal = Break;
			}

			break;
	}

	return retVal;
}
