 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/popupmenuentrycheck.h>
#include <smooth/gui/widgets/special/shortcut.h>
#include <smooth/gui/widgets/basic/checkbox.h>
#include <smooth/graphics/color.h>
#include <smooth/graphics/surface.h>

const S::Short	 S::GUI::PopupMenuEntryCheck::classID = S::Object::RequestClassID();

S::GUI::PopupMenuEntryCheck::PopupMenuEntryCheck(const String &iText, Bool *ibVar) : PopupMenuEntry(iText)
{
	type	= classID;

	bVar	= ibVar;
}

S::GUI::PopupMenuEntryCheck::~PopupMenuEntryCheck()
{
}

S::Int S::GUI::PopupMenuEntryCheck::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	Surface	*surface = GetDrawSurface();
	Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

	surface->StartPaint(frame);

	PopupMenuEntry::Paint(message);

	switch (message)
	{
		case SP_PAINT:
		case SP_MOUSEIN:
		case SP_MOUSEOUT:
			if (*bVar == True)
			{
				Float	 scaleFactor = surface->GetSurfaceDPI() / 96.0;

				for (Int i = 0; i < 2; i++)
				{
					Color	 color	= (IsActive() ? Setup::TextColor : Setup::InactiveTextColor);

					if (message == SP_MOUSEIN) color = Setup::GradientTextColor;
					if (i == 0)		   color = Color(color.GetRed() + (255 - color.GetRed()) * 0.6, color.GetGreen() + (255 - color.GetGreen()) * 0.6, color.GetBlue() + (255 - color.GetBlue()) * 0.6);

					Int	 width  = 4 * scaleFactor;
					Point	 offset = Point(IsRightToLeft() ? 1 : 0, 0);

					if (i == 0) offset += Point(IsRightToLeft() ? -1 : 1, 1);

					Point	 p1	= Point(frame.left		       + width, frame.top    + width) + offset;
					Point	 p2	= Point(frame.left + frame.GetHeight() - width, frame.bottom - width) + offset;

					surface->Line(p1 + Point(0, 0), p2 + Point( 0,  0), color);
					surface->Line(p1 + Point(1, 0), p2 + Point( 0, -1), color);
					surface->Line(p1 + Point(0, 1), p2 + Point(-1,  0), color);

					p1 = Point(frame.left + frame.GetHeight() - width - 1, frame.top    + width) + offset;
					p2 = Point(frame.left			  + width - 1, frame.bottom - width) + offset;

					surface->Line(p1 + Point( 0, 0), p2 + Point(0,  0), color);
					surface->Line(p1 + Point( 0, 1), p2 + Point(1,  0), color);
					surface->Line(p1 + Point(-1, 0), p2 + Point(0, -1), color);
				}
			}

			break;
	}

	surface->EndPaint();

	return Success();
}

S::Void S::GUI::PopupMenuEntryCheck::OnClickEntry()
{
	*bVar = !*bVar;

	CheckBox::internalCheckValues.Emit();

	PopupMenuEntry::OnClickEntry();
}

S::Bool S::GUI::PopupMenuEntryCheck::IsTypeCompatible(Short compType) const
{
	if (compType == PopupMenuEntry::classID) return True;
	else					 return PopupMenuEntry::IsTypeCompatible(compType);
}
