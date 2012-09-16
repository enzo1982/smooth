 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
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
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	PopupMenuEntry::Paint(message);

	Surface	*surface = GetDrawSurface();
	Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
		case SP_MOUSEIN:
		case SP_MOUSEOUT:
			if (*iVar == iCode)
			{
				Point	 point = Point(frame.left, frame.top) + Point(5, 5) * surface->GetSurfaceDPI() / 96.0;

				for (Int i = 0; i < 2; i++)
				{
					Int	 color = (message == SP_MOUSEIN ? Setup::InactiveGradientTextColor : Setup::DividerDarkColor);

					if (i == 1)
					{
						if (IsActive())	color = (message == SP_MOUSEIN ? Setup::GradientTextColor : Setup::TextColor);
						else		color = (message == SP_MOUSEIN ? Setup::GradientTextColor : Setup::GrayTextColor);

						point -= Point(i, i);
					}

					surface->Box(Rect(point + Point(1, 0), Size(3, 3) * surface->GetSurfaceDPI() / 96.0 + Size(0, 2)), color, Rect::Filled);
					surface->Box(Rect(point + Point(0, 1), Size(3, 3) * surface->GetSurfaceDPI() / 96.0 + Size(2, 0)), color, Rect::Filled);
				}
			}

			break;
	}

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
	if (compType == Object::classID	   ||
	    compType == Widget::classID	   ||
	    compType == MenuEntry::classID ||
	    compType == PopupMenuEntry::classID) return True;
	else					 return False;
}
