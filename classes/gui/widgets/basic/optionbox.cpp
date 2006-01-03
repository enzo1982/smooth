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

	GetTextSize();

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

	EnterProtectedRegion();

	Surface	*surface	= container->GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), GetSize());

	Point	 lineStart;
	Point	 lineEnd;

	Int	 lightColor;
	Int	 i;

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			if (active)	lightColor = Setup::ClientColor;
			else		lightColor = Setup::BackgroundColor;

			lineStart	= GetRealPosition() + Point(6, 3);
			lineEnd		= lineStart + Point(5, 0);

			surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);

			lineStart.x--;
			lineStart.y++;
			lineEnd.y++;

			surface->Line(lineStart, lineEnd, lightColor);

			surface->SetPixel(lineStart.x, lineStart.y, Setup::DividerDarkColor);

			lineStart.x--;
			lineStart.y++;
			lineEnd.x += 2;
			lineEnd.y++;

			surface->Line(lineStart, lineEnd, lightColor);

			surface->SetPixel(lineStart.x, lineStart.y, Setup::DividerDarkColor);
			surface->SetPixel(lineEnd.x - 1, lineEnd.y, Setup::DividerLightColor);

			lineStart.x--;
			lineEnd.x++;

			for (i = 0; i < 5; i++)
			{
				lineStart.y++;
				lineEnd.y++;

				surface->Line(lineStart, lineEnd, lightColor);

				surface->SetPixel(lineStart.x, lineStart.y, Setup::DividerDarkColor);
				surface->SetPixel(lineEnd.x - 1, lineEnd.y, Setup::DividerLightColor);
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
				Point	 point = Point(frame.left + 7, frame.top + 7);

				for (Int i = 0; i < 2; i++)
				{
					Int	 color = active ? Setup::DividerDarkColor : Setup::DividerDarkColor.Average(Setup::BackgroundColor);

					if (i == 1)
					{
						color = active ? Setup::ClientTextColor : Setup::GrayTextColor;

						point -= Point(i, i);
					}

					surface->Box(Rect(point + Point(1, 0), Size(3, 5)), color, FILLED);
					surface->Box(Rect(point + Point(0, 1), Size(5, 3)), color, FILLED);
				}
			}

			Rect	 textRect;

			textRect.left	= frame.left + 17;
			textRect.top	= frame.top + 2;
			textRect.right	= textRect.left + GetWidth();
			textRect.bottom	= textRect.top + 20;

			Font	 nFont = font;

			if (!active) nFont.SetColor(Setup::GrayTextColor);

			surface->SetText(text, textRect, nFont);

			break;
	}

	LeaveProtectedRegion();

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
