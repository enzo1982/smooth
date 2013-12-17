 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
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

const S::Short	 S::GUI::Statusbar::classID = S::Object::RequestClassID();

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
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	switch (message)
	{
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

				surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
				surface->SetText(text, frame + Point(4, 1) * surface->GetSurfaceDPI() / 96.0 - Size(8, 2) * surface->GetSurfaceDPI() / 96.0, font);
			}

			break;
	};

	Int	 occupied_right	= 5;
	Int	 occupied_left	= 5;

#ifdef __APPLE__
	/* Check if OS X version is lower than 10.7 (Darwin 11.0) and
	 * occupy ten pixels for the handle at the right in that case.
	 */
	Buffer<char>	 buffer(32);
	FILE		*pstdin = popen("sysctl kern.osrelease", "r");

	fscanf(pstdin, String("%[^\n]").Append(String::FromInt(buffer.Size() - 1)), (char *) buffer);

	pclose(pstdin);

	String		 osrelease = (char *) buffer;

	if (!osrelease.StartsWith("kern.osrelease: ") || osrelease.Tail(osrelease.Length() - 16).ToInt() < 11)
	{
		occupied_right += 10;
	}
#endif

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*object = GetNthObject(i);

		if (object->GetOrientation() == OR_UPPERRIGHT)
		{
			object->SetPosition(Point(occupied_right + object->GetWidth(), (GetHeight() - object->GetHeight()) / 2));

			occupied_right += object->GetWidth() + 5;
		}
		else if (object->GetOrientation() == OR_UPPERLEFT)
		{
			object->SetPosition(Point(occupied_left, (GetHeight() - object->GetHeight()) / 2));

			occupied_left += object->GetWidth() + 5;
		}
	}

	return Widget::Paint(message);
}

S::Int S::GUI::Statusbar::SetText(const String &newStatus)
{
	if (text == newStatus) return Success();

	text = newStatus;

	Paint(SP_PAINT);

	return Success();
}
