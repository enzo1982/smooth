 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/window.h>
#include <smooth/statusbar.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_STATUSBAR = S::Object::RequestObjectID();

S::GUI::Statusbar::Statusbar(String status)
{
	type				= OBJ_STATUSBAR;
	objectProperties->text		= status;
	objectProperties->orientation	= OR_BOTTOM;
	objectProperties->size.cy	= METRIC_STATUSBARHEIGHT;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

S::GUI::Statusbar::~Statusbar()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Statusbar::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Window	*wnd = (Window *) myContainer->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	HDC	 dc = GetContext(wnd);
	Rect	 textRect;

	textRect.left	= objectProperties->pos.x + METRIC_SBTEXTOFFSETX;
	textRect.top	= objectProperties->pos.y + METRIC_SBTEXTOFFSETY;
	textRect.right	= objectProperties->pos.x + objectProperties->size.cx;
	textRect.bottom	= objectProperties->pos.y + objectProperties->size.cy;

	::SetText(dc, objectProperties->text, textRect,  objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return Success;
}

S::Int S::GUI::Statusbar::SetText(String newStatus)
{
	String	 oldStatus = objectProperties->text;

	objectProperties->text = newStatus;

	if (!registered || !visible) return Success;

	Window	*wnd = (Window *) myContainer->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	HDC	 dc = GetContext(wnd);
	Rect	 textRect;

	textRect.left	= objectProperties->pos.x + METRIC_SBTEXTOFFSETX;
	textRect.top	= objectProperties->pos.y + METRIC_SBTEXTOFFSETY;
	textRect.right	= objectProperties->pos.x + objectProperties->size.cx;
	textRect.bottom	= objectProperties->pos.y + objectProperties->size.cy;

	::SetText(dc, oldStatus, textRect, objectProperties->font, objectProperties->fontSize, Setup::BackgroundColor, objectProperties->fontWeight);
	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return Success;
}
