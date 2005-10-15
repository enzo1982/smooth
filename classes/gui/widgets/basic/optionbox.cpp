 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/optionbox.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int		 S::GUI::OptionBox::classID = S::Object::RequestClassID();

S::Signal0<S::Void>	 S::GUI::OptionBox::internalCheckValues;

S::GUI::OptionBox::OptionBox(String iText, Point iPos, Size iSize, Int *var, Int iCode)
{
	type		= classID;
	text		= iText;
	variable	= var;
	code		= iCode;

	font.SetColor(Setup::ClientTextColor);

	if (*variable == code)	state = True;
	else			state = False;

	possibleContainers.AddEntry(Layer::classID);

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = 80;
	if (size.cy == 0) size.cy = 17;

	GetTextSize();

	onLeftButtonClick.Connect(&OptionBox::OnLeftButtonClick, this);

	internalCheckValues.Connect(&OptionBox::InternalCheckValues, this);
}

S::GUI::OptionBox::~OptionBox()
{
	internalCheckValues.Disconnect(&OptionBox::InternalCheckValues, this);
}

S::Int S::GUI::OptionBox::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface	= container->GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), size);

	Point	 lineStart;
	Point	 lineEnd;

	Int	 lightColor;
	Int	 i;

	switch (message)
	{
		default:
		case SP_PAINT:
		case SP_UPDATE:
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
				Int	 shadowColor	= 0;
				Int	 dotColor	= 0;

				if (active)
				{
					shadowColor	= Setup::DividerDarkColor;
					dotColor	= Setup::ClientTextColor;
				}
				else
				{
					shadowColor	= Setup::DividerDarkColor.Average(Setup::BackgroundColor);
					dotColor	= Setup::GrayTextColor;
				}

				lineStart.x++;
				lineStart.y -= 7;
				lineEnd.x--;
				lineEnd.y -= 7;

				surface->Line(lineStart, lineEnd, dotColor);

				lineStart.x--;
				lineEnd.x++;

				for (i = 0; i < 3; i++)
				{
					lineStart.y++;
					lineEnd.y++;

					surface->Line(lineStart, lineEnd, dotColor);
				}

				surface->SetPixel(lineEnd.x, lineEnd.y - 1, shadowColor);
				surface->SetPixel(lineEnd.x, lineEnd.y, shadowColor);

				lineStart.x++;
				lineStart.y++;
				lineEnd.y++;

				surface->Line(lineStart, lineEnd, dotColor);

				surface->SetPixel(lineEnd.x - 1, lineEnd.y, shadowColor);
				surface->SetPixel(lineEnd.x, lineEnd.y, shadowColor);

				lineStart.x++;
				lineStart.y++;
				lineEnd.y++;

				surface->Line(lineStart, lineEnd, shadowColor);
			}

			if (message != SP_UPDATE)
			{
				Rect	 textRect;

				textRect.left	= frame.left + 17;
				textRect.top	= frame.top + 2;
				textRect.right	= textRect.left + size.cx;
				textRect.bottom	= textRect.top + 20;

				Font	 nFont = font;

				if (!active) nFont.SetColor(Setup::GrayTextColor);

				surface->SetText(text, textRect, nFont);
			}

			break;
		case SP_MOUSEUP:
		case SP_MOUSEIN:
			surface->Frame(frame, FRAME_UP);

			break;
		case SP_MOUSEOUT:
			surface->Box(frame, Setup::BackgroundColor, OUTLINED);

			break;
		case SP_MOUSEDOWN:
			surface->Frame(frame, FRAME_DOWN);

			break;
	}

	return Success;
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
