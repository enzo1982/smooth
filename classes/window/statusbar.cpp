 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/statusbar.h>
#include <smooth/window.h>
#include <smooth/metrics.h>
#include <smooth/objectproperties.h>
#include <smooth/surface.h>

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

	Surface	*surface = myContainer->GetDrawSurface();
	Rect	 textRect;

	textRect.left	= objectProperties->pos.x + METRIC_SBTEXTOFFSETX;
	textRect.top	= objectProperties->pos.y + METRIC_SBTEXTOFFSETY;
	textRect.right	= objectProperties->pos.x + objectProperties->size.cx;
	textRect.bottom	= objectProperties->pos.y + objectProperties->size.cy;

	surface->SetText(objectProperties->text, textRect,  objectProperties->font);

	return Success;
}

S::Int S::GUI::Statusbar::SetText(String newStatus)
{
	String	 oldStatus = objectProperties->text;

	objectProperties->text = newStatus;

	if (!registered || !visible) return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Rect	 textRect;

	textRect.left	= objectProperties->pos.x + METRIC_SBTEXTOFFSETX;
	textRect.top	= objectProperties->pos.y + METRIC_SBTEXTOFFSETY;
	textRect.right	= objectProperties->pos.x + objectProperties->size.cx;
	textRect.bottom	= objectProperties->pos.y + objectProperties->size.cy;

	Font	 font = objectProperties->font;

	font.SetColor(Setup::BackgroundColor);

	surface->SetText(oldStatus, textRect, font);
	surface->SetText(objectProperties->text, textRect, objectProperties->font);

	return Success;
}
