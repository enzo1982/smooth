 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/slider.h>
#include <smooth/gui/widgets/hotspot/hotspot.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::Slider::classID = S::Object::RequestClassID();

S::GUI::Slider::Slider(const Point &iPos, const Size &iSize, Int sType, Int *var, Int rangeStart, Int rangeEnd) : Widget(iPos, iSize)
{
	type		= classID;
	subtype		= sType;
	startValue	= rangeStart;
	endValue	= rangeEnd;

	dummyVariable	= 0;

	if (var == NIL)	variable = &dummyVariable;
	else		variable = var;

	if (GetWidth() == 0 && subtype == OR_HORZ) SetWidth(100);
	if (GetHeight() == 0 && subtype == OR_VERT) SetHeight(100);

	if (subtype == OR_HORZ)	SetHeight(18);
	else			SetWidth(18);

	onValueChange.SetParentObject(this);
	onValueChange.Connect(&onAction);

	clickHotspot	= new Hotspot(subtype == OR_HORZ ? Point(4, 1) : Point(1, 4), GetSize() - (subtype == OR_HORZ ? Size(8, 2) : Size(2, 8)));
	dragHotspot	= new Hotspot(Point(), Size());

	dragging	= False;

	clickHotspot->onLeftButtonClick.Connect(&Slider::OnMouseClick, this);

	dragHotspot->onMouseDragStart.Connect(&Slider::OnMouseDragStart, this);
	dragHotspot->onMouseDrag.Connect(&Slider::OnMouseDrag, this);
	dragHotspot->onMouseDragEnd.Connect(&Slider::OnMouseDragEnd, this);

	RegisterObject(clickHotspot);
	RegisterObject(dragHotspot);

	SetValue(*variable);
}

S::GUI::Slider::~Slider()
{
	DeleteObject(clickHotspot);
	DeleteObject(dragHotspot);
}

S::Int S::GUI::Slider::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	EnterProtectedRegion();

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 sliderRect;

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			surface->Box(Rect(realPos, GetSize()), Setup::BackgroundColor, FILLED);

			if (subtype == OR_HORZ)	surface->Bar(realPos + Point(4, 8), realPos + Point(GetWidth() - 4, 8), OR_HORZ);
			else			surface->Bar(realPos + Point(8, 4), realPos + Point(8, GetHeight() - 4), OR_VERT);

			if (subtype == OR_HORZ)	sliderRect = Rect(realPos + Point((Int) (((Float) (GetWidth() - 9)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue))), 0), Size(9, 17));
			else			sliderRect = Rect(realPos + Point(0, (GetHeight() - 10) - (Int) (((Float) (GetHeight() - 10)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)))), Size(18, 10));

			if (!dragging)	surface->Box(sliderRect, Setup::BackgroundColor, FILLED);
			else		surface->Box(sliderRect, Setup::LightGrayColor, FILLED);

			surface->Frame(sliderRect, FRAME_UP);

			break;
	}

	LeaveProtectedRegion();

	return Success();
}

S::Int S::GUI::Slider::SetRange(Int rangeStart, Int rangeEnd)
{
	startValue	= rangeStart;
	endValue	= rangeEnd;

	SetValue(*variable);

	return Success();
}

S::Int S::GUI::Slider::SetValue(Int newValue)
{
	Int	 prevValue	= *variable;

	*variable = Math::Min(Math::Max(newValue, startValue), endValue);

	Paint(SP_PAINT);

	UpdateHotspotPositions();

	if (*variable != prevValue) onValueChange.Emit(*variable);

	return Success();
}

S::Int S::GUI::Slider::GetValue()
{
	return *variable;
}

S::Void S::GUI::Slider::OnMouseClick(const Point &mousePos)
{
	Int	 value = 0;

	if (subtype == OR_HORZ)	value = Math::Round(((Float) (endValue - startValue)) / (((Float) GetWidth() - 9) / ((Float) (mousePos.x - (GetRealPosition().x + 4)))));
	else			value = Math::Round(((Float) (endValue - startValue)) / (((Float) GetHeight() - 9) / ((Float) (mousePos.y - (GetRealPosition().y + 4)))));

	if (!dragging)
	{
		if (subtype == OR_HORZ)	SetValue(startValue + value);
		else			SetValue(endValue - value);
	}
}

S::Void S::GUI::Slider::OnMouseDragStart(const Point &mousePos)
{
	if (subtype == OR_HORZ)	mouseBias = (GetRealPosition().x + (Int) (((Float) (GetWidth() - 9)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue))) + 4) - mousePos.x;
	else			mouseBias = (GetRealPosition().y + (GetHeight() - 9) - (Int) (((Float) (GetHeight() - 9)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue))) + 4) - mousePos.y;

	dragging = True;
}

S::Void S::GUI::Slider::OnMouseDrag(const Point &mousePos)
{
	Int	 value = 0;

	if (subtype == OR_HORZ)	value = Math::Round(((Float) (endValue - startValue)) / (((Float) GetWidth() - 9) / ((Float) (mousePos.x + mouseBias - (GetRealPosition().x + 4)))));
	else			value = Math::Round(((Float) (endValue - startValue)) / (((Float) GetHeight() - 9) / ((Float) (mousePos.y + mouseBias - (GetRealPosition().y + 4)))));

	if (subtype == OR_HORZ)	SetValue(startValue + value);
	else			SetValue(endValue - value);
}

S::Void S::GUI::Slider::OnMouseDragEnd(const Point &mousePos)
{
	dragging = False;

	Paint(SP_PAINT);
}

S::Void S::GUI::Slider::UpdateHotspotPositions()
{
	dragHotspot->SetMetrics(subtype == OR_HORZ ? Point((Int) (((Float) (GetWidth() - 9)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue))), 0) : Point(0, (GetHeight() - 9) - (Int) (((Float) (GetHeight() - 9)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)))), subtype == OR_HORZ ? Size(8, 16) : Size(16, 8));
}
