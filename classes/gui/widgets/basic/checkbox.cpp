 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/checkbox.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int		 S::GUI::CheckBox::classID = S::Object::RequestClassID();

S::Signal0<S::Void>	 S::GUI::CheckBox::internalCheckValues;

S::GUI::CheckBox::CheckBox(String iText, Point iPos, Size iSize, Bool *iVariable)
{
	type		= classID;
	text		= iText;
	variable	= iVariable;
	state		= *variable;

	font.SetColor(Setup::ClientTextColor);

	possibleContainers.AddEntry(Layer::classID);

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = 80;
	if (size.cy == 0) size.cy = 17;

	GetTextSize();

	onLeftButtonClick.Connect(&CheckBox::OnLeftButtonClick, this);

	internalCheckValues.Connect(&CheckBox::InternalCheckValues, this);
}

S::GUI::CheckBox::~CheckBox()
{
	internalCheckValues.Disconnect(&CheckBox::InternalCheckValues, this);
}

S::Int S::GUI::CheckBox::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface	= container->GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), size);
	
	switch (message)
	{
		default:
		case SP_PAINT:
		case SP_UPDATE:
			frame = Rect(GetRealPosition() + Point(3, 3), Size(11, 11));

			if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
			else		surface->Box(frame, Setup::BackgroundColor, FILLED);

			surface->Frame(frame, FRAME_DOWN);

			if (*variable == True)
			{
				Int	 shadowColor	= 0;
				Int	 crossColor	= 0;

				if (active)
				{
					shadowColor	= Setup::DividerDarkColor;
					crossColor	= Setup::ClientTextColor;
				}
				else
				{
					shadowColor	= Setup::DividerDarkColor;
					crossColor	= Setup::GrayTextColor;
				}

				Point	 lineStart;
				Point	 lineEnd;

				lineStart.x	= frame.left + 3 - (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 3;
				lineEnd.x	= frame.right - 1 - (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 1;

				surface->Line(lineStart, lineEnd, shadowColor);

				lineStart.x	= frame.left + 4 - (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 3;
				lineEnd.x	= frame.right - 1 - (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 2;

				surface->Line(lineStart, lineEnd, shadowColor);

				lineStart.x	= frame.left + 3 - (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 4;
				lineEnd.x	= frame.right - 2 - (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 1;

				surface->Line(lineStart, lineEnd, shadowColor);

				lineStart.x	= frame.right - 2 - (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 3;
				lineEnd.x	= frame.left + 2 - (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 1;

				surface->Line(lineStart, lineEnd, shadowColor);

				lineStart.x	= frame.right - 2 - (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 4;
				lineEnd.x	= frame.left + 3 - (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 1;

				surface->Line(lineStart, lineEnd, shadowColor);

				lineStart.x	= frame.right - 3 - (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 3;
				lineEnd.x	= frame.left + 2 - (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 2;

				surface->Line(lineStart, lineEnd, shadowColor);

				lineStart.x	= frame.left + 2 + (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 2;
				lineEnd.x	= frame.right - 2 + (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 2;

				surface->Line(lineStart, lineEnd, crossColor);

				lineStart.x	= frame.left + 3 + (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 2;
				lineEnd.x	= frame.right - 2 + (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 3;

				surface->Line(lineStart, lineEnd, crossColor);

				lineStart.x	= frame.left + 2 + (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 3;
				lineEnd.x	= frame.right - 3 + (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 2;

				surface->Line(lineStart, lineEnd, crossColor);

				lineStart.x	= frame.right - 3 + (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 2;
				lineEnd.x	= frame.left + 1 + (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 2;

				surface->Line(lineStart, lineEnd, crossColor);

				lineStart.x	= frame.right - 3 + (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 3;
				lineEnd.x	= frame.left + 2 + (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 2;

				surface->Line(lineStart, lineEnd, crossColor);

				lineStart.x	= frame.right - 4 + (Setup::rightToLeft ? 1 : 0);
				lineStart.y	= frame.top + 2;
				lineEnd.x	= frame.left + 1 + (Setup::rightToLeft ? 1 : 0);
				lineEnd.y	= frame.bottom - 3;

				surface->Line(lineStart, lineEnd, crossColor);
			}

			if (message != SP_UPDATE)
			{
				Rect	 textRect;

				textRect.left	= frame.right + 3;
				textRect.top	= frame.top - 1;
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

S::Void S::GUI::CheckBox::OnLeftButtonClick()
{
	if (*variable == False)	*variable = True;
	else			*variable = False;

	internalCheckValues.Emit();
}

S::Void S::GUI::CheckBox::InternalCheckValues()
{
	if (state != *variable)
	{
		state = *variable;

		Paint(SP_UPDATE);
	}
}
