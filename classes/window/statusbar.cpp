 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_STATUSBAR_
#define __OBJSMOOTH_STATUSBAR_

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

SMOOTHInt	 OBJ_STATUSBAR = SMOOTH::RequestObjectID();

SMOOTHStatusbar::SMOOTHStatusbar(SMOOTHString status)
{
	type				= OBJ_STATUSBAR;
	objectProperties->text		= status;
	objectProperties->orientation	= OR_BOTTOM;
	objectProperties->size.cy	= METRIC_STATUSBARHEIGHT;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

SMOOTHStatusbar::~SMOOTHStatusbar()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHStatusbar::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHRect	 textRect;

	textRect.left	= objectProperties->pos.x + METRIC_SBTEXTOFFSETX;
	textRect.top	= objectProperties->pos.y + METRIC_SBTEXTOFFSETY;
	textRect.right	= objectProperties->pos.x + objectProperties->size.cx;
	textRect.bottom	= objectProperties->pos.y + objectProperties->size.cy;

	::SetText(dc, objectProperties->text, textRect,  objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHStatusbar::SetText(SMOOTHString newStatus)
{
	SMOOTHString	 oldStatus = objectProperties->text;

	objectProperties->text = newStatus;

	if (!registered || !visible) return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHRect	 textRect;

	textRect.left	= objectProperties->pos.x + METRIC_SBTEXTOFFSETX;
	textRect.top	= objectProperties->pos.y + METRIC_SBTEXTOFFSETY;
	textRect.right	= objectProperties->pos.x + objectProperties->size.cx;
	textRect.bottom	= objectProperties->pos.y + objectProperties->size.cy;

	::SetText(dc, oldStatus, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::BackgroundColor, objectProperties->fontWeight);
	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

#endif
