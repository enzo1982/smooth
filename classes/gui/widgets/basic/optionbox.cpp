 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
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
					surface->SetText(text, frame + Point(frame.GetHeight(), 2), nFont);
				}

				Int	 lightColor;

				if (IsActive())	lightColor = Setup::ClientColor;
				else		lightColor = Setup::BackgroundColor;

				Point	 lineStart	= GetRealPosition() + Point(3, 3) * surface->GetSurfaceDPI() / 96.0 + Point(3 + (IsRightToLeft() ? 1 : 0), 0);
				Point	 lineEnd	= lineStart + Point(5 * surface->GetSurfaceDPI() / 96.0, 0);

				surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);

				lineStart.x--;
				lineStart.y++;
				lineEnd.x++;
				lineEnd.y++;

				surface->Line(lineStart, lineEnd, lightColor);

				surface->SetPixel(lineStart, (IsRightToLeft() ? Setup::DividerLightColor : Setup::DividerDarkColor));
				surface->SetPixel(lineEnd - Point(1, 0), (IsRightToLeft() ? Setup::DividerDarkColor : Setup::DividerLightColor));

				lineStart.x--;
				lineStart.y++;
				lineEnd.x++;
				lineEnd.y++;

				surface->Line(lineStart, lineEnd, lightColor);

				surface->SetPixel(lineStart, (IsRightToLeft() ? Setup::DividerLightColor : Setup::DividerDarkColor));
				surface->SetPixel(lineEnd - Point(1, 0), (IsRightToLeft() ? Setup::DividerDarkColor : Setup::DividerLightColor));

				lineStart.x--;
				lineEnd.x++;

				for (Int i = 0; i < 5 * surface->GetSurfaceDPI() / 96.0; i++)
				{
					lineStart.y++;
					lineEnd.y++;

					surface->Line(lineStart, lineEnd, lightColor);

					surface->SetPixel(lineStart, (IsRightToLeft() ? Setup::DividerLightColor : Setup::DividerDarkColor));
					surface->SetPixel(lineEnd - Point(1, 0), (IsRightToLeft() ? Setup::DividerDarkColor : Setup::DividerLightColor));
				}

				lineStart.x++;
				lineStart.y++;
				lineEnd.x--;
				lineEnd.y++;

				surface->Line(lineStart, lineEnd, lightColor);

				surface->SetPixel(lineStart, Setup::DividerLightColor);
				surface->SetPixel(lineEnd - Point(1, 0), Setup::DividerLightColor);

				lineStart.x++;
				lineStart.y++;
				lineEnd.x--;
				lineEnd.y++;

				surface->Line(lineStart, lineEnd, lightColor);

				surface->SetPixel(lineStart, Setup::DividerLightColor);
				surface->SetPixel(lineEnd - Point(1, 0), Setup::DividerLightColor);

				lineStart.x++;
				lineStart.y++;
				lineEnd.x--;
				lineEnd.y++;

				surface->Line(lineStart, lineEnd, Setup::DividerLightColor);

				if (*variable == code)
				{
					Point	 point = Point(frame.left - (IsRightToLeft() ? 2 : 0), frame.top) + Point(3, 3) * surface->GetSurfaceDPI() / 96.0 + Point(4, 4);

					for (Int j = 0; j < 2; j++)
					{
						Int	 color = IsActive() ? Setup::DividerDarkColor : Setup::DividerDarkColor.Average(Setup::BackgroundColor);

						if (j == 1)
						{
							color = IsActive() ? Setup::ClientTextColor : Setup::InactiveTextColor;

							point -= Point((IsRightToLeft() ? -j : j), j);
						}

						surface->Box(Rect(point + Point(1, 0), Size(3, 3) * surface->GetSurfaceDPI() / 96.0 + Size(0, 2)), color, Rect::Filled);
						surface->Box(Rect(point + Point(0, 1), Size(3, 3) * surface->GetSurfaceDPI() / 96.0 + Size(2, 0)), color, Rect::Filled);
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
