 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/window/statusbar.h>
#include <smooth/window/window.h>
#include <smooth/objectproperties.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Statusbar::classID = S::Object::RequestClassID();

S::GUI::Statusbar::Statusbar(String status)
{
	type				= classID;
	objectProperties->text		= status;
	objectProperties->orientation	= OR_BOTTOM;
	objectProperties->size.cy	= 16;

	possibleContainers.AddEntry(Window::classID);
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

	textRect.left	= objectProperties->pos.x + 4;
	textRect.top	= objectProperties->pos.y + 1;
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

	textRect.left	= objectProperties->pos.x + 4;
	textRect.top	= objectProperties->pos.y + 1;
	textRect.right	= objectProperties->pos.x + objectProperties->size.cx;
	textRect.bottom	= objectProperties->pos.y + objectProperties->size.cy;

	Font	 font = objectProperties->font;

	font.SetColor(Setup::BackgroundColor);

	surface->SetText(oldStatus, textRect, font);
	surface->SetText(objectProperties->text, textRect, objectProperties->font);

	return Success;
}
