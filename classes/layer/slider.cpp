 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
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

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_SLIDER = S::Object::RequestObjectID();

S::Slider::Slider(Point pos, Size size, Int subType, Int *var, Int rangeStart, Int rangeEnd)
{
	type				= OBJ_SLIDER;
	subtype				= subType;
	variable			= var;
	objectProperties->clicked	= False;
	startValue			= rangeStart;
	endValue			= rangeEnd;
	prevValue			= 0;

	possibleContainers.AddEntry(OBJ_LAYER);

	if (*variable < startValue)	*variable = startValue;
	else if (*variable > endValue)	*variable = endValue;

	objectProperties->pos.x = Math::Round(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * SMOOTH::Setup::FontSize);

	if (size.cx == 0) size.cx = size.cy;
	if (size.cy == 0) size.cy = size.cx;

	if (size.cx == 0)	objectProperties->size.cx = Math::Round(100 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cx = Math::Round(size.cx * SMOOTH::Setup::FontSize);
	if (size.cy == 0)	objectProperties->size.cy = Math::Round(100 * SMOOTH::Setup::FontSize);
	else			objectProperties->size.cy = Math::Round(size.cy * SMOOTH::Setup::FontSize);

	if (subType == OR_HORZ)	objectProperties->size.cy = METRIC_SLIDERAREAHEIGHT;
	else			objectProperties->size.cx = METRIC_SLIDERAREAHEIGHT;
}

S::Slider::~Slider()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::Slider::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	HDC	 dc = GetContext(wnd);
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

		PaintPixel(dc, Point(lineEnd.x - 1, lineEnd.y - 1), SMOOTH::Setup::DividerLightColor);

		sliderRect.left		= realPos.x + (Int) (((Float) (objectProperties->size.cx - METRIC_SLIDERAREAWIDTH)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
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

		PaintPixel(dc, Point(lineEnd.x - 1, lineEnd.y - 1), SMOOTH::Setup::DividerLightColor);

		sliderRect.left		= realPos.x;
		sliderRect.top		= realPos.y + (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH) - (Int) (((Float) (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
		sliderRect.right	= sliderRect.left + METRIC_SLIDERAREAHEIGHT - 1;
		sliderRect.bottom	= sliderRect.top + METRIC_SLIDERAREAWIDTH;

		if (!objectProperties->clicked)	Box(dc, sliderRect, SMOOTH::Setup::BackgroundColor, FILLED);
		else				Box(dc, sliderRect, SMOOTH::Setup::LightGrayColor, FILLED);

		Frame(dc, sliderRect, FRAME_UP);
	}

	prevValue = *variable;

	FreeContext(wnd, dc);

	return Success;
}

S::Int S::Slider::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Rect	 slider;
	Rect	 actionArea;
	Int	 oldValue = *variable;
	Float	 buffer;
	HDC	 dc;

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
			dc = GetContext(wnd);

			if (IsMouseOn(wnd->hwnd, slider, WINDOW) && !objectProperties->clicked)
			{
				objectProperties->clicked = True;

				slider.left++;
				slider.top++;
				Box(dc, slider, SMOOTH::Setup::LightGrayColor, FILLED);
				slider.left--;
				slider.top--;

				if (subtype == OR_HORZ)	mouseBias = (slider.left + METRIC_SLIDERAREAOFFSETX) - MouseX(wnd->hwnd, WINDOW);
				else			mouseBias = (slider.top + METRIC_SLIDERAREAOFFSETX) - MouseY(wnd->hwnd, WINDOW);

				retVal = Break;
			}
			else if (IsMouseOn(wnd->hwnd, actionArea, WINDOW) && !objectProperties->clicked)
			{
				mouseBias = 0;

				objectProperties->clicked = True;
				Process(SM_MOUSEMOVE, 0, 0);
				objectProperties->clicked = False;

				Paint(SP_PAINT);

				retVal = Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONUP:
			dc = GetContext(wnd);

			if (objectProperties->clicked)
			{
				objectProperties->clicked = False;

				slider.left++;
				slider.top++;
				Box(dc, slider, SMOOTH::Setup::BackgroundColor, FILLED);
				slider.left--;
				slider.top--;

				retVal = Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			if (objectProperties->clicked)
			{
				if (subtype == OR_HORZ)
				{
					buffer = ((Float) (endValue - startValue)) / (((Float) (objectProperties->size.cx - METRIC_SLIDERAREAWIDTH)) / ((Float) (MouseX(wnd->hwnd, WINDOW) + mouseBias - (realPos.x + METRIC_SLIDERAREAOFFSETX))));

					*variable = startValue + Math::Round(buffer);

					if (*variable < startValue)	*variable = startValue;
					if (*variable > endValue)	*variable = endValue;
				}
				else
				{
					buffer = ((Float) (endValue - startValue)) / (((Float) (objectProperties->size.cy - METRIC_SLIDERAREAWIDTH)) / ((Float) (MouseY(wnd->hwnd, WINDOW) + mouseBias - (realPos.y + METRIC_SLIDERAREAOFFSETX))));

					*variable = endValue - Math::Round(buffer);

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

					onClick.Emit();
				}
			}

			FreeContext(wnd, dc);

			break;
	}

	return retVal;
}

S::Int S::Slider::SetRange(Int rangeStart, Int rangeEnd)
{
	Int	 prevStartValue	= startValue;
	Int	 prevEndValue	= endValue;

	startValue	= rangeStart;
	endValue	= rangeEnd;

	*variable	= ((*variable) - prevStartValue) * ((endValue - startValue) / (prevEndValue - prevStartValue)) + startValue;

	SetValue(*variable);

	return Success;
}

S::Int S::Slider::SetValue(Int newValue)
{
	if (newValue < startValue)	newValue = startValue;
	if (newValue > endValue)	newValue = endValue;

	if (*variable == newValue) return Success;

	Bool	 prevVisible = visible;

	if (registered && visible) Hide();

	*variable = newValue;

	if (registered && prevVisible) Show();

	onClick.Emit();

	return Success;
}

S::Int S::Slider::GetValue()
{
	return *variable;
}
