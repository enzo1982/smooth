 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/popupmenuentryoption.h>
#include <smooth/gui/widgets/special/shortcut.h>
#include <smooth/gui/widgets/basic/optionbox.h>
#include <smooth/graphics/color.h>
#include <smooth/graphics/surface.h>

const S::Short	 S::GUI::PopupMenuEntryOption::classID = S::Object::RequestClassID();

S::GUI::PopupMenuEntryOption::PopupMenuEntryOption(const String &iText, Int *iiVar, Int iiCode) : PopupMenuEntry(iText)
{
	type	= classID;

	iVar	= iiVar;
	iCode	= iiCode;
}

S::GUI::PopupMenuEntryOption::~PopupMenuEntryOption()
{
}

S::Int S::GUI::PopupMenuEntryOption::Paint(Int message)
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
			if (*iVar == iCode)
			{
				Float	 scaleFactor = surface->GetSurfaceDPI() / 96.0;

				for (Int j = 0; j < 2; j++)
				{
					Color	 color	= (IsActive() ? Setup::TextColor : Setup::InactiveTextColor);
					Point	 offset	= Point(IsRightToLeft() ? 1 : 0, 0);

					if (message == SP_MOUSEIN) color = Setup::GradientTextColor;

					if (j == 0)
					{
						color  = Color(color.GetRed() + (255 - color.GetRed()) * 0.6, color.GetGreen() + (255 - color.GetGreen()) * 0.6, color.GetBlue() + (255 - color.GetBlue()) * 0.6);
						offset = Point(IsRightToLeft() ? -1 : 1, 1);
					}

					Int	 inset	    = 1 * scaleFactor;

					Point	 lineStart  = Point(frame.left, frame.top) + Point(4, 4) * scaleFactor + offset + Point(inset + 2, 0);
					Point	 lineEnd    = lineStart + Point(5 * scaleFactor - 2 * inset - 2, 0);

					for (Int i = 0; i < 5 * scaleFactor; i++)
					{
						if	(i <=			  inset) { lineStart.x--; lineEnd.x++; }
						else if (i >= (5 * scaleFactor) - inset) { lineStart.x++; lineEnd.x--; }

						lineStart.y++;
						lineEnd.y++;

						surface->Line(lineStart, lineEnd, color);
					}
				}
			}

			break;
	}

	surface->EndPaint();

	return Success();
}

S::Void S::GUI::PopupMenuEntryOption::OnClickEntry()
{
	*iVar = iCode;

	OptionBox::internalCheckValues.Emit();

	PopupMenuEntry::OnClickEntry();
}

S::Bool S::GUI::PopupMenuEntryOption::IsTypeCompatible(Short compType) const
{
	if (compType == PopupMenuEntry::classID) return True;
	else					 return PopupMenuEntry::IsTypeCompatible(compType);
}
