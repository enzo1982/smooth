 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

const S::Int	 S::GUI::Scrollbar::classID = S::Object::RequestClassID();

S::GUI::Scrollbar::Scrollbar(const Point &iPos, const Size &iSize, Int sType, Int *var, Int rangeStart, Int rangeEnd) : Arrows(iPos, iSize, sType, var, rangeStart, rangeEnd)
{
	type		= classID;

	if (GetWidth() == 0) SetWidth(subtype == OR_VERT ? 17 : 120);
	if (GetHeight() == 0) SetHeight(subtype == OR_VERT ? 120 : 17);

	clickHotspot	= new Hotspot(Point(), Size());
	dragHotspot	= new Hotspot(Point(), Size());

	dragging	= False;

	clickHotspot->onLeftButtonClick.Connect(&Scrollbar::OnMouseClick, this);

	dragHotspot->onMouseDragStart.Connect(&Scrollbar::OnMouseDragStart, this);
	dragHotspot->onMouseDrag.Connect(&Scrollbar::OnMouseDrag, this);
	dragHotspot->onMouseDragEnd.Connect(&Scrollbar::OnMouseDragEnd, this);

	dragHotspot->onMouseWheel.Connect(&Scrollbar::OnMouseWheel, this);

	RegisterObject(clickHotspot);
	RegisterObject(dragHotspot);

	UpdateHotspotPositions();

	onValueChange.Connect(&Scrollbar::OnValueChange, this);
}

S::GUI::Scrollbar::~Scrollbar()
{
	DeleteObject(clickHotspot);
	DeleteObject(dragHotspot);
}

S::Int S::GUI::Scrollbar::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	EnterProtectedRegion();

	Surface	*surface	= container->GetDrawSurface();
	Point	 realPos	= GetRealPosition();
	Rect	 arrow1Frame	= Rect(realPos + Point(subtype == OR_HORZ ? GetWidth() - GetHeight() : 0, subtype == OR_VERT ? GetHeight() - GetWidth() : 0), subtype == OR_HORZ ? Size(GetHeight(), GetHeight()) : Size(GetWidth(), GetWidth()));
	Rect	 arrow2Frame	= Rect(realPos + Point(0, 0), subtype == OR_HORZ ? Size(GetHeight(), GetHeight()) : Size(GetWidth(), GetWidth()));
	Int	 arrowColor	= Setup::TextColor;

	if (!active) arrowColor = Setup::GrayTextColor;

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			OnValueChange(*variable);

			surface->Frame(arrow1Frame, FRAME_UP);
			surface->Frame(arrow2Frame, FRAME_UP);

			if (subtype == OR_HORZ)
			{
				for (Int i = 0; i < 4; i++)
				{
					Point	 lineStart	= Point(realPos.x + GetHeight() / 2 - 2 + i, realPos.y + GetHeight() / 2 - i);
					Point	 lineEnd	= lineStart + Point(0, 2 * i + 1);

					surface->Line(lineStart, lineEnd, arrowColor);
				}

				for (Int j = 0; j < 4; j++)
				{
					Point	 lineStart	= Point(realPos.x + GetWidth() - GetHeight() / 2 - 2 + j, realPos.y + GetHeight() / 2 - 3 + j);
					Point	 lineEnd	= lineStart + Point(0, 7 - 2 * j);

					surface->Line(lineStart, lineEnd, arrowColor);
				}
			}
			else if (subtype == OR_VERT)
			{
				for (Int i = 0; i < 4; i++)
				{
					Point	 lineStart	= Point(realPos.x + GetWidth() / 2 + (Setup::rightToLeft ? 1 : 0) - i, (realPos.y + GetWidth() / 2) - 2 + i);
					Point	 lineEnd	= lineStart + Point(2 * i + 1, 0);

					surface->Line(lineStart, lineEnd, arrowColor);
				}

				for (Int j = 0; j < 4; j++)
				{
					Point	 lineStart	= Point(realPos.x + GetWidth() / 2 - 3 + (Setup::rightToLeft ? 1 : 0) + j, (realPos.y + GetHeight() - GetWidth() / 2) - 2 + j);
					Point	 lineEnd	= lineStart + Point(7 - 2 * j, 0);

					surface->Line(lineStart, lineEnd, arrowColor);
				}
			}

			break;
	}

	LeaveProtectedRegion();

	return Success();
}

S::Void S::GUI::Scrollbar::OnMouseClick(const Point &mousePos)
{
	Int	 value = 0;

	if (subtype == OR_HORZ)	value = Math::Round(((Float) (endValue - startValue)) / (((Float) GetWidth() - 3 * GetHeight()) / ((Float) (mousePos.x - (GetRealPosition().x + 1.5 * GetHeight())))));
	else			value = Math::Round(((Float) (endValue - startValue)) / (((Float) GetHeight() - 3 * GetWidth()) / ((Float) (mousePos.y - (GetRealPosition().y + 1.5 * GetWidth())))));

	if (!dragging) SetValue(startValue + value);
}

S::Void S::GUI::Scrollbar::OnMouseWheel(Int value)
{
	SetValue(*variable - value);
}

S::Void S::GUI::Scrollbar::OnMouseDragStart(const Point &mousePos)
{
	if (subtype == OR_HORZ)	mouseBias = (GetRealPosition().x + dragHotspot->GetX() + GetHeight() / 2) - mousePos.x;
	else			mouseBias = (GetRealPosition().y + dragHotspot->GetY() + GetWidth() / 2) - mousePos.y;

	dragging = True;
}

S::Void S::GUI::Scrollbar::OnMouseDrag(const Point &mousePos)
{
	Int	 value = 0;

	if (subtype == OR_HORZ)	value = Math::Round(((Float) (endValue - startValue)) / (((Float) GetWidth() - 3 * GetHeight()) / ((Float) (mousePos.x + mouseBias - (GetRealPosition().x + 1.5 * GetHeight())))));
	else			value = Math::Round(((Float) (endValue - startValue)) / (((Float) GetHeight() - 3 * GetWidth()) / ((Float) (mousePos.y + mouseBias - (GetRealPosition().y + 1.5 * GetWidth())))));

	SetValue(startValue + value);
}

S::Void S::GUI::Scrollbar::OnMouseDragEnd(const Point &mousePos)
{
	dragging = False;

	OnValueChange(*variable);
}

S::Void S::GUI::Scrollbar::OnValueChange(Int value)
{
	UpdateHotspotPositions();

	if (!IsRegistered() || !IsVisible()) return;

	Surface	*surface	= container->GetDrawSurface();
	Rect	 backFrame	= Rect(GetRealPosition() + (subtype == OR_HORZ ? Point(GetHeight(), 0) : Point(0, GetWidth())), GetSize() - (subtype == OR_HORZ ? Size(2 * GetHeight(), 0) : Size(0, 2 * GetWidth())));
	Rect	 sliderFrame	= Rect(GetRealPosition() + (subtype == OR_HORZ ? Point(GetHeight() + (Int) (((Float) GetWidth() - 3 * GetHeight()) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue))), 0) : Point(0, GetWidth() + (Int) (((Float) GetHeight() - 3 * GetWidth()) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue))))), subtype == OR_HORZ ? Size(GetHeight(), GetHeight()) : Size(GetWidth(), GetWidth()));

	surface->Box(backFrame, Setup::LightGrayColor, FILLED);

	if (!dragging) surface->Box(sliderFrame, Setup::BackgroundColor, FILLED);

	surface->Frame(sliderFrame, FRAME_UP);
}

S::Void S::GUI::Scrollbar::UpdateHotspotPositions()
{
	arrow1Hotspot->SetMetrics(Point(2 + (subtype == OR_HORZ ? GetWidth() - GetHeight() : 0), 2 + (subtype == OR_VERT ? GetHeight() - GetWidth() : 0)), (subtype == OR_HORZ ? Size(GetHeight(), GetHeight()) : Size(GetWidth(), GetWidth())) - Size(4, 4));
	arrow2Hotspot->SetMetrics(Point(2, 2), (subtype == OR_HORZ ? Size(GetHeight(), GetHeight()) : Size(GetWidth(), GetWidth())) - Size(4, 4));

	clickHotspot->SetMetrics(subtype == OR_HORZ ? Point(GetHeight(), 0) : Point(0, GetWidth()), GetSize() - (subtype == OR_HORZ ? Size(2 * GetHeight(), 0) : Size(0, 2 * GetWidth())));
	dragHotspot->SetMetrics(subtype == OR_HORZ ? Point(GetHeight() + (Int) (((Float) GetWidth() - 3 * GetHeight()) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue))), 0) : Point(0, GetWidth() + (Int) (((Float) GetHeight() - 3 * GetWidth()) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)))), subtype == OR_HORZ ? Size(GetHeight(), GetHeight()) : Size(GetWidth(), GetWidth()));
}
