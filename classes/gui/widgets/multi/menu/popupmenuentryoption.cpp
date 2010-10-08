 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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

	Surface	*surface = GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame	 = Rect(realPos, GetSize());

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
		case SP_MOUSEOUT:
			{
				Rect	 textRect = Rect(realPos + Point(18, 0), GetSize() - Size(22, 2));
				Font	 nFont = font;

				if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

				surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
				surface->SetText(text, textRect, nFont);

				if (shortcut != NIL)
				{
					textRect.left = textRect.right - shortcutOffset;

					surface->SetText(shortcut->ToString(), textRect, nFont);
				}

				if (*iVar == iCode)
				{
					Point	 point = Point(frame.left + 5, frame.top + 5);

					for (Int i = 0; i < 2; i++)
					{
						Int	 color = Setup::DividerDarkColor;

						if (i == 1)
						{
							if (IsActive())	color = Setup::TextColor;
							else		color = Setup::GrayTextColor;

							point -= Point(i, i);
						}

						surface->Box(Rect(point + Point(1, 0), Size(3, 5)), color, Rect::Filled);
						surface->Box(Rect(point + Point(0, 1), Size(5, 3)), color, Rect::Filled);
					}
				}
			}

			break;
		case SP_MOUSEIN:
			{
				Rect	 textRect	= Rect(realPos + Point(18, 0), GetSize() - Size(22, 2));
				Font	 nFont		= font;

				nFont.SetColor(Setup::GradientTextColor);

				surface->Gradient(frame, Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
				surface->SetText(text, textRect, nFont);

				if (shortcut != NIL)
				{
					textRect.left = textRect.right - shortcutOffset;

					surface->SetText(shortcut->ToString(), textRect, nFont);
				}

				if (*iVar == iCode)
				{
					Point	 point = Point(frame.left + 5, frame.top + 5);

					for (Int i = 0; i < 2; i++)
					{
						Int	 color = Setup::InactiveGradientTextColor;

						if (i == 1)
						{
							color = Setup::GradientTextColor;

							point -= Point(i, i);
						}

						surface->Box(Rect(point + Point(1, 0), Size(3, 5)), color, Rect::Filled);
						surface->Box(Rect(point + Point(0, 1), Size(5, 3)), color, Rect::Filled);
					}
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
