 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
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
				for (Int i = 0; i < 2; i++)
				{
					Int	 color	= (message == SP_MOUSEIN ? Setup::InactiveGradientTextColor : Setup::DividerDarkColor);
					Int	 offset = 4 * surface->GetSurfaceDPI() / 96.0;

					Point	 p1	= Point(frame.left		       + offset, frame.top    + offset) - (i == 1 ? Point(i, i) : Point());
					Point	 p2	= Point(frame.left + frame.GetHeight() - offset, frame.bottom - offset) - (i == 1 ? Point(i, i) : Point());

					if (i == 1)
					{
						if (IsActive())	color = (message == SP_MOUSEIN ? Setup::GradientTextColor : Setup::TextColor);
						else		color = (message == SP_MOUSEIN ? Setup::GradientTextColor : Setup::GrayTextColor);
					}

					surface->Line(p1 + Point(0, 0), p2 + Point( 0,  0), color);
					surface->Line(p1 + Point(1, 0), p2 + Point( 0, -1), color);
					surface->Line(p1 + Point(0, 1), p2 + Point(-1,  0), color);

					p1 = Point(frame.left + frame.GetHeight() - offset - 1, frame.top    + offset) - (i == 1 ? Point(i, i) : Point());
					p2 = Point(frame.left			  + offset - 1, frame.bottom - offset) - (i == 1 ? Point(i, i) : Point());

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
	if (compType == Object::classID	   ||
	    compType == Widget::classID	   ||
	    compType == MenuEntry::classID ||
	    compType == PopupMenuEntry::classID) return True;
	else					 return False;
}
