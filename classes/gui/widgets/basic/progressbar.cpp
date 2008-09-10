 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/progressbar.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/color.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Progressbar::classID = S::Object::RequestClassID();

S::GUI::Progressbar::Progressbar(const Point &iPos, const Size &iSize, Int sType, Int iTextFlag, Int rangeStart, Int rangeEnd, Int iValue) : Widget(iPos, iSize)
{
	type		= classID;
	subtype		= sType;
	startValue	= rangeStart;
	endValue	= rangeEnd;

	SetFlags(iTextFlag);
	SetValue(iValue);

	font.SetColor(Setup::ClientTextColor);

	if (GetWidth() == 0) SetWidth(subtype == OR_VERT ? 19 : 80);
	if (GetHeight() == 0) SetHeight(subtype == OR_VERT ? 80 : 19);

	CreateGradient(GetSize());
}

S::GUI::Progressbar::~Progressbar()
{
}

S::Int S::GUI::Progressbar::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface	= container->GetDrawSurface();
	Point	 realPos	= GetRealPosition();
	Rect	 frame		= Rect(realPos, GetSize());

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
		case SP_UPDATE:
			surface->StartPaint(GetVisibleArea());

			if (IsActive())	surface->Box(frame + Point(1, 1) - Size(2, 2), Setup::ClientColor, Rect::Filled);
			else		surface->Box(frame + Point(1, 1) - Size(2, 2), Setup::BackgroundColor, Rect::Filled);

			if (message != SP_UPDATE) surface->Frame(frame, FRAME_DOWN);

			if (value > 0)
			{
				frame = frame + Point(1, 1) - Size(2, 2);

				if (subtype == OR_HORZ)	frame.right = frame.left   + (Int) ((GetWidth() - 2) / ((Float) (endValue - startValue) / (Float) (value - startValue)));
				else			frame.top   = frame.bottom - (Int) ((GetHeight() - 2) / ((Float) (endValue - startValue) / (Float) (value - startValue)));

				gradient.BlitToSurface(Rect(Point(0, 0), Size(frame.right - frame.left, frame.bottom - frame.top)), surface, frame);
			}

			if (subtype == OR_HORZ)
			{
				switch (GetFlags())
				{
					case PB_NOTEXT:
						break;
					case PB_VALUE:
						text = String::FromInt((Int) Math::Max(value, startValue));
						break;
					case PB_PERCENT:
						if (value > 0)	text = String::FromInt((Int) Math::Max(0, Math::Round(100 / ((Float) (endValue - startValue) / (Float) (value - startValue)))));
						else		text = "0";

						text.Append("%");
						break;
				}

				Int	 textSize	= font.GetTextSizeX(text);
				Rect	 textRect	= Rect(Point(realPos.x + (GetWidth() / 2) - (textSize / 2), realPos.y + 2), Size(textSize, GetHeight() - 3));

				surface->SetText(text, textRect, font);

				if (value > 0)
				{
					textRect.right = realPos.x + (Int) ((GetWidth() - 2) / ((Float) (endValue - startValue) / (Float) (value - startValue)));

					Font	 nFont = font;

					nFont.SetColor(Setup::GradientTextColor);

					surface->SetText(text, textRect, nFont);
				}
			}

			surface->EndPaint();

			break;
	}

	return Success();
}

S::Int S::GUI::Progressbar::SetMetrics(const Point &nPos, const Size &nSize)
{
	CreateGradient(nSize);

	return Widget::SetMetrics(nPos, nSize);
}

S::Int S::GUI::Progressbar::SetValue(Int newValue)
{
	Int	 prevValue = value;

	value = (Int) Math::Min(endValue, Math::Max(startValue, newValue));

	if (prevValue != value) Paint(SP_UPDATE);

	return Success();
}

S::Int S::GUI::Progressbar::GetValue()
{
	return value;
}

S::Void S::GUI::Progressbar::CreateGradient(const Size &gSize)
{
	gradient.CreateBitmap(gSize.cx - 2, gSize.cy - 2, 24);

	Int	 rs	= Setup::GradientStartColor.GetRed();
	Int	 gs	= Setup::GradientStartColor.GetGreen();
	Int	 bs	= Setup::GradientStartColor.GetBlue();
	Float	 rp	= ((Float) (Setup::GradientEndColor.GetRed() - rs)) / ((subtype == OR_HORZ) ? (gSize.cx - 2) : (gSize.cy - 2));
	Float	 gp	= ((Float) (Setup::GradientEndColor.GetGreen() - gs)) / ((subtype == OR_HORZ) ? (gSize.cx - 2) : (gSize.cy - 2));
	Float	 bp	= ((Float) (Setup::GradientEndColor.GetBlue() - bs)) / ((subtype == OR_HORZ) ? (gSize.cx - 2) : (gSize.cy - 2));

	if (subtype == OR_HORZ)
	{
		for (Int x = 0; x < gSize.cx - 2; x++)
		{
			for (Int y = 0; y < gSize.cy - 2; y++) gradient.SetPixel(Point(x, y), Color((Int) (rs + rp * x), (Int) (gs + gp * x), (Int) (bs + bp * x)));
		}
	}
	else
	{
		for (Int y = 0; y < gSize.cy - 2; y++)
		{
			for (Int x = 0; x < gSize.cx - 2; x++) gradient.SetPixel(Point(x, gSize.cy - 3 - y), Color((Int) (rs + rp * y), (Int) (gs + gp * y), (Int) (bs + bp * y)));
		}
	}
}
