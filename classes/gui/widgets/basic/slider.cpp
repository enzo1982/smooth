 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/slider.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::Slider::classID = S::Object::RequestClassID();

S::GUI::Slider::Slider(Point iPos, Size iSize, Int subType, Int *var, Int rangeStart, Int rangeEnd)
{
	type		= classID;
	subtype		= subType;
	variable	= var;
	clicked		= False;
	startValue	= rangeStart;
	endValue	= rangeEnd;
	prevValue	= 0;

	possibleContainers.AddEntry(Layer::classID);

	if (*variable < startValue)	*variable = startValue;
	else if (*variable > endValue)	*variable = endValue;

	pos		= iPos;
	size		= iSize;

	if (size.cx == 0) size.cx = size.cy;
	if (size.cy == 0) size.cy = size.cx;

	if (size.cx == 0) size.cx = 100;
	if (size.cy == 0) size.cy = 100;

	if (subType == OR_HORZ)	size.cy = 18;
	else			size.cx = 18;
}

S::GUI::Slider::~Slider()
{
	if (registered && container != NIL) container->UnregisterObject(this);
}

S::Int S::GUI::Slider::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Point	 lineStart;
	Point	 lineEnd;
	Rect	 sliderRect;

	if (prevValue != *variable)
	{
		if (subtype == OR_HORZ)
		{
			sliderRect.left		= realPos.x + (Int) (((Float) (size.cx - 9)) / ((Float) (endValue - startValue)) * ((Float) (prevValue - startValue)));
			sliderRect.top		= realPos.y;
			sliderRect.right	= sliderRect.left + 9;
			sliderRect.bottom	= sliderRect.top + 18;
		}
		else
		{
			sliderRect.left		= realPos.x;
			sliderRect.top		= realPos.y + (size.cy - 9) - (Int) (((Float) (size.cy - 9)) / ((Float) (endValue - startValue)) * ((Float) (prevValue - startValue)));
			sliderRect.right	= sliderRect.left + 18;
			sliderRect.bottom	= sliderRect.top + 10;
		}

		surface->Box(sliderRect, Setup::BackgroundColor, FILLED);
	}

	if (subtype == OR_HORZ)
	{
		lineStart.x = realPos.x + 4;
		lineStart.y = realPos.y + 8;
		lineEnd.x = realPos.x + size.cx - 4;
		lineEnd.y = lineStart.y;

		surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);

		lineStart.y++;
		lineEnd.y++;

		surface->Line(lineStart, lineEnd, Setup::DividerLightColor);

		surface->SetPixel(lineEnd.x - 1, lineEnd.y - 1, Setup::DividerLightColor);

		sliderRect.left		= realPos.x + (Int) (((Float) (size.cx - 9)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
		sliderRect.top		= realPos.y;
		sliderRect.right	= sliderRect.left + 8;
		sliderRect.bottom	= sliderRect.top + 16;

		if (!clicked)	surface->Box(sliderRect, Setup::BackgroundColor, FILLED);
		else		surface->Box(sliderRect, Setup::LightGrayColor, FILLED);

		surface->Frame(sliderRect, FRAME_UP);
	}
	else
	{
		lineStart.x = realPos.x + 8;
		lineStart.y = realPos.y + 4;
		lineEnd.x = lineStart.x;
		lineEnd.y = realPos.y + size.cy - 4;

		surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);

		lineStart.x++;
		lineEnd.x++;

		surface->Line(lineStart, lineEnd, Setup::DividerLightColor);

		surface->SetPixel(lineEnd.x - 1, lineEnd.y - 1, Setup::DividerLightColor);

		sliderRect.left		= realPos.x;
		sliderRect.top		= realPos.y + (size.cy - 9) - (Int) (((Float) (size.cy - 9)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
		sliderRect.right	= sliderRect.left + 17;
		sliderRect.bottom	= sliderRect.top + 9;

		if (!clicked)	surface->Box(sliderRect, Setup::BackgroundColor, FILLED);
		else		surface->Box(sliderRect, Setup::LightGrayColor, FILLED);

		surface->Frame(sliderRect, FRAME_UP);
	}

	prevValue = *variable;

	return Success;
}

S::Int S::GUI::Slider::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Failure;
	if (!active || !visible)	return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Rect	 slider;
	Rect	 actionArea;
	Int	 oldValue = *variable;
	Float	 buffer;

	if (prevValue != *variable) Paint(SP_PAINT);

	if (subtype == OR_HORZ)
	{
		slider.left	= realPos.x + (Int) (((Float) (size.cx - 9)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
		slider.top	= realPos.y;
		slider.right	= slider.left + 8;
		slider.bottom	= slider.top + 16;

		actionArea.left		= realPos.x + 4;
		actionArea.top		= realPos.y;
		actionArea.right	= actionArea.left + size.cx - 9;
		actionArea.bottom	= actionArea.top + 16;
	}
	else
	{
		slider.left	= realPos.x;
		slider.top	= realPos.y + (size.cy - 9) - (Int) (((Float) (size.cy - 9)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
		slider.right	= slider.left + 17;
		slider.bottom	= slider.top + 9;

		actionArea.left		= realPos.x;
		actionArea.top		= realPos.y + 4;
		actionArea.right	= actionArea.left + 17;
		actionArea.bottom	= actionArea.top + size.cy - 9;
	}

	switch (message)
	{
		case SM_LBUTTONDOWN:
			if (wnd->IsMouseOn(slider) && !clicked)
			{
				clicked = True;

				slider.left++;
				slider.top++;
				surface->Box(slider, Setup::LightGrayColor, FILLED);
				slider.left--;
				slider.top--;

				if (subtype == OR_HORZ)	mouseBias = (slider.left + 4) - wnd->MouseX();
				else			mouseBias = (slider.top + 4) - wnd->MouseY();

				retVal = Break;
			}
			else if (wnd->IsMouseOn(actionArea) && !clicked)
			{
				mouseBias = 0;

				clicked = True;
				Process(SM_MOUSEMOVE, 0, 0);
				clicked = False;

				Paint(SP_PAINT);

				retVal = Break;
			}

			break;
		case SM_LBUTTONUP:
			if (clicked)
			{
				clicked = False;

				slider.left++;
				slider.top++;
				surface->Box(slider, Setup::BackgroundColor, FILLED);
				slider.left--;
				slider.top--;

				retVal = Break;
			}

			break;
		case SM_MOUSEMOVE:
			if (clicked)
			{
				if (subtype == OR_HORZ)
				{
					buffer = ((Float) (endValue - startValue)) / (((Float) (size.cx - 9)) / ((Float) (wnd->MouseX() + mouseBias - (realPos.x + 4))));

					*variable = startValue + Math::Round(buffer);

					if (*variable < startValue)	*variable = startValue;
					if (*variable > endValue)	*variable = endValue;
				}
				else
				{
					buffer = ((Float) (endValue - startValue)) / (((Float) (size.cy - 9)) / ((Float) (wnd->MouseY() + mouseBias - (realPos.y + 4))));

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
