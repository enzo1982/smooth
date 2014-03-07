 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/scrollbar.h>
#include <smooth/gui/widgets/hotspot/hotspot.h>
#include <smooth/misc/math.h>
#include <smooth/system/timer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Short	 S::GUI::Scrollbar::classID = S::Object::RequestClassID();

S::GUI::Scrollbar::Scrollbar(const Point &iPos, const Size &iSize, Int sType, Int *var, Int rangeStart, Int rangeEnd) : Arrows(iPos, iSize, sType, var, rangeStart, rangeEnd)
{
	type		= classID;

	if (GetWidth()	== 0) SetWidth(subtype	== OR_VERT ? 17  : 120);
	if (GetHeight()	== 0) SetHeight(subtype	== OR_VERT ? 120 :  17);

	clickHotspot	= new Hotspot(Point(), Size());
	dragHotspot	= new Hotspot(Point(), Size());

	clickTimer	= new System::Timer();
	clickTimerDirection = 1;

	pageSize	= 6;
	dragging	= False;

	clickHotspot->onLeftButtonDown.Connect(&Scrollbar::OnMouseClick, this);
	clickHotspot->onLeftButtonUp.Connect(&System::Timer::Stop, clickTimer);
	clickHotspot->onMouseOut.Connect(&System::Timer::Stop, clickTimer);

	dragHotspot->onMouseDragStart.Connect(&Scrollbar::OnMouseDragStart, this);
	dragHotspot->onMouseDrag.Connect(&Scrollbar::OnMouseDrag, this);
	dragHotspot->onMouseDragEnd.Connect(&Scrollbar::OnMouseDragEnd, this);

	dragHotspot->onMouseWheel.Connect(&Scrollbar::OnMouseWheel, this);

	clickTimer->onInterval.Connect(&Scrollbar::OnMouseClickTimer, this);

	Add(clickHotspot);
	Add(dragHotspot);

	UpdateHotspotPositions();

	onRangeChange.Connect(&Scrollbar::OnValueChange, this);
	onValueChange.Connect(&Scrollbar::OnValueChange, this);
}

S::GUI::Scrollbar::~Scrollbar()
{
	DeleteObject(clickHotspot);
	DeleteObject(dragHotspot);

	DeleteObject(clickTimer);
}

S::Int S::GUI::Scrollbar::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	Surface	*surface	= GetDrawSurface();
	Point	 realPos	= GetRealPosition();
	Size	 realSize	= GetRealSize();

	Bool	 smallHotspots	= (subtype == OR_HORZ && (GetWidth() <= 55)) || (subtype == OR_VERT && (GetHeight() <= 55));
	Int	 hotspotSize	= Math::Round((smallHotspots ? 10 : (subtype == OR_HORZ ? GetHeight() : GetWidth())) * surface->GetSurfaceDPI() / 96.0) - 4;

	Rect	 arrow1Frame	= Rect(realPos + Point(subtype == OR_HORZ ? realSize.cx - (hotspotSize + 4) : 0, subtype == OR_VERT ? realSize.cy - (hotspotSize + 4) : 0), subtype == OR_HORZ ? Size(hotspotSize + 4, realSize.cy) : Size(realSize.cx, hotspotSize + 4));
	Rect	 arrow2Frame	= Rect(realPos + Point(0, 0), subtype == OR_HORZ ? Size(hotspotSize + 4, realSize.cy) : Size(realSize.cx, hotspotSize + 4));
	Int	 arrowColor	= Setup::TextColor;

	if (!IsActive()) arrowColor = Setup::InactiveTextColor;

	switch (message)
	{
		case SP_PAINT:
			surface->StartPaint(Rect(realPos, realSize));

			OnValueChange();

			surface->Box(arrow1Frame, Setup::BackgroundColor, Rect::Filled);
			surface->Box(arrow2Frame, Setup::BackgroundColor, Rect::Filled);

			surface->Frame(arrow1Frame, FRAME_UP);
			surface->Frame(arrow2Frame, FRAME_UP);

			if (subtype == OR_HORZ)
			{
				Int	 size = Math::Round(3 * surface->GetSurfaceDPI() / 96.0);

				for (Int i = 0; i < size; i++)
				{
					Point	 lineStart = Point(realPos.x + (hotspotSize + 4) / 2 - size / 2 + i, realPos.y + realSize.cy / 2 - i);
					Point	 lineEnd   = lineStart + Point(0, 2 * i + 1);

					surface->Line(lineStart, lineEnd, arrowColor);
				}

				for (Int i = 0; i < size; i++)
				{
					Point	 lineStart = Point(realPos.x + realSize.cx - (hotspotSize + 4) / 2 - (size + 1) / 2 + i, realPos.y + realSize.cy / 2 - size + 1 + i);
					Point	 lineEnd   = lineStart + Point(0, 2 * (size - i) - 1);

					surface->Line(lineStart, lineEnd, arrowColor);
				}
			}
			else if (subtype == OR_VERT)
			{
				Int	 size = Math::Round(3 * surface->GetSurfaceDPI() / 96.0);

				for (Int i = 0; i < size; i++)
				{
					Point	 lineStart = Point(realPos.x + realSize.cx / 2 + (IsRightToLeft() ? 1 : 0) - i, (realPos.y + (hotspotSize + 4) / 2) - size / 2 + i);
					Point	 lineEnd   = lineStart + Point(2 * i + 1, 0);

					surface->Line(lineStart, lineEnd, arrowColor);
				}

				for (Int i = 0; i < size; i++)
				{
					Point	 lineStart = Point(realPos.x + realSize.cx / 2 + (IsRightToLeft() ? 1 : 0) - size + 1 + i, (realPos.y + realSize.cy - (hotspotSize + 4) / 2) - (size + 1) / 2 + i);
					Point	 lineEnd   = lineStart + Point(2 * (size - i) - 1, 0);

					surface->Line(lineStart, lineEnd, arrowColor);
				}
			}

			surface->EndPaint();

			break;
	}

	return Success();
}

S::Void S::GUI::Scrollbar::OnMouseClick(const Point &mousePos)
{
	if (dragHotspot->IsMouseOver()) return;

	Int	 value = 0;

	if (subtype == OR_HORZ)	value = (mousePos.x < dragHotspot->GetRealPosition().x) ? -pageSize : pageSize;
	else			value = (mousePos.y < dragHotspot->GetRealPosition().y) ? -pageSize : pageSize;

	if (clickTimer->GetStatus() == System::TIMER_STOPPED)
	{
		clickTimer->Start(250);
		clickTimerDirection = (value > 0 ? 1 : -1);
	}

	if (value / clickTimerDirection >= 0) SetValue(*variable + value);
}

S::Void S::GUI::Scrollbar::OnMouseClickTimer()
{
	Window	*window	= container->GetContainerWindow();

	OnMouseClick(window->GetMousePosition());

	clickTimer->Restart(100);
}

S::Void S::GUI::Scrollbar::OnMouseWheel(Int value)
{
	if (subtype != OR_VERT) return;

	Window	*window	= container->GetContainerWindow();

	if (IsMouseOver() || window->IsMouseOn(Rect(container->GetRealPosition(), container->GetRealSize()))) SetValue(*variable - (value * stepSize));
}

S::Void S::GUI::Scrollbar::OnMouseDragStart(const Point &mousePos)
{
	if (subtype == OR_HORZ)	mouseBias = mousePos.x - dragHotspot->GetRealPosition().x;
	else			mouseBias = mousePos.y - dragHotspot->GetRealPosition().y;

	dragging = True;
}

S::Void S::GUI::Scrollbar::OnMouseDrag(const Point &mousePos)
{
	Point	 realPos  = GetRealPosition();
	Size	 realSize = GetRealSize();

	Int	 value = 0;

	Surface	*surface	= GetDrawSurface();

	Bool	 smallHotspots	= (subtype == OR_HORZ && (GetWidth() <= 55)) || (subtype == OR_VERT && (GetHeight() <= 55));
	Int	 hotspotSize	= Math::Round((smallHotspots ? 10 : (subtype == OR_HORZ ? GetHeight() : GetWidth())) * surface->GetSurfaceDPI() / 96.0) - 4;

	if (subtype == OR_HORZ)	value = Math::Round((((Float) (endValue - startValue)) / ((Float) realSize.cx - 3 * (hotspotSize + 4))) * ((Float) (mousePos.x - mouseBias - (realPos.x + hotspotSize + 4))));
	else			value = Math::Round((((Float) (endValue - startValue)) / ((Float) realSize.cy - 3 * (hotspotSize + 4))) * ((Float) (mousePos.y - mouseBias - (realPos.y + hotspotSize + 4))));

	SetValue(startValue + value);
}

S::Void S::GUI::Scrollbar::OnMouseDragEnd(const Point &mousePos)
{
	dragging = False;

	OnValueChange();
}

S::Void S::GUI::Scrollbar::OnValueChange()
{
	UpdateHotspotPositions();

	if (!IsRegistered() || !IsVisible()) return;

	Surface	*surface	= GetDrawSurface();
	Point	 realPos	= GetRealPosition();
	Size	 realSize	= GetRealSize();

	Bool	 smallHotspots	= (subtype == OR_HORZ && (GetWidth() <= 55)) || (subtype == OR_VERT && (GetHeight() <= 55));
	Int	 hotspotSize	= Math::Round((smallHotspots ? 10 : (subtype == OR_HORZ ? GetHeight() : GetWidth())) * surface->GetSurfaceDPI() / 96.0) - 4;

	Rect	 backFrame	= Rect(realPos + (subtype == OR_HORZ ? Point(hotspotSize + 4, 0) : Point(0, hotspotSize + 4)), realSize - (subtype == OR_HORZ ? Size(2 * (hotspotSize + 4), 0) : Size(0, 2 * (hotspotSize + 4))));
	Rect	 sliderFrame	= Rect(realPos + (subtype == OR_HORZ ? Point(hotspotSize + 4 + (Int) (((Float) realSize.cx - 3 * (hotspotSize + 4)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue))), 0) : Point(0, hotspotSize + 4 + (Int) (((Float) realSize.cy - 3 * (hotspotSize + 4)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue))))), subtype == OR_HORZ ? Size(hotspotSize + 4, realSize.cy) : Size(realSize.cx, hotspotSize + 4));

	surface->StartPaint(backFrame);

	surface->Box(backFrame, Setup::LightGrayColor, Rect::Filled);

	if (!dragging) surface->Box(sliderFrame, Setup::BackgroundColor, Rect::Filled);

	surface->Frame(sliderFrame, FRAME_UP);

	surface->EndPaint();
}

S::Void S::GUI::Scrollbar::UpdateHotspotPositions()
{
	Bool	 smallHotspots	= (subtype == OR_HORZ && (GetWidth() <= 55)) || (subtype == OR_VERT && (GetHeight() <= 55));
	Int	 hotspotSize	= (smallHotspots ? 10 : (subtype == OR_HORZ ? GetHeight() : GetWidth())) - 4;

	arrow1Hotspot->SetMetrics(Point(2 + (subtype == OR_HORZ ? GetWidth() - (hotspotSize + 4) : 0), 2 + (subtype == OR_VERT ? GetHeight() - (hotspotSize + 4) : 0)), subtype == OR_HORZ ? Size(hotspotSize, GetHeight() - 4) : Size(GetWidth() - 4, hotspotSize));
	arrow2Hotspot->SetMetrics(Point(2, 2), subtype == OR_HORZ ? Size(hotspotSize, GetHeight() - 4) : Size(GetWidth() - 4, hotspotSize));

	clickHotspot->SetMetrics(subtype == OR_HORZ ? Point(hotspotSize + 4, 0) : Point(0, hotspotSize + 4), GetSize() - (subtype == OR_HORZ ? Size(2 * (hotspotSize + 4), 0) : Size(0, 2 * (hotspotSize + 4))));
	dragHotspot->SetMetrics(subtype == OR_HORZ ? Point(hotspotSize + 4 + (Int) (((Float) GetWidth() - 3 * (hotspotSize + 4)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue))), 0) : Point(0, hotspotSize + 4 + (Int) (((Float) GetHeight() - 3 * (hotspotSize + 4)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)))), subtype == OR_HORZ ? Size(hotspotSize + 4, GetHeight()) : Size(GetWidth(), hotspotSize + 4));
}
