 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/progressbar.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/color.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Progressbar::classID = S::Object::RequestClassID();

S::GUI::Progressbar::Progressbar(Point iPos, Size iSize, Int subType, Int iTextFlag, Int rangeStart, Int rangeEnd, Int iValue)
{
	type		= classID;
	subtype		= subType;
	startValue	= rangeStart;
	endValue	= rangeEnd;
	textFlag	= iTextFlag;

	SetValue(iValue);

	font.SetColor(Setup::ClientTextColor);

	possibleContainers.AddEntry(Layer::classID);

	pos		= iPos;
	size		= iSize;

	if (subtype == OR_HORZ)
	{
		if (size.cx == 0) size.cx = 80;
		if (size.cy == 0) size.cy = 19;
	}
	else
	{
		if (size.cx == 0) size.cx = 19;
		if (size.cy == 0) size.cy = 80;
	}

	CreateGradient(size);
}

S::GUI::Progressbar::~Progressbar()
{
}

S::Int S::GUI::Progressbar::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface	= container->GetDrawSurface();
	Point	 realPos	= GetRealPosition();
	Rect	 frame		= Rect(realPos, size);

	if (value == prevValue && message == SP_UPDATE) return Success;

	surface->StartPaint(frame);

	if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
	else		surface->Box(frame, Setup::BackgroundColor, FILLED);

	surface->Frame(frame, FRAME_DOWN);

	if (value > 0)
	{
		frame.left++;
		frame.top++;
		frame.right--;
		frame.bottom--;

		if (subtype == OR_HORZ)	frame.right = frame.left   + (Int) ((size.cx - 2) / ((Float) (endValue - startValue) / (Float) (value - startValue)));
		else			frame.top   = frame.bottom - (Int) ((size.cy - 2) / ((Float) (endValue - startValue) / (Float) (value - startValue)));

		gradient.BlitToSurface(Rect(Point(0, 0), Size(frame.right - frame.left, frame.bottom - frame.top)), surface, frame);
	}

	if (subtype == OR_HORZ && textFlag != PB_NOTEXT)
	{
		switch (textFlag)
		{
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
		Rect	 textRect	= Rect(Point(realPos.x + (size.cx / 2) - (textSize / 2), realPos.y + 2), Size(textSize, size.cy - 3));

		surface->SetText(text, textRect, font);

		if (value > 0)
		{
			textRect.right = realPos.x + (Int) ((size.cx - 2) / ((Float) (endValue - startValue) / (Float) (value - startValue)));

			Font	 nFont = font;

			nFont.SetColor(Setup::GradientTextColor);

			surface->SetText(text, textRect, nFont);
		}
	}

	surface->EndPaint();

	return Success;
}

S::Int S::GUI::Progressbar::SetMetrics(Point nPos, Size nSize)
{
	CreateGradient(nSize);

	return Widget::SetMetrics(nPos, nSize);
}

S::Int S::GUI::Progressbar::SetValue(Int newValue)
{
	value = (Int) Math::Min(endValue, Math::Max(startValue, newValue));

	Paint(SP_UPDATE);

	prevValue = value;

	return Success;
}

S::Int S::GUI::Progressbar::GetValue()
{
	return value;
}

S::Void S::GUI::Progressbar::CreateGradient(Size gSize)
{
	gradient.CreateBitmap(gSize.cx - 2, gSize.cy - 2, 32);

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
			for (Int y = 0; y < gSize.cy - 2; y++) gradient.SetPixel(x, y, Color((Int) (rs + rp * x), (Int) (gs + gp * x), (Int) (bs + bp * x)));
		}
	}
	else
	{
		for (Int y = 0; y < gSize.cy - 2; y++)
		{
			for (Int x = 0; x < gSize.cx - 2; x++) gradient.SetPixel(x, gSize.cy - 3 - y, Color((Int) (rs + rp * y), (Int) (gs + gp * y), (Int) (bs + bp * y)));
		}
	}
}
