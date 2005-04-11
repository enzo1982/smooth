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
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/color.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Progressbar::classID = S::Object::RequestClassID();

S::GUI::Progressbar::Progressbar(Point iPos, Size iSize, Int subType, Int iTextFlag, Int rangeStart, Int rangeEnd, Int iValue)
{
	type		= classID;
	subtype		= subType;
	startValue	= rangeStart;
	endValue	= rangeEnd;
	textFlag	= iTextFlag;
	prevValue	= (Int) Math::Min(endValue, Math::Max(startValue, iValue));
	value		= (Int) Math::Min(endValue, Math::Max(startValue, iValue));

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
}

S::GUI::Progressbar::~Progressbar()
{
}

S::Int S::GUI::Progressbar::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface	*surface	= container->GetDrawSurface();
	Point	 realPos	= GetRealPosition();
	Rect	 frame		= Rect(GetRealPosition(), size);

	Int	 rs		= GetRed(Setup::GradientStartColor);
	Int	 gs		= GetGreen(Setup::GradientStartColor);
	Int	 bs		= GetBlue(Setup::GradientStartColor);
	Float	 rp		= ((Float) (GetRed(Setup::GradientEndColor) - rs)) / (size.cx - 2);
	Float	 gp		= ((Float) (GetGreen(Setup::GradientEndColor) - gs)) / (size.cx - 2);
	Float	 bp		= ((Float) (GetBlue(Setup::GradientEndColor) - bs)) / (size.cx - 2);

	value = (Int) Math::Min(endValue, Math::Max(startValue, value));

	if (value == prevValue && message == SP_UPDATE) return Success;

	surface->StartPaint(frame);

	if (message != SP_UPDATE || value == 0)
	{
		if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
		else		surface->Box(frame, Setup::BackgroundColor, FILLED);

		surface->Frame(frame, FRAME_DOWN);
	}

	frame.left++;
	frame.top++;
	frame.right--;
	frame.bottom--;

	if (value > 0)
	{
		Int	 minI = (Int) (((subtype == OR_HORZ ? size.cx : size.cy) - 2) / ((Float) (endValue - startValue) / (Float) (prevValue - startValue)));
		Int	 maxI = (Int) (((subtype == OR_HORZ ? size.cx : size.cy) - 2) / ((Float) (endValue - startValue) / (Float) (value - startValue)));

		if (minI < maxI || message != SP_UPDATE)
		{
			for (Int i = (message == SP_UPDATE) ? minI : 0; i < maxI; i++)
			{
				Point	 lineStart	= (subtype == OR_HORZ) ? Point(frame.left + i, frame.top)    : Point(frame.left + i, frame.top);
				Point	 lineEnd	= (subtype == OR_HORZ) ? Point(frame.left + i, frame.bottom) : Point(frame.left + i, frame.bottom);
				Int	 color		= Setup::GradientStartColor;

				if (i > 0) color = CombineColor((Int) (rs + rp * i), (Int) (gs + gp * i), (Int) (bs + bp * i));

				surface->Line(lineStart, lineEnd, color);
			}
		}
		else if (maxI < minI)
		{
			Rect	 eFrame = frame;

			if (subtype == OR_HORZ)
			{
				eFrame.left += maxI;
				eFrame.right = eFrame.left + minI;
			}
			else
			{
				eFrame.bottom -= maxI;
				eFrame.top = eFrame.bottom - minI;
			}

			if (active)	surface->Box(eFrame, Setup::ClientColor, FILLED);
			else		surface->Box(eFrame, Setup::BackgroundColor, FILLED);
		}
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
		Rect	 textRect;

		textRect.left	= (frame.right + frame.left) / 2 - textSize / 2;
		textRect.top	= realPos.y + 2;
		textRect.bottom	= realPos.y + size.cy - 1;
		textRect.right	= textRect.left + textSize;

		Int	 maxI = (Int) ((size.cx - 2) / ((Float) (endValue - startValue) / (Float) (value - startValue)));

		for (Int i = textRect.left - frame.left - 5; i < textRect.right - frame.left + 5; i++)
		{
			Point	 lineStart	= Point(frame.left + i, frame.top);
			Point	 lineEnd	= Point(frame.left + i, frame.bottom);
			Int	 color		= Setup::GradientStartColor;

			if (i < maxI)	color = CombineColor((Int) (rs + rp * i), (Int) (gs + gp * i), (Int) (bs + bp * i));
			else		color = Setup::ClientColor;

			surface->Line(lineStart, lineEnd, color);
		}

		surface->SetText(text, textRect, font);

		if (value > 0)	textRect.right = realPos.x + (Int) ((frame.right - frame.left) / ((Float) (endValue - startValue) / (Float) (value - startValue))) + 1;
		else		textRect.right = 0;

		Font	 nFont = font;

		nFont.SetColor(Setup::GradientTextColor);

		surface->SetText(text, textRect, nFont);
	}

	prevValue = value;

	surface->EndPaint();

	return Success;
}

S::Int S::GUI::Progressbar::SetValue(Int newValue)
{
	value = (Int) Math::Min(endValue, Math::Max(startValue, newValue));

	Paint(SP_UPDATE);

	return Success;
}

S::Int S::GUI::Progressbar::GetValue()
{
	return value;
}
