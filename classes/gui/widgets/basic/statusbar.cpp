 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

S::GUI::Statusbar::Statusbar(const String &status) : Widget(Point(), Size(0, 16))
{
	type		= classID;
	text		= status;
	orientation	= OR_BOTTOM;
}

S::GUI::Statusbar::~Statusbar()
{
}

S::Int S::GUI::Statusbar::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Surface	*surface = container->GetDrawSurface();

				surface->Box(Rect(GetRealPosition(), GetSize()), Setup::BackgroundColor, FILLED);
				surface->SetText(text, Rect(GetRealPosition() + Point(4, 1), GetSize() - Size(8, 2)), font);
			}

			break;
	};

	return Success();
}

S::Int S::GUI::Statusbar::SetText(const String &newStatus)
{
	text = newStatus;

	Paint(SP_PAINT);

	return Success();
}
