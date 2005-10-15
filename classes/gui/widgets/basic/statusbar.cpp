 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/statusbar.h>
#include <smooth/gui/window/window.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Statusbar::classID = S::Object::RequestClassID();

S::GUI::Statusbar::Statusbar(String status)
{
	type		= classID;
	text		= status;
	orientation	= OR_BOTTOM;
	size.cy		= 16;

	possibleContainers.AddEntry(Window::classID);
}

S::GUI::Statusbar::~Statusbar()
{
}

S::Int S::GUI::Statusbar::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface = container->GetDrawSurface();
	Rect	 textRect;

	textRect.left	= pos.x + 4;
	textRect.top	= pos.y + 1;
	textRect.right	= pos.x + size.cx;
	textRect.bottom	= pos.y + size.cy;

	surface->SetText(text, textRect, font);

	return Success;
}

S::Int S::GUI::Statusbar::SetText(const String &newStatus)
{
	text = newStatus;

	if (!IsRegistered() || !IsVisible()) return Success;

	Surface	*surface	= container->GetDrawSurface();
	Rect	 textRect;

	textRect.left	= pos.x + 4;
	textRect.top	= pos.y + 1;
	textRect.right	= pos.x + size.cx;
	textRect.bottom	= pos.y + size.cy;

	surface->Box(textRect, Setup::BackgroundColor, FILLED);

	surface->SetText(text, textRect, font);

	return Success;
}
