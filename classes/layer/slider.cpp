 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_SLIDER_
#define __OBJSMOOTH_SLIDER_

#include <smooth/slider.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/mathtools.h>
#include <smooth/metrics.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_SLIDER = SMOOTH::RequestObjectID();

SMOOTHSlider::SMOOTHSlider(SMOOTHPoint pos, SMOOTHSize size, SMOOTHInt subType, SMOOTHInt *var, SMOOTHInt rangeStart, SMOOTHInt rangeEnd, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_SLIDER;
	subtype				= subType;
	variable			= var;
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;
	objectProperties->clicked	= SMOOTH::False;
	startValue			= rangeStart;
	endValue			= rangeEnd;
	prevValue			= 0;

	possibleContainers.AddEntry(OBJ_LAYER);

	if (*variable < startValue)	*variable = startValue;
	else if (*variable > endValue)	*variable = endValue;

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0) size.cx = size.cy;
	if (size.cy == 0) size.cy = size.cx;

	if (size.cx == 0)	objectProperties->size.cx = roundtoint(100 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = roundtoint(100 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);

	if (subType == OR_HORZ)	objectProperties->size.cy = METRIC_SLIDERAREAHEIGHT;
	else			objectProperties->size.cx = METRIC_SLIDERAREAHEIGHT;
}

SMOOTHSlider::~SMOOTHSlider()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHSlider::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHPoint	 lineStart;
	SMOOTHPoint	 lineEnd;
	SMOOTHRect	 sliderRect;

	if (prevValue != *variable)
	{
		if (subtype == OR_HORZ)
		{
			sliderRect.left		= realPos.x + (SMOOTHInt) (((SMOOTHFloat) (objectProperties->size.cx - METRIC_SLIDERAREAWIDTH)) / ((SMOOTHFloat) (endValue - startValue)) * ((SMOOTHFloat) (prevValue - startValue)));
			sliderRect.top		= realPos.y;
			sliderRect.right	= sliderRect.left + METRIC_SLIDERAREAWIDTH;
			sliderRect.bottom	= sliderRect.top + METRIC_SLIDERAREAHEIGHT;
		}
		else
		{
			sliderRect.left		= realPos.x;
			sliderRect.top		= realPos.y + (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH) - (SMOOTHInt) (((SMOOTHFloat) (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH)) / ((SMOOTHFloat) (endValue - startValue)) * ((SMOOTHFloat) (prevValue - startValue)));
			sliderRect.right	= sliderRect.left + METRIC_SLIDERAREAHEIGHT;
			sliderRect.bottom	= sliderRect.top + METRIC_SLIDERAREAWIDTH + 1;
		}

		Box(dc, sliderRect, SMOOTH::Setup::BackgroundColor, FILLED);
	}

	if (subtype == OR_HORZ)
	{
		lineStart.x = realPos.x + METRIC_SLIDERAREAOFFSETX;
		lineStart.y = realPos.y + METRIC_SLIDERAREAOFFSETY;
		lineEnd.x = realPos.x + objectProperties->size.cx - METRIC_SLIDERAREAOFFSETX;
		lineEnd.y = lineStart.y;

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

		lineStart.y++;
		lineEnd.y++;

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerLightColor, PS_SOLID, 1);

		PaintPixel(dc, SMOOTHPoint(lineEnd.x - 1, lineEnd.y - 1), SMOOTH::Setup::DividerLightColor);

		sliderRect.left		= realPos.x + (SMOOTHInt) (((SMOOTHFloat) (objectProperties->size.cx - METRIC_SLIDERAREAWIDTH)) / ((SMOOTHFloat) (endValue - startValue)) * ((SMOOTHFloat) (*variable - startValue)));
		sliderRect.top		= realPos.y;
		sliderRect.right	= sliderRect.left + METRIC_SLIDERAREAWIDTH - 1;
		sliderRect.bottom	= sliderRect.top + METRIC_SLIDERAREAHEIGHT - 2;

		if (!objectProperties->clicked)	Box(dc, sliderRect, SMOOTH::Setup::BackgroundColor, FILLED);
		else				Box(dc, sliderRect, SMOOTH::Setup::LightGrayColor, FILLED);

		Frame(dc, sliderRect, FRAME_UP);
	}
	else
	{
		lineStart.x = realPos.x + METRIC_SLIDERAREAOFFSETY;
		lineStart.y = realPos.y + METRIC_SLIDERAREAOFFSETX;
		lineEnd.x = lineStart.x;
		lineEnd.y = realPos.y + objectProperties->size.cy - METRIC_SLIDERAREAOFFSETX;

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

		lineStart.x++;
		lineEnd.x++;

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerLightColor, PS_SOLID, 1);

		PaintPixel(dc, SMOOTHPoint(lineEnd.x - 1, lineEnd.y - 1), SMOOTH::Setup::DividerLightColor);

		sliderRect.left		= realPos.x;
		sliderRect.top		= realPos.y + (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH) - (SMOOTHInt) (((SMOOTHFloat) (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH)) / ((SMOOTHFloat) (endValue - startValue)) * ((SMOOTHFloat) (*variable - startValue)));
		sliderRect.right	= sliderRect.left + METRIC_SLIDERAREAHEIGHT - 1;
		sliderRect.bottom	= sliderRect.top + METRIC_SLIDERAREAWIDTH;

		if (!objectProperties->clicked)	Box(dc, sliderRect, SMOOTH::Setup::BackgroundColor, FILLED);
		else				Box(dc, sliderRect, SMOOTH::Setup::LightGrayColor, FILLED);

		Frame(dc, sliderRect, FRAME_UP);
	}

	prevValue = *variable;

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSlider::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHInt	 retVal = SMOOTH::Success;
	SMOOTHRect	 slider;
	SMOOTHRect	 actionArea;
	SMOOTHInt	 oldValue = *variable;
	SMOOTHFloat	 buffer;
	HDC		 dc;

	if (prevValue != *variable) Paint(SP_PAINT);

	if (subtype == OR_HORZ)
	{
		slider.left	= realPos.x + (SMOOTHInt) (((SMOOTHFloat) (objectProperties->size.cx - METRIC_SLIDERAREAWIDTH)) / ((SMOOTHFloat) (endValue - startValue)) * ((SMOOTHFloat) (*variable - startValue)));
		slider.top	= realPos.y;
		slider.right	= slider.left + METRIC_SLIDERAREAWIDTH - 1;
		slider.bottom	= slider.top + METRIC_SLIDERAREAHEIGHT - 2;

		actionArea.left		= realPos.x + METRIC_SLIDERAREAOFFSETX;
		actionArea.top		= realPos.y;
		actionArea.right	= actionArea.left + objectProperties->size.cx - METRIC_SLIDERAREAWIDTH;
		actionArea.bottom	= actionArea.top + METRIC_SLIDERAREAHEIGHT - 2;
	}
	else
	{
		slider.left	= realPos.x;
		slider.top	= realPos.y + (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH) - (SMOOTHInt) (((SMOOTHFloat) (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH)) / ((SMOOTHFloat) (endValue - startValue)) * ((SMOOTHFloat) (*variable - startValue)));
		slider.right	= slider.left + METRIC_SLIDERAREAHEIGHT - 1;
		slider.bottom	= slider.top + METRIC_SLIDERAREAWIDTH;

		actionArea.left		= realPos.x;
		actionArea.top		= realPos.y + METRIC_SLIDERAREAOFFSETX;
		actionArea.right	= actionArea.left + METRIC_SLIDERAREAHEIGHT - 1;
		actionArea.bottom	= actionArea.top + objectProperties->size.cy - METRIC_SLIDERAREAWIDTH;
	}

	switch (message)
	{
		case SM_LBUTTONDOWN:
			dc = GetContext(wnd);

			if (IsMouseOn(wnd->hwnd, slider, WINDOW) && !objectProperties->clicked)
			{
				objectProperties->clicked = SMOOTH::True;

				slider.left++;
				slider.top++;
				Box(dc, slider, SMOOTH::Setup::LightGrayColor, FILLED);
				slider.left--;
				slider.top--;

				if (subtype == OR_HORZ)	mouseBias = (slider.left + METRIC_SLIDERAREAOFFSETX) - MouseX(wnd->hwnd, WINDOW);
				else			mouseBias = (slider.top + METRIC_SLIDERAREAOFFSETX) - MouseY(wnd->hwnd, WINDOW);

				retVal = SMOOTH::Break;
			}
			else if (IsMouseOn(wnd->hwnd, actionArea, WINDOW) && !objectProperties->clicked)
			{
				mouseBias = 0;

				objectProperties->clicked = SMOOTH::True;
				Process(SM_MOUSEMOVE, 0, 0);
				objectProperties->clicked = SMOOTH::False;

				Paint(SP_PAINT);

				retVal = SMOOTH::Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONUP:
			dc = GetContext(wnd);

			if (objectProperties->clicked)
			{
				objectProperties->clicked = SMOOTH::False;

				slider.left++;
				slider.top++;
				Box(dc, slider, SMOOTH::Setup::BackgroundColor, FILLED);
				slider.left--;
				slider.top--;

				retVal = SMOOTH::Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			if (objectProperties->clicked)
			{
				if (subtype == OR_HORZ)
				{
					buffer = ((SMOOTHFloat) (endValue - startValue)) / (((SMOOTHFloat) (objectProperties->size.cx - METRIC_SLIDERAREAWIDTH)) / ((SMOOTHFloat) (MouseX(wnd->hwnd, WINDOW) + mouseBias - (realPos.x + METRIC_SLIDERAREAOFFSETX))));

					*variable = startValue + roundtoint(buffer);

					if (*variable < startValue)	*variable = startValue;
					if (*variable > endValue)	*variable = endValue;
				}
				else
				{
					buffer = ((SMOOTHFloat) (endValue - startValue)) / (((SMOOTHFloat) (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH)) / ((SMOOTHFloat) (MouseY(wnd->hwnd, WINDOW) + mouseBias - (realPos.y + METRIC_SLIDERAREAOFFSETX))));

					*variable = endValue - roundtoint(buffer);

					if (*variable < startValue)	*variable = startValue;
					if (*variable > endValue)	*variable = endValue;
				}

				if (*variable != oldValue)
				{
					slider.right++;
					slider.bottom++;
					Box(dc, slider, SMOOTH::Setup::BackgroundColor, FILLED);
					slider.right--;
					slider.bottom--;

					Paint(SP_PAINT);

					SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);
				}
			}

			FreeContext(wnd, dc);

			break;
	}

	return retVal;
}

SMOOTHInt SMOOTHSlider::SetRange(SMOOTHInt rangeStart, SMOOTHInt rangeEnd)
{
	SMOOTHInt	 prevStartValue	= startValue;
	SMOOTHInt	 prevEndValue	= endValue;

	startValue	= rangeStart;
	endValue	= rangeEnd;

	*variable	= ((*variable) - prevStartValue) * ((endValue - startValue) / (prevEndValue - prevStartValue)) + startValue;

	SetValue(*variable);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSlider::SetValue(SMOOTHInt newValue)
{
	if (newValue < startValue)	newValue = startValue;
	if (newValue > endValue)	newValue = endValue;

	if (*variable == newValue) return SMOOTH::Success;

	SMOOTHBool	 prevVisible = visible;

	if (registered && visible) Hide();

	*variable = newValue;

	if (registered && prevVisible) Show();

	SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHSlider::GetValue()
{
	return *variable;
}

#endif
