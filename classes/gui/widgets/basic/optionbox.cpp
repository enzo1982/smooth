 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

const S::Int		 S::GUI::OptionBox::classID = S::Object::RequestClassID();

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

	RegisterObject(hotspot);
}

S::GUI::OptionBox::~OptionBox()
{
	DeleteObject(hotspot);

	internalCheckValues.Disconnect(&OptionBox::InternalCheckValues, this);
}

S::Int S::GUI::OptionBox::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface	= container->GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), GetSize());

	Point	 lineStart;
	Point	 lineEnd;

	Int	 lightColor;

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			if (IsActive())	lightColor = Setup::ClientColor;
			else		lightColor = Setup::BackgroundColor;

			lineStart	= GetRealPosition() + Point(6 + (IsRightToLeft() ? 1 : 0), 3);
			lineEnd		= lineStart + Point(5, 0);

			surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);

			lineStart.x--;
			lineStart.y++;
			lineEnd.x++;
			lineEnd.y++;

			surface->Line(lineStart, lineEnd, lightColor);

			surface->SetPixel(lineStart.x, lineStart.y, (IsRightToLeft() ? Setup::DividerLightColor : Setup::DividerDarkColor));
			surface->SetPixel(lineEnd.x - 1, lineEnd.y, (IsRightToLeft() ? Setup::DividerDarkColor : Setup::DividerLightColor));

			lineStart.x--;
			lineStart.y++;
			lineEnd.x++;
			lineEnd.y++;

			surface->Line(lineStart, lineEnd, lightColor);

			surface->SetPixel(lineStart.x, lineStart.y, (IsRightToLeft() ? Setup::DividerLightColor : Setup::DividerDarkColor));
			surface->SetPixel(lineEnd.x - 1, lineEnd.y, (IsRightToLeft() ? Setup::DividerDarkColor : Setup::DividerLightColor));

			lineStart.x--;
			lineEnd.x++;

			for (Int i = 0; i < 5; i++)
			{
				lineStart.y++;
				lineEnd.y++;

				surface->Line(lineStart, lineEnd, lightColor);

				surface->SetPixel(lineStart.x, lineStart.y, (IsRightToLeft() ? Setup::DividerLightColor : Setup::DividerDarkColor));
				surface->SetPixel(lineEnd.x - 1, lineEnd.y, (IsRightToLeft() ? Setup::DividerDarkColor : Setup::DividerLightColor));
			}

			lineStart.x++;
			lineStart.y++;
			lineEnd.x--;
			lineEnd.y++;

			surface->Line(lineStart, lineEnd, lightColor);

			surface->SetPixel(lineStart.x, lineStart.y, Setup::DividerLightColor);
			surface->SetPixel(lineEnd.x - 1, lineEnd.y, Setup::DividerLightColor);

			lineStart.x++;
			lineStart.y++;
			lineEnd.x--;
			lineEnd.y++;

			surface->Line(lineStart, lineEnd, lightColor);

			surface->SetPixel(lineStart.x, lineStart.y, Setup::DividerLightColor);
			surface->SetPixel(lineEnd.x - 1, lineEnd.y, Setup::DividerLightColor);

			lineStart.x++;
			lineStart.y++;
			lineEnd.x--;
			lineEnd.y++;

			surface->Line(lineStart, lineEnd, Setup::DividerLightColor);

			if (*variable == code)
			{
				Point	 point = Point(frame.left + 7 - (IsRightToLeft() ? 2 : 0), frame.top + 7);

				for (Int j = 0; j < 2; j++)
				{
					Int	 color = IsActive() ? Setup::DividerDarkColor : Setup::DividerDarkColor.Average(Setup::BackgroundColor);

					if (j == 1)
					{
						color = IsActive() ? Setup::ClientTextColor : Setup::GrayTextColor;

						point -= Point((IsRightToLeft() ? -j : j), j);
					}

					surface->Box(Rect(point + Point(1, 0), Size(3, 5)), color, FILLED);
					surface->Box(Rect(point + Point(0, 1), Size(5, 3)), color, FILLED);
				}
			}

			Font	 nFont = font;

			if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

			surface->SetText(text,  frame + Point(17, 2), nFont);

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

	Paint(SP_PAINT);
}
