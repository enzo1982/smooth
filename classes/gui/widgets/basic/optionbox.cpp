 /* The smooth Class Library
  * Copyright (C) 1998-2020 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/optionbox.h>
#include <smooth/gui/widgets/hotspot/simplebutton.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Short		 S::GUI::OptionBox::classID = S::Object::RequestClassID();

S::Signal0<S::Void>	 S::GUI::OptionBox::internalCheckValues;

S::GUI::OptionBox::OptionBox(const String &iText, const Point &iPos, const Size &iSize, Int *var, Int iCode) : Widget(iPos, iSize)
{
	type		= classID;
	text		= iText;
	variable	= var;
	code		= iCode;

	font.SetColor(Setup::ClientTextColor);

	if (*variable == code)	state = True;
	else			state = False;

	if (GetWidth() == 0) SetWidth(80);
	if (GetHeight() == 0) SetHeight(17);

	ComputeTextSize();

	internalCheckValues.Connect(&OptionBox::InternalCheckValues, this);

	hotspot	= new HotspotSimpleButton(Point(0, 0), GetSize());
	hotspot->onLeftButtonClick.Connect(&OptionBox::OnLeftButtonClick, this);
	hotspot->onLeftButtonClick.Connect(&onAction);

	onChangeSize.Connect(&HotspotSimpleButton::SetSize, hotspot);

	Add(hotspot);
}

S::GUI::OptionBox::~OptionBox()
{
	DeleteObject(hotspot);

	internalCheckValues.Disconnect(&OptionBox::InternalCheckValues, this);
}

S::Int S::GUI::OptionBox::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	switch (message)
	{
		case SP_PAINT:
		case SP_UPDATE:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

				surface->StartPaint(frame);

				if (message == SP_PAINT)
				{
					Font	 nFont = font;

					if (!IsActive()) nFont.SetColor(Setup::InactiveTextColor);

					surface->Box(frame, GetBackgroundColor(), Rect::Filled);
					surface->SetText(text, frame + Point(frame.GetHeight(), Math::Ceil(Float(frame.GetHeight() - scaledTextSize.cy) / 2) - 1), nFont);
				}

				Float	 scaleFactor = surface->GetSurfaceDPI() / 96.0;
				Int	 inset	     = 3 * scaleFactor;

				Point	 lineStart   = Point(frame.left, frame.top) + Point(3, 3) * scaleFactor + Point(inset + (IsRightToLeft() ? 1 : 0), 0);
				Point	 lineEnd     = lineStart + Point(11 * scaleFactor - 2 * inset, 0);

				Int	 lightColor  = Color(Math::Min(Setup::BackgroundColor.GetRed() + 64, 255), Math::Min(Setup::BackgroundColor.GetGreen() + 64, 255), Math::Min(Setup::BackgroundColor.GetBlue() + 64, 255));
				Int	 darkColor   = Color(Math::Max(Setup::BackgroundColor.GetRed() - 64, 0), Math::Max(Setup::BackgroundColor.GetGreen() - 64, 0), Math::Max(Setup::BackgroundColor.GetBlue() - 64, 0));

				Int	 innerColor  = (IsActive() ? Setup::ClientColor : Setup::BackgroundColor);

				surface->Line(lineStart, lineEnd, darkColor);

				for (Int i = 0; i < Int(11 * scaleFactor) - 2; i++)
				{
					if	(i <			      inset) { lineStart.x--; lineEnd.x++; }
					else if (i > (11 * scaleFactor) - 2 - inset) { lineStart.x++; lineEnd.x--; }

					lineStart.y++;
					lineEnd.y++;

					surface->Line(lineStart, lineEnd, innerColor);

					Int	 leftColor  = (i > (11 * scaleFactor) - 2 - inset) ? lightColor : (IsRightToLeft() ? lightColor : darkColor);
					Int	 rightColor = (i > (11 * scaleFactor) - 2 - inset) ? lightColor : (IsRightToLeft() ? darkColor : lightColor);

					surface->SetPixel(lineStart, leftColor);
					surface->SetPixel(lineEnd - Point(1, 0), rightColor);
				}

				lineStart.x++;
				lineStart.y++;
				lineEnd.x--;
				lineEnd.y++;

				surface->Line(lineStart, lineEnd, lightColor);

				if (*variable == code)
				{
					for (Int i = 0; i < 2; i++)
					{
						Int	 color	= IsActive() ? Setup::DividerDarkColor : Setup::DividerDarkColor.Average(Setup::BackgroundColor);
						Point	 offset	= Point(IsRightToLeft() ? 0 : 1, 0);

						if (i == 1)
						{
							color	= IsActive() ? Setup::ClientTextColor : Setup::InactiveTextColor;
							offset -= Point(IsRightToLeft() ? -i : i, i);
						}

						Int	 inset	    = 2 * scaleFactor;

						Point	 lineStart  = Point(frame.left, frame.top) + Point(6, 6) * scaleFactor + offset + Point(inset, 0);
						Point	 lineEnd    = lineStart + Point(5 * scaleFactor - 2 * inset, 0);

						for (Int j = 0; j < Int(5 * scaleFactor); j++)
						{
							if	(j <			 inset) { lineStart.x--; lineEnd.x++; }
							else if (j > (5 * scaleFactor) - inset) { lineStart.x++; lineEnd.x--; }

							lineStart.y++;
							lineEnd.y++;

							surface->Line(lineStart, lineEnd, color);
						}
					}
				}

				surface->EndPaint();
			}

			break;
	}

	return Success();
}

S::Void S::GUI::OptionBox::OnLeftButtonClick()
{
	*variable = code;

	internalCheckValues.Emit();
}

S::Void S::GUI::OptionBox::InternalCheckValues()
{
	state = (*variable == code ? True : False);

	Paint(SP_UPDATE);
}
