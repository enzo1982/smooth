 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/hotspot/simplebutton.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::HotspotSimpleButton::classID = S::Object::RequestClassID();

S::GUI::HotspotSimpleButton::HotspotSimpleButton(const Point &iPos, const Size &iSize) : Hotspot(iPos, iSize)
{
	type	= classID;
}

S::GUI::HotspotSimpleButton::~HotspotSimpleButton()
{
}

S::Int S::GUI::HotspotSimpleButton::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface = container->GetDrawSurface();
	Rect	 frame	 = Rect(GetRealPosition(), GetSize());

	switch (message)
	{
		case SP_MOUSEIN:
		case SP_MOUSEUP:
			surface->Frame(frame, FRAME_UP);

			break;
		case SP_MOUSEDOWN:
			surface->Frame(frame, FRAME_DOWN);

			break;
		case SP_MOUSEOUT:
			surface->Box(frame, GetBackgroundColor() >= 0 ? GetBackgroundColor() : Setup::BackgroundColor, Rect::Outlined);

			break;
	}

	return Success();
}
