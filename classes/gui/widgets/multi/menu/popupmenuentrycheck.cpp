 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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

				if (*bVar == True)
				{
					Point p1 = Point(frame.left + 4, frame.top + 4);
					Point p2 = Point(frame.left + 11, frame.bottom - 4);

					for (Int i = 0; i < 2; i++)
					{
						Int	 color = Setup::DividerDarkColor;

						if (i == 1)
						{
							if (IsActive())	color = Setup::TextColor;
							else		color = Setup::GrayTextColor;

							p1 -= Point(i, i);
							p2 -= Point(i, i);
						}

						surface->Line(p1 + Point(0, 0), p2 + Point(0, 0), color);
						surface->Line(p1 + Point(1, 0), p2 + Point(0, -1), color);
						surface->Line(p1 + Point(0, 1), p2 + Point(-1, 0), color);
						surface->Line(p1 + Point(6, 0), p2 + Point(-8, 0), color);
						surface->Line(p1 + Point(6, 1), p2 + Point(-7, 0), color);
						surface->Line(p1 + Point(5, 0), p2 + Point(-8, -1), color);
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

				if (*bVar == True)
				{
					Point p1 = Point(frame.left + 4, frame.top + 4);
					Point p2 = Point(frame.left + 11, frame.bottom - 4);

					for (Int i = 0; i < 2; i++)
					{
						Int	 color = Setup::InactiveGradientTextColor;

						if (i == 1)
						{
							color = Setup::GradientTextColor;

							p1 -= Point(i, i);
							p2 -= Point(i, i);
						}

						surface->Line(p1 + Point(0, 0), p2 + Point(0, 0), color);
						surface->Line(p1 + Point(1, 0), p2 + Point(0, -1), color);
						surface->Line(p1 + Point(0, 1), p2 + Point(-1, 0), color);
						surface->Line(p1 + Point(6, 0), p2 + Point(-8, 0), color);
						surface->Line(p1 + Point(6, 1), p2 + Point(-7, 0), color);
						surface->Line(p1 + Point(5, 0), p2 + Point(-8, -1), color);
					}
				}
			}

			break;
	}

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
