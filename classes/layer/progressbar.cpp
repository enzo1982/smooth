 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_PROGRESSBAR_
#define __OBJSMOOTH_PROGRESSBAR_

#include <smooth/progressbar.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/mathtools.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_PROGRESSBAR = SMOOTH::RequestObjectID();

SMOOTHProgressbar::SMOOTHProgressbar(SMOOTHPoint pos, SMOOTHSize size, SMOOTHInt subType, SMOOTHInt iTextFlag, SMOOTHInt rangeStart, SMOOTHInt rangeEnd, SMOOTHInt iValue)
{
	type				= OBJ_PROGRESSBAR;
	subtype				= subType;
	startValue			= rangeStart;
	endValue			= rangeEnd;
	textFlag			= iTextFlag;
	value				= min(endValue, max(startValue, iValue));
	objectProperties->fontColor	= SMOOTH::Setup::ClientTextColor;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (subtype == OR_HORZ)
	{
		if (size.cx == 0)	objectProperties->size.cx = roundtoint(80 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = roundtoint(19 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
	}
	else
	{
		if (size.cx == 0)	objectProperties->size.cx = roundtoint(19 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = roundtoint(80 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
	}
}

SMOOTHProgressbar::~SMOOTHProgressbar()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHProgressbar::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHRect	 frame;
	SMOOTHPoint	 lineStart;
	SMOOTHPoint	 lineEnd;
	SMOOTHRect	 textRect;
	SMOOTHInt	 textSize = 0;
	SMOOTHInt	 color = 0;
	SMOOTHInt	 rs = GetRed(SMOOTH::Setup::GradientStartColor);
	SMOOTHInt	 gs = GetGreen(SMOOTH::Setup::GradientStartColor);
	SMOOTHInt	 bs = GetBlue(SMOOTH::Setup::GradientStartColor);
	SMOOTHInt	 re = GetRed(SMOOTH::Setup::GradientEndColor);
	SMOOTHInt	 ge = GetGreen(SMOOTH::Setup::GradientEndColor);
	SMOOTHInt	 be = GetBlue(SMOOTH::Setup::GradientEndColor);

	value = min(endValue, max(startValue, value));

	frame.left	= realPos.x;
	frame.right	= realPos.x + objectProperties->size.cx;
	frame.top	= realPos.y;
	frame.bottom	= realPos.y + objectProperties->size.cy;

	if (active)	Box(dc, frame, SMOOTH::Setup::ClientColor, FILLED);
	else		Box(dc, frame, SMOOTH::Setup::BackgroundColor, FILLED);

	Frame(dc, frame, FRAME_DOWN);

	frame.left	= realPos.x + 1;
	frame.top	= realPos.y + 1;

	if (subtype == OR_HORZ)
	{
		if (value > 0)
		{
			for (SMOOTHInt i = realPos.x + 1; i < realPos.x + (SMOOTHInt) ((frame.right - frame.left) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue))) + 1; i++)
			{
				lineStart.x = i;
				lineStart.y = realPos.y + 1;
				lineEnd.x = i;
				lineEnd.y = realPos.y + objectProperties->size.cy;

				if ((i - realPos.x - 1) > 0)	color = RGB((SMOOTHInt) (rs + (re - rs) / ((SMOOTHFloat) objectProperties->size.cx / (SMOOTHFloat) (i - realPos.x - 1))), (SMOOTHInt) (gs + (ge - gs) / ((SMOOTHFloat) objectProperties->size.cx / (SMOOTHFloat) (i - realPos.x - 1))), (SMOOTHInt) (bs + (be - bs) / ((SMOOTHFloat) objectProperties->size.cx / (SMOOTHFloat) (i - realPos.x - 1))));
				else				color = SMOOTH::Setup::GradientStartColor;

				Line(dc, lineStart, lineEnd, color, PS_SOLID, 1);
			}
		}
	}
	else
	{
		if (value > 0)
		{
			for (SMOOTHInt i = realPos.y + objectProperties->size.cy - 1; i > realPos.y + (SMOOTHInt) ((frame.bottom - frame.top) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue))) - 1; i--)
			{
				lineStart.x = realPos.x + 1;
				lineStart.y = i;
				lineEnd.x = realPos.x + objectProperties->size.cx;
				lineEnd.y = i;

				if ((i - realPos.y + 1) > 0)	color = RGB((SMOOTHInt) (re + (rs - re) / ((SMOOTHFloat) objectProperties->size.cy / (SMOOTHFloat) (i - realPos.y + 1))), (SMOOTHInt) (ge + (gs - ge) / ((SMOOTHFloat) objectProperties->size.cy / (SMOOTHFloat) (i - realPos.y + 1))), (SMOOTHInt) (be + (bs - be) / ((SMOOTHFloat) objectProperties->size.cy / (SMOOTHFloat) (i - realPos.y + 1 ))));
				else				color = SMOOTH::Setup::GradientStartColor;

				Line(dc, lineStart, lineEnd, color, PS_SOLID, 1);
			}
		}
	}

	if (subtype == OR_HORZ)
	{
		switch (textFlag)
		{
			case PB_VALUE:
				objectProperties->text = SMOOTHString::IntToString(max(value, startValue));
				break;
			case PB_PERCENT:
				if (value > 0)	objectProperties->text = SMOOTHString::IntToString(max(0, roundtoint(100 / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue)))));
				else		objectProperties->text = "0";

				objectProperties->text.Append("%");
				break;
			case PB_NOTEXT:
				objectProperties->text = NIL;
				break;
		}

		textSize = GetTextSizeX(dc, objectProperties->text, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

		textRect.left	= (frame.right + frame.left) / 2 - textSize / 2;
		textRect.top	= realPos.y + METRIC_PBTEXTOFFSETY;
		textRect.bottom	= realPos.y + objectProperties->size.cy;
		textRect.right	= textRect.left + textSize;

		::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

		if (value > 0)	textRect.right = realPos.x + (SMOOTHInt) ((frame.right - frame.left) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue))) + 1;
		else		textRect.right = 0;

		::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::GradientTextColor, objectProperties->fontWeight);
	}

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHProgressbar::SetValue(SMOOTHInt newValue)
{
	newValue = min(endValue, max(startValue, newValue));

	if (!registered || !visible)
	{
		value = newValue;

		return SMOOTH::Success;
	}

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHRect	 frame;
	SMOOTHPoint	 lineStart;
	SMOOTHPoint	 lineEnd;
	SMOOTHRect	 textRect;
	SMOOTHInt	 textSize = 0;
	SMOOTHInt	 color = 0;
	SMOOTHInt	 rs = GetRed(SMOOTH::Setup::GradientStartColor);
	SMOOTHInt	 gs = GetGreen(SMOOTH::Setup::GradientStartColor);
	SMOOTHInt	 bs = GetBlue(SMOOTH::Setup::GradientStartColor);
	SMOOTHInt	 re = GetRed(SMOOTH::Setup::GradientEndColor);
	SMOOTHInt	 ge = GetGreen(SMOOTH::Setup::GradientEndColor);
	SMOOTHInt	 be = GetBlue(SMOOTH::Setup::GradientEndColor);

	frame.left	= realPos.x + 1;
	frame.right	= realPos.x + objectProperties->size.cx;
	frame.top	= realPos.y + 1;
	frame.bottom	= realPos.y + objectProperties->size.cy;

	if (subtype == OR_HORZ)
	{
		switch (textFlag)
		{
			case PB_VALUE:
				objectProperties->text = SMOOTHString::IntToString(max(value, startValue));
				break;
			case PB_PERCENT:
				if (value > 0)	objectProperties->text = SMOOTHString::IntToString(max(0, roundtoint(100 / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue)))));
				else		objectProperties->text = "0";

				objectProperties->text.Append("%");
				break;
			case PB_NOTEXT:
				objectProperties->text = NIL;
				break;
		}

		textSize = GetTextSizeX(dc, objectProperties->text, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

		textRect.left	= (frame.right + frame.left) / 2 - textSize / 2;
		textRect.top	= realPos.y + METRIC_PBTEXTOFFSETY;
		textRect.bottom	= realPos.y + objectProperties->size.cy;
		textRect.right	= textRect.left + textSize;

		if (active)	::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::ClientColor, objectProperties->fontWeight);
		else		::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::BackgroundColor, objectProperties->fontWeight);

		if (textRect.left <= realPos.x + (SMOOTHInt) ((frame.right - frame.left) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue))) + 1)
		{
			for (SMOOTHInt i = textRect.left; i < min(textRect.right, realPos.x + (SMOOTHInt) ((frame.right - frame.left) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue))) + 1); i++)
			{
				lineStart.x = i;
				lineStart.y = realPos.y + 1;
				lineEnd.x = i;
				lineEnd.y = realPos.y + objectProperties->size.cy;

				if ((i - realPos.x - 1) > 0)	color = RGB((SMOOTHInt) (rs + (re - rs) / ((SMOOTHFloat) objectProperties->size.cx / (SMOOTHFloat) (i - realPos.x - 1))), (SMOOTHInt) (gs + (ge - gs) / ((SMOOTHFloat) objectProperties->size.cx / (SMOOTHFloat) (i - realPos.x - 1))), (SMOOTHInt) (bs + (be - bs) / ((SMOOTHFloat) objectProperties->size.cx / (SMOOTHFloat) (i - realPos.x - 1))));
				else				color = SMOOTH::Setup::GradientStartColor;

				Line(dc, lineStart, lineEnd, color, PS_SOLID, 1);
			}
		}
	}

	if (subtype == OR_HORZ)
	{	
		if (newValue > 0 && newValue > value)
		{
			for (SMOOTHInt i = realPos.x + (SMOOTHInt) ((frame.right - frame.left) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue))) + 1; i < realPos.x + (SMOOTHInt) ((frame.right - frame.left) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (newValue - startValue))) + 1; i++)
			{
				lineStart.x = i;
				lineStart.y = realPos.y + 1;
				lineEnd.x = i;
				lineEnd.y = realPos.y + objectProperties->size.cy;

				if ((i - realPos.x - 1) > 0)	color = RGB((SMOOTHInt) (rs + (re - rs) / ((SMOOTHFloat) objectProperties->size.cx / (SMOOTHFloat) (i - realPos.x - 1))), (SMOOTHInt) (gs + (ge - gs) / ((SMOOTHFloat) objectProperties->size.cx / (SMOOTHFloat) (i - realPos.x - 1))), (SMOOTHInt) (bs + (be - bs) / ((SMOOTHFloat) objectProperties->size.cx / (SMOOTHFloat) (i - realPos.x - 1))));
				else				color = SMOOTH::Setup::GradientStartColor;

				Line(dc, lineStart, lineEnd, color, PS_SOLID, 1);
			}
		}
		else if (newValue > 0 && newValue < value)
		{
			for (SMOOTHInt i = realPos.x + (SMOOTHInt) ((frame.right - frame.left) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (newValue - startValue))) + 1; i < realPos.x + (SMOOTHInt) ((frame.right - frame.left) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue))) + 1; i++)
			{
				lineStart.x = i;
				lineStart.y = realPos.y + 1;
				lineEnd.x = i;
				lineEnd.y = realPos.y + objectProperties->size.cy;

				if (active)	Line(dc, lineStart, lineEnd, SMOOTH::Setup::ClientColor, PS_SOLID, 1);
				else		Line(dc, lineStart, lineEnd, SMOOTH::Setup::BackgroundColor, PS_SOLID, 1);
			}
		}
		else if (newValue == 0 && value > 0)
		{
			if (active)	Box(dc, frame, SMOOTH::Setup::ClientColor, FILLED);
			else		Box(dc, frame, SMOOTH::Setup::BackgroundColor, FILLED);
		}
	}
	else
	{
		if (newValue > 0 && newValue > value)
		{
			for (SMOOTHInt i = realPos.y + objectProperties->size.cy - (SMOOTHInt) ((frame.bottom - frame.top) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue))) - 1; i > realPos.y + objectProperties->size.cy - (SMOOTHInt) ((frame.bottom - frame.top) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (newValue - startValue))) - 1; i--)
			{
				lineStart.x = realPos.x + 1;
				lineStart.y = i;
				lineEnd.x = realPos.x + objectProperties->size.cx;
				lineEnd.y = i;

				if ((i - realPos.y + 1) > 0)	color = RGB((SMOOTHInt) (re + (rs - re) / ((SMOOTHFloat) objectProperties->size.cy / (SMOOTHFloat) (i - realPos.y + 1))), (SMOOTHInt) (ge + (gs - ge) / ((SMOOTHFloat) objectProperties->size.cy / (SMOOTHFloat) (i - realPos.y + 1))), (SMOOTHInt) (be + (bs - be) / ((SMOOTHFloat) objectProperties->size.cy / (SMOOTHFloat) (i - realPos.y + 1 ))));
				else				color = SMOOTH::Setup::GradientStartColor;

				Line(dc, lineStart, lineEnd, color, PS_SOLID, 1);
			}
		}
		else if (newValue > 0 && newValue < value)
		{
			for (SMOOTHInt i = realPos.y + objectProperties->size.cy - (SMOOTHInt) ((frame.bottom - frame.top) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (newValue - startValue))) - 1; i > realPos.y + objectProperties->size.cy - (SMOOTHInt) ((frame.bottom - frame.top) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue))) - 1; i--)
			{
				lineStart.x = realPos.x + 1;
				lineStart.y = i;
				lineEnd.x = realPos.x + objectProperties->size.cx;
				lineEnd.y = i;

				if (active)	Line(dc, lineStart, lineEnd, SMOOTH::Setup::ClientColor, PS_SOLID, 1);
				else		Line(dc, lineStart, lineEnd, SMOOTH::Setup::BackgroundColor, PS_SOLID, 1);
			}
		}
		else if (newValue == 0 && value > 0)
		{
			if (active)	Box(dc, frame, SMOOTH::Setup::ClientColor, FILLED);
			else		Box(dc, frame, SMOOTH::Setup::BackgroundColor, FILLED);
		}
	}

	value = newValue;

	if (subtype == OR_HORZ)
	{
		switch (textFlag)
		{
			case PB_VALUE:
				objectProperties->text = SMOOTHString::IntToString(max(value, startValue));
				break;
			case PB_PERCENT:
				if (value > 0)	objectProperties->text = SMOOTHString::IntToString(max(0, roundtoint(100 / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue)))));
				else		objectProperties->text = "0";

				objectProperties->text.Append("%");
				break;
			case PB_NOTEXT:
				objectProperties->text = NIL;
				break;
		}

		textSize = GetTextSizeX(dc, objectProperties->text, objectProperties->font, objectProperties->fontSize, objectProperties->fontWeight);

		textRect.left	= (frame.right + frame.left) / 2 - textSize / 2;
		textRect.top	= realPos.y + METRIC_PBTEXTOFFSETY;
		textRect.bottom	= realPos.y + objectProperties->size.cy;
		textRect.right	= textRect.left + textSize;

		::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

		if (value > 0)	textRect.right = realPos.x + (SMOOTHInt) ((frame.right - frame.left) / ((SMOOTHFloat) (endValue - startValue) / (SMOOTHFloat) (value - startValue))) + 1;
		else		textRect.right = 0;

		::SetText(dc, objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::GradientTextColor, objectProperties->fontWeight);
	}

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHProgressbar::GetValue()
{
	return value;
}

#endif
