 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/progressbar.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/math.h>
#include <smooth/metrics.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/color.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Progressbar::classID = S::Object::RequestClassID();

S::GUI::Progressbar::Progressbar(Point pos, Size size, Int subType, Int iTextFlag, Int rangeStart, Int rangeEnd, Int iValue)
{
	type				= classID;
	subtype				= subType;
	startValue			= rangeStart;
	endValue			= rangeEnd;
	textFlag			= iTextFlag;
	value				= (Int) Math::Min(endValue, Math::Max(startValue, iValue));

	objectProperties->font.SetColor(Setup::ClientTextColor);

	possibleContainers.AddEntry(Layer::classID);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (subtype == OR_HORZ)
	{
		if (size.cx == 0)	objectProperties->size.cx = Math::Round(80 * Setup::FontSize);
		else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = Math::Round(19 * Setup::FontSize);
		else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
	}
	else
	{
		if (size.cx == 0)	objectProperties->size.cx = Math::Round(19 * Setup::FontSize);
		else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = Math::Round(80 * Setup::FontSize);
		else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
	}
}

S::GUI::Progressbar::~Progressbar()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Progressbar::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Point	 lineStart;
	Point	 lineEnd;
	Rect	 textRect;
	Int	 textSize = 0;
	Int	 color = 0;
	Int	 rs = GetRed(Setup::GradientStartColor);
	Int	 gs = GetGreen(Setup::GradientStartColor);
	Int	 bs = GetBlue(Setup::GradientStartColor);
	Int	 re = GetRed(Setup::GradientEndColor);
	Int	 ge = GetGreen(Setup::GradientEndColor);
	Int	 be = GetBlue(Setup::GradientEndColor);

	value = (Int) Math::Min(endValue, Math::Max(startValue, value));

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
	else		surface->Box(frame, Setup::BackgroundColor, FILLED);

	surface->Frame(frame, FRAME_DOWN);

	frame.left	= realPos.x + 1;
	frame.top	= realPos.y + 1;

	if (subtype == OR_HORZ)
	{
		if (value > 0)
		{
			for (Int i = realPos.x + 1; i < realPos.x + (Int) ((frame.right - frame.left) / ((Float) (endValue - startValue) / (Float) (value - startValue))) + 1; i++)
			{
				lineStart.x = i;
				lineStart.y = realPos.y + 1;
				lineEnd.x = i;
				lineEnd.y = realPos.y + objectProperties->size.cy - 1;

				if ((i - realPos.x - 1) > 0)	color = RGB((Int) (rs + (re - rs) / ((Float) (objectProperties->size.cx - 1) / (Float) (i - realPos.x - 1))), (Int) (gs + (ge - gs) / ((Float) (objectProperties->size.cx - 1) / (Float) (i - realPos.x - 1))), (Int) (bs + (be - bs) / ((Float) (objectProperties->size.cx - 1) / (Float) (i - realPos.x - 1))));
				else				color = Setup::GradientStartColor;

				surface->Line(lineStart, lineEnd, color);
			}
		}
	}
	else
	{
		if (value > 0)
		{
			for (Int i = realPos.y + objectProperties->size.cy - 2; i > realPos.y + (Int) ((frame.bottom - frame.top) / ((Float) (endValue - startValue) / (Float) (value - startValue))) - 1; i--)
			{
				lineStart.x = realPos.x + 1;
				lineStart.y = i;
				lineEnd.x = realPos.x + objectProperties->size.cx - 1;
				lineEnd.y = i;

				if ((i - realPos.y + 1) > 0)	color = RGB((Int) (re + (rs - re) / ((Float) (objectProperties->size.cy - 1) / (Float) (i - realPos.y + 1))), (Int) (ge + (gs - ge) / ((Float) (objectProperties->size.cy - 1) / (Float) (i - realPos.y + 1))), (Int) (be + (bs - be) / ((Float) (objectProperties->size.cy - 1) / (Float) (i - realPos.y + 1 ))));
				else				color = Setup::GradientStartColor;

				surface->Line(lineStart, lineEnd, color);
			}
		}
	}

	if (subtype == OR_HORZ)
	{
		switch (textFlag)
		{
			case PB_VALUE:
				objectProperties->text = String::FromInt(Math::Max(value, startValue));
				break;
			case PB_PERCENT:
				if (value > 0)	objectProperties->text = String::FromInt(Math::Max(0, Math::Round(100 / ((Float) (endValue - startValue) / (Float) (value - startValue)))));
				else		objectProperties->text = "0";

				objectProperties->text.Append("%");
				break;
			case PB_NOTEXT:
				objectProperties->text = NIL;
				break;
		}

		textSize = objectProperties->font.GetTextSizeX(objectProperties->text);

		textRect.left	= (frame.right + frame.left) / 2 - textSize / 2;
		textRect.top	= realPos.y + METRIC_PBTEXTOFFSETY;
		textRect.bottom	= realPos.y + objectProperties->size.cy - 1;
		textRect.right	= textRect.left + textSize;

		surface->SetText(objectProperties->text, textRect, objectProperties->font);

		if (value > 0)	textRect.right = realPos.x + (Int) ((frame.right - frame.left) / ((Float) (endValue - startValue) / (Float) (value - startValue))) + 1;
		else		textRect.right = 0;

		Font	 font = objectProperties->font;

		font.SetColor(Setup::GradientTextColor);

		surface->SetText(objectProperties->text, textRect, font);
	}

	return Success;
}

S::Int S::GUI::Progressbar::SetValue(Int newValue)
{
	newValue = (Int) Math::Min(endValue, Math::Max(startValue, newValue));

	if (!registered || !visible)
	{
		value = newValue;

		return Success;
	}

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Point	 lineStart;
	Point	 lineEnd;
	Rect	 textRect;
	Int	 textSize = 0;
	Int	 color = 0;
	Int	 rs = GetRed(Setup::GradientStartColor);
	Int	 gs = GetGreen(Setup::GradientStartColor);
	Int	 bs = GetBlue(Setup::GradientStartColor);
	Int	 re = GetRed(Setup::GradientEndColor);
	Int	 ge = GetGreen(Setup::GradientEndColor);
	Int	 be = GetBlue(Setup::GradientEndColor);

	frame.left	= realPos.x + 1;
	frame.top	= realPos.y + 1;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (subtype == OR_HORZ)
	{
		switch (textFlag)
		{
			case PB_VALUE:
				objectProperties->text = String::FromInt(Math::Max(value, startValue));
				break;
			case PB_PERCENT:
				if (value > 0)	objectProperties->text = String::FromInt(Math::Max(0, Math::Round(100 / ((Float) (endValue - startValue) / (Float) (value - startValue)))));
				else		objectProperties->text = "0";

				objectProperties->text.Append("%");
				break;
			case PB_NOTEXT:
				objectProperties->text = NIL;
				break;
		}

		textSize = objectProperties->font.GetTextSizeX(objectProperties->text);

		textRect.left	= (frame.right + frame.left) / 2 - textSize / 2;
		textRect.top	= realPos.y + METRIC_PBTEXTOFFSETY;
		textRect.bottom	= realPos.y + objectProperties->size.cy - 1;
		textRect.right	= textRect.left + textSize;

		Font	 font = objectProperties->font;

		if (active)	font.SetColor(Setup::ClientColor);
		else		font.SetColor(Setup::BackgroundColor);

		surface->SetText(objectProperties->text, textRect, font);

		if (textRect.left <= realPos.x + (Int) ((frame.right - frame.left) / ((Float) (endValue - startValue) / (Float) (value - startValue))) + 1)
		{
			for (Int i = textRect.left; i < Math::Min(textRect.right, realPos.x + (Int) ((frame.right - frame.left) / ((Float) (endValue - startValue) / (Float) (value - startValue))) + 1); i++)
			{
				lineStart.x = i;
				lineStart.y = realPos.y + 1;
				lineEnd.x = i;
				lineEnd.y = realPos.y + objectProperties->size.cy - 1;

				if ((i - realPos.x - 1) > 0)	color = RGB((Int) (rs + (re - rs) / ((Float) (objectProperties->size.cx - 1) / (Float) (i - realPos.x - 1))), (Int) (gs + (ge - gs) / ((Float) (objectProperties->size.cx - 1) / (Float) (i - realPos.x - 1))), (Int) (bs + (be - bs) / ((Float) (objectProperties->size.cx - 1) / (Float) (i - realPos.x - 1))));
				else				color = Setup::GradientStartColor;

				surface->Line(lineStart, lineEnd, color);
			}
		}
	}

	if (subtype == OR_HORZ)
	{	
		if (newValue > 0 && newValue > value)
		{
			for (Int i = (Int) Math::Max(realPos.x + 1, realPos.x + (Int) ((frame.right - frame.left) / ((Float) (endValue - startValue) / (Float) (value - startValue)))); i < realPos.x + (Int) ((frame.right - frame.left) / ((Float) (endValue - startValue) / (Float) (newValue - startValue))) + 1; i++)
			{
				lineStart.x = i;
				lineStart.y = realPos.y + 1;
				lineEnd.x = i;
				lineEnd.y = realPos.y + objectProperties->size.cy - 1;

				if ((i - realPos.x - 1) > 0)	color = RGB((Int) (rs + (re - rs) / ((Float) (objectProperties->size.cx - 1) / (Float) (i - realPos.x - 1))), (Int) (gs + (ge - gs) / ((Float) (objectProperties->size.cx - 1) / (Float) (i - realPos.x - 1))), (Int) (bs + (be - bs) / ((Float) (objectProperties->size.cx - 1) / (Float) (i - realPos.x - 1))));
				else				color = Setup::GradientStartColor;

				surface->Line(lineStart, lineEnd, color);
			}
		}
		else if (newValue > 0 && newValue < value)
		{
			for (Int i = realPos.x + (Int) ((frame.right - frame.left) / ((Float) (endValue - startValue) / (Float) (newValue - startValue))) + 1; i < realPos.x + (Int) ((frame.right - frame.left) / ((Float) (endValue - startValue) / (Float) (value - startValue))) + 1; i++)
			{
				lineStart.x = i;
				lineStart.y = realPos.y + 1;
				lineEnd.x = i;
				lineEnd.y = realPos.y + objectProperties->size.cy - 1;

				if (active)	surface->Line(lineStart, lineEnd, Setup::ClientColor);
				else		surface->Line(lineStart, lineEnd, Setup::BackgroundColor);
			}
		}
		else if (newValue == 0 && value > 0)
		{
			if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
			else		surface->Box(frame, Setup::BackgroundColor, FILLED);
		}
	}
	else
	{
		if (newValue > 0 && newValue > value)
		{
			for (Int i = realPos.y + objectProperties->size.cy - (Int) ((frame.bottom - frame.top) / ((Float) (endValue - startValue) / (Float) (value - startValue))) - 2; i > realPos.y + objectProperties->size.cy - (Int) ((frame.bottom - frame.top) / ((Float) (endValue - startValue) / (Float) (newValue - startValue))) - 2; i--)
			{
				lineStart.x = realPos.x + 1;
				lineStart.y = i;
				lineEnd.x = realPos.x + objectProperties->size.cx - 1;
				lineEnd.y = i;

				if ((i - realPos.y + 1) > 0)	color = RGB((Int) (re + (rs - re) / ((Float) (objectProperties->size.cy - 1) / (Float) (i - realPos.y + 1))), (Int) (ge + (gs - ge) / ((Float) (objectProperties->size.cy - 1) / (Float) (i - realPos.y + 1))), (Int) (be + (bs - be) / ((Float) (objectProperties->size.cy - 1) / (Float) (i - realPos.y + 1 ))));
				else				color = Setup::GradientStartColor;

				surface->Line(lineStart, lineEnd, color);
			}
		}
		else if (newValue > 0 && newValue < value)
		{
			for (Int i = realPos.y + objectProperties->size.cy - (Int) ((frame.bottom - frame.top) / ((Float) (endValue - startValue) / (Float) (newValue - startValue))) - 2; i > realPos.y + objectProperties->size.cy - (Int) ((frame.bottom - frame.top) / ((Float) (endValue - startValue) / (Float) (value - startValue))) - 2; i--)
			{
				lineStart.x = realPos.x + 1;
				lineStart.y = i;
				lineEnd.x = realPos.x + objectProperties->size.cx - 1;
				lineEnd.y = i;

				if (active)	surface->Line(lineStart, lineEnd, Setup::ClientColor);
				else		surface->Line(lineStart, lineEnd, Setup::BackgroundColor);
			}
		}
		else if (newValue == 0 && value > 0)
		{
			if (active)	surface->Box(frame, Setup::ClientColor, FILLED);
			else		surface->Box(frame, Setup::BackgroundColor, FILLED);
		}
	}

	value = newValue;

	if (subtype == OR_HORZ)
	{
		switch (textFlag)
		{
			case PB_VALUE:
				objectProperties->text = String::FromInt(Math::Max(value, startValue));
				break;
			case PB_PERCENT:
				if (value > 0)	objectProperties->text = String::FromInt(Math::Max(0, Math::Round(100 / ((Float) (endValue - startValue) / (Float) (value - startValue)))));
				else		objectProperties->text = "0";

				objectProperties->text.Append("%");
				break;
			case PB_NOTEXT:
				objectProperties->text = NIL;
				break;
		}

		textSize = objectProperties->font.GetTextSizeX(objectProperties->text);

		textRect.left	= (frame.right + frame.left) / 2 - textSize / 2;
		textRect.top	= realPos.y + METRIC_PBTEXTOFFSETY;
		textRect.bottom	= realPos.y + objectProperties->size.cy - 1;
		textRect.right	= textRect.left + textSize;

		surface->SetText(objectProperties->text, textRect, objectProperties->font);

		if (value > 0)	textRect.right = realPos.x + (Int) ((frame.right - frame.left) / ((Float) (endValue - startValue) / (Float) (value - startValue))) + 1;
		else		textRect.right = 0;

		Font	 font = objectProperties->font;

		font.SetColor(Setup::GradientTextColor);

		surface->SetText(objectProperties->text, textRect, font);
	}

	return Success;
}

S::Int S::GUI::Progressbar::GetValue()
{
	return value;
}
