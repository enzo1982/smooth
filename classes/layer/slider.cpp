 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/slider.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/math.h>
#include <smooth/metrics.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/surface.h>

const S::Int	 S::GUI::Slider::classID = S::Object::RequestClassID();

S::GUI::Slider::Slider(Point pos, Size size, Int subType, Int *var, Int rangeStart, Int rangeEnd)
{
	type				= classID;
	subtype				= subType;
	variable			= var;
	objectProperties->clicked	= False;
	startValue			= rangeStart;
	endValue			= rangeEnd;
	prevValue			= 0;

	possibleContainers.AddEntry(Layer::classID);

	if (*variable < startValue)	*variable = startValue;
	else if (*variable > endValue)	*variable = endValue;

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (size.cx == 0) size.cx = size.cy;
	if (size.cy == 0) size.cy = size.cx;

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(100 * Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(100 * Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);

	if (subType == OR_HORZ)	objectProperties->size.cy = METRIC_SLIDERAREAHEIGHT;
	else			objectProperties->size.cx = METRIC_SLIDERAREAHEIGHT;
}

S::GUI::Slider::~Slider()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Slider::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Point	 lineStart;
	Point	 lineEnd;
	Rect	 sliderRect;

	if (prevValue != *variable)
	{
		if (subtype == OR_HORZ)
		{
			sliderRect.left		= realPos.x + (Int) (((Float) (objectProperties->size.cx - METRIC_SLIDERAREAWIDTH)) / ((Float) (endValue - startValue)) * ((Float) (prevValue - startValue)));
			sliderRect.top		= realPos.y;
			sliderRect.right	= sliderRect.left + METRIC_SLIDERAREAWIDTH;
			sliderRect.bottom	= sliderRect.top + METRIC_SLIDERAREAHEIGHT;
		}
		else
		{
			sliderRect.left		= realPos.x;
			sliderRect.top		= realPos.y + (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH) - (Int) (((Float) (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH)) / ((Float) (endValue - startValue)) * ((Float) (prevValue - startValue)));
			sliderRect.right	= sliderRect.left + METRIC_SLIDERAREAHEIGHT;
			sliderRect.bottom	= sliderRect.top + METRIC_SLIDERAREAWIDTH + 1;
		}

		surface->Box(sliderRect, Setup::BackgroundColor, FILLED);
	}

	if (subtype == OR_HORZ)
	{
		lineStart.x = realPos.x + METRIC_SLIDERAREAOFFSETX;
		lineStart.y = realPos.y + METRIC_SLIDERAREAOFFSETY;
		lineEnd.x = realPos.x + objectProperties->size.cx - METRIC_SLIDERAREAOFFSETX;
		lineEnd.y = lineStart.y;

		surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);

		lineStart.y++;
		lineEnd.y++;

		surface->Line(lineStart, lineEnd, Setup::DividerLightColor);

		surface->SetPixel(lineEnd.x - 1, lineEnd.y - 1, Setup::DividerLightColor);

		sliderRect.left		= realPos.x + (Int) (((Float) (objectProperties->size.cx - METRIC_SLIDERAREAWIDTH)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
		sliderRect.top		= realPos.y;
		sliderRect.right	= sliderRect.left + METRIC_SLIDERAREAWIDTH - 1;
		sliderRect.bottom	= sliderRect.top + METRIC_SLIDERAREAHEIGHT - 2;

		if (!objectProperties->clicked)	surface->Box(sliderRect, Setup::BackgroundColor, FILLED);
		else				surface->Box(sliderRect, Setup::LightGrayColor, FILLED);

		surface->Frame(sliderRect, FRAME_UP);
	}
	else
	{
		lineStart.x = realPos.x + METRIC_SLIDERAREAOFFSETY;
		lineStart.y = realPos.y + METRIC_SLIDERAREAOFFSETX;
		lineEnd.x = lineStart.x;
		lineEnd.y = realPos.y + objectProperties->size.cy - METRIC_SLIDERAREAOFFSETX;

		surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);

		lineStart.x++;
		lineEnd.x++;

		surface->Line(lineStart, lineEnd, Setup::DividerLightColor);

		surface->SetPixel(lineEnd.x - 1, lineEnd.y - 1, Setup::DividerLightColor);

		sliderRect.left		= realPos.x;
		sliderRect.top		= realPos.y + (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH) - (Int) (((Float) (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
		sliderRect.right	= sliderRect.left + METRIC_SLIDERAREAHEIGHT - 1;
		sliderRect.bottom	= sliderRect.top + METRIC_SLIDERAREAWIDTH;

		if (!objectProperties->clicked)	surface->Box(sliderRect, Setup::BackgroundColor, FILLED);
		else				surface->Box(sliderRect, Setup::LightGrayColor, FILLED);

		surface->Frame(sliderRect, FRAME_UP);
	}

	prevValue = *variable;

	return Success;
}

S::Int S::GUI::Slider::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Rect	 slider;
	Rect	 actionArea;
	Int	 oldValue = *variable;
	Float	 buffer;

	if (prevValue != *variable) Paint(SP_PAINT);

	if (subtype == OR_HORZ)
	{
		slider.left	= realPos.x + (Int) (((Float) (objectProperties->size.cx - METRIC_SLIDERAREAWIDTH)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
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
		slider.top	= realPos.y + (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH) - (Int) (((Float) (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
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
			if (wnd->IsMouseOn(slider) && !objectProperties->clicked)
			{
				objectProperties->clicked = True;

				slider.left++;
				slider.top++;
				surface->Box(slider, Setup::LightGrayColor, FILLED);
				slider.left--;
				slider.top--;

				if (subtype == OR_HORZ)	mouseBias = (slider.left + METRIC_SLIDERAREAOFFSETX) - wnd->MouseX();
				else			mouseBias = (slider.top + METRIC_SLIDERAREAOFFSETX) - wnd->MouseY();

				retVal = Break;
			}
			else if (wnd->IsMouseOn(actionArea) && !objectProperties->clicked)
			{
				mouseBias = 0;

				objectProperties->clicked = True;
				Process(SM_MOUSEMOVE, 0, 0);
				objectProperties->clicked = False;

				Paint(SP_PAINT);

				retVal = Break;
			}

			break;
		case SM_LBUTTONUP:
			if (objectProperties->clicked)
			{
				objectProperties->clicked = False;

				slider.left++;
				slider.top++;
				surface->Box(slider, Setup::BackgroundColor, FILLED);
				slider.left--;
				slider.top--;

				retVal = Break;
			}

			break;
		case SM_MOUSEMOVE:
			if (objectProperties->clicked)
			{
				if (subtype == OR_HORZ)
				{
					buffer = ((Float) (endValue - startValue)) / (((Float) (objectProperties->size.cx - METRIC_SLIDERAREAWIDTH)) / ((Float) (wnd->MouseX() + mouseBias - (realPos.x + METRIC_SLIDERAREAOFFSETX))));

					*variable = startValue + Math::Round(buffer);

					if (*variable < startValue)	*variable = startValue;
					if (*variable > endValue)	*variable = endValue;
				}
				else
				{
					buffer = ((Float) (endValue - startValue)) / (((Float) (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH)) / ((Float) (wnd->MouseY() + mouseBias - (realPos.y + METRIC_SLIDERAREAOFFSETX))));

					*variable = endValue - Math::Round(buffer);

					if (*variable < startValue)	*variable = startValue;
					if (*variable > endValue)	*variable = endValue;
				}

				if (*variable != oldValue)
				{
					slider.right++;
					slider.bottom++;
					surface->Box(slider, Setup::BackgroundColor, FILLED);
					slider.right--;
					slider.bottom--;

					Paint(SP_PAINT);

					onClick.Emit(wnd->MouseX(), wnd->MouseY());
				}
			}

			break;
	}

	return retVal;
}

S::Int S::GUI::Slider::SetRange(Int rangeStart, Int rangeEnd)
{
	Int	 prevStartValue	= startValue;
	Int	 prevEndValue	= endValue;

	startValue	= rangeStart;
	endValue	= rangeEnd;

	*variable	= (Int) (((Float) (*variable) - prevStartValue) * ((Float) (endValue - startValue) / (prevEndValue - prevStartValue)) + startValue);
	*variable	= (Int) Math::Max(rangeStart, Math::Min(rangeEnd, *variable));

	SetValue(*variable);

	return Success;
}

S::Int S::GUI::Slider::SetValue(Int newValue)
{
	if (newValue < startValue)	newValue = startValue;
	if (newValue > endValue)	newValue = endValue;

	if (*variable == newValue) return Success;

	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	*variable = newValue;

	if (registered && prevVisible) Show();

	onClick.Emit(0, 0);

	return Success;
}

S::Int S::GUI::Slider::GetValue()
{
	return *variable;
}
