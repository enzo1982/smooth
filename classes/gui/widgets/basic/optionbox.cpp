 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
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

				Int	 lightColor = (IsActive() ? Setup::ClientColor : Setup::BackgroundColor);
				Int	 inset	    = 2 * scaleFactor;

				Point	 lineStart  = Point(frame.left, frame.top) + Point(3, 3) * scaleFactor + Point(inset - (IsRightToLeft() ? 0 : 1), 0);
				Point	 lineEnd    = lineStart + Point(9 * scaleFactor - 2 * inset, 0);

				surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);

				for (Int i = 0; i < 9 * scaleFactor; i++)
				{
					if	(i <=			  inset) { lineStart.x--; lineEnd.x++; }
					else if (i >= (9 * scaleFactor) - inset) { lineStart.x++; lineEnd.x--; }

					lineStart.y++;
					lineEnd.y++;

					surface->Line(lineStart, lineEnd, lightColor);

					Int	 leftColor  = (i >= (9 * scaleFactor) - inset) ? Setup::DividerLightColor : (IsRightToLeft() ? Setup::DividerLightColor : Setup::DividerDarkColor);
					Int	 rightColor = (i >= (9 * scaleFactor) - inset) ? Setup::DividerLightColor : (IsRightToLeft() ? Setup::DividerDarkColor : Setup::DividerLightColor);	

					surface->SetPixel(lineStart, leftColor);
					surface->SetPixel(lineEnd - Point(1, 0), rightColor);
				}

				lineStart.x++;
				lineStart.y++;
				lineEnd.x--;
				lineEnd.y++;

				surface->Line(lineStart, lineEnd, Setup::DividerLightColor);

				if (*variable == code)
				{
					for (Int j = 0; j < 2; j++)
					{
						Int	 color	= IsActive() ? Setup::ClientTextColor : Setup::InactiveTextColor;
						Point	 offset	= Point(IsRightToLeft() ? 1 : 0, 0);

						if (j == 0)
						{
							color  = IsActive() ? Setup::DividerDarkColor : Setup::DividerDarkColor.Average(Setup::BackgroundColor);
							offset += Point(IsRightToLeft() ? -1 : 1, 1);
						}

						Int	 inset	    = 1 * scaleFactor;

						Point	 lineStart  = Point(frame.left, frame.top) + Point(3, 3) * scaleFactor + Point(2, 2) * scaleFactor + offset + Point(inset, 0);
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
