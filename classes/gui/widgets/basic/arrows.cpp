 /* The smooth Class Library
  * Copyright (C) 1998-2012 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/arrows.h>
#include <smooth/gui/widgets/hotspot/simplebutton.h>
#include <smooth/gui/window/window.h>
#include <smooth/misc/math.h>
#include <smooth/system/timer.h>
#include <smooth/graphics/surface.h>

const S::Short	 S::GUI::Arrows::classID = S::Object::RequestClassID();

S::GUI::Arrows::Arrows(const Point &iPos, const Size &iSize, Int sType, Int *var, Int rangeStart, Int rangeEnd) : Widget(iPos, iSize)
{
	type		= classID;
	subtype		= sType;

	startValue	= rangeStart;
	endValue	= rangeEnd;

	stepSize	= 1;

	dummyVariable	= 0;

	if (var == NIL)	variable = &dummyVariable;
	else		variable = var;

	SetValue(*variable);

	timer		= NIL;
	timerCount	= 0;
	timerDirection	= 1;

	if (GetWidth() == 0) SetWidth(subtype == OR_VERT ? 17 : 24);
	if (GetHeight() == 0) SetHeight(subtype == OR_VERT ? 24 : 17);

	onRangeChange.SetParentObject(this);

	onValueChange.SetParentObject(this);
	onValueChange.Connect(&onAction);

	SetBackgroundColor(Setup::BackgroundColor);

	arrow1Hotspot	= new HotspotSimpleButton(Point(2 + (subtype == OR_HORZ ? GetWidth() / 2 : 0), 2), GetSize() - Size(4 + (subtype == OR_HORZ ? GetWidth() / 2 : 0), 4 + (subtype == OR_HORZ ? 0 : GetHeight() / 2)));
	arrow2Hotspot	= new HotspotSimpleButton(Point(2, 2 + (subtype == OR_VERT ? GetHeight() / 2 : 0)), GetSize() - Size(4 + (subtype == OR_HORZ ? GetWidth() / 2 : 0), 4 + (subtype == OR_HORZ ? 0 : GetHeight() / 2)));

	arrow1Hotspot->onLeftButtonDown.Connect(&Arrows::OnMouseDownPlus, this);
	arrow2Hotspot->onLeftButtonDown.Connect(&Arrows::OnMouseDownMinus, this);

	arrow1Hotspot->onLeftButtonUp.Connect(&Arrows::OnMouseRelease, this);
	arrow2Hotspot->onLeftButtonUp.Connect(&Arrows::OnMouseRelease, this);

	Add(arrow1Hotspot);
	Add(arrow2Hotspot);

	onChangeSize.Connect(&Arrows::OnChangeSize, this);
}

S::GUI::Arrows::~Arrows()
{
	if (timer != NIL)
	{
		timer->Stop();

		DeleteObject(timer);

		timer = NIL;
	}

	DeleteObject(arrow1Hotspot);
	DeleteObject(arrow2Hotspot);
}

S::Int S::GUI::Arrows::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface	= GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), GetRealSize());
	Int	 arrowColor	= Setup::TextColor;

	if (!IsActive()) arrowColor = Setup::GrayTextColor;

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			surface->Frame(frame, FRAME_UP);

			if (subtype == OR_HORZ)
			{
				Point	 lineStart	= Point((frame.left + frame.right) / 2 - 1, frame.top + 1);
				Point	 lineEnd	= Point(lineStart.x, frame.bottom - 1);

				surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);
				surface->Line(lineStart + Point(1, 0), lineEnd + Point(1, 0), Setup::DividerLightColor);

				Int	 size		= Math::Round(3 * surface->GetSurfaceDPI() / 96.0);
				Point	 offset		= Point((size % 2) ? 1 : 0, (frame.GetHeight() % 2) ? 0 : 1);

				for (Int i = 0; i < size; i++)
				{
					lineStart	= Point((frame.left + frame.GetWidth() / 4) - size / 2 + i - offset.x, frame.top + frame.GetHeight() / 2 - i - offset.y);
					lineEnd		= lineStart + Point(0, 2 * i + 1);

					surface->Line(lineStart, lineEnd, arrowColor);
				}

				for (Int j = 0; j < size; j++)
				{
					lineStart	= Point((frame.right - frame.GetWidth() / 4) - (size + 1) / 2 + j, frame.top + frame.GetHeight() / 2 - size + 1 + j - offset.y);
					lineEnd		= lineStart + Point(0, 2 * (size - j) - 1);

					surface->Line(lineStart, lineEnd, arrowColor);
				}
			}
			else if (subtype == OR_VERT)
			{
				Point	 lineStart	= Point(frame.left + 1, (frame.top + frame.bottom) / 2 - 1);
				Point	 lineEnd	= Point(frame.right - 1, lineStart.y);

				surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);
				surface->Line(lineStart + Point(0, 1), lineEnd + Point(0, 1), Setup::DividerLightColor);

				Int	 size		= Math::Round(3 * surface->GetSurfaceDPI() / 96.0);

				for (Int i = 0; i < size; i++)
				{
					lineStart	= Point(frame.left + frame.GetWidth() / 2 + (IsRightToLeft() ? 1 : 0) - i, (frame.top + frame.GetHeight() / 4) - size / 2 + i);
					lineEnd		= lineStart + Point(2 * i + 1, 0);

					surface->Line(lineStart, lineEnd, arrowColor);
				}

				for (Int j = 0; j < size; j++)
				{
					lineStart	= Point(frame.left + frame.GetWidth() / 2 + (IsRightToLeft() ? 1 : 0) - size + 1 + j, (frame.bottom - frame.GetHeight() / 4) - (size + 1) / 2 + j);
					lineEnd		= lineStart + Point(2 * (size - j) - 1, 0);

					surface->Line(lineStart, lineEnd, arrowColor);
				}
			}

			break;
	}

	return Success();
}

S::Int S::GUI::Arrows::SetStepSize(UnsignedInt nStepSize)
{
	stepSize = nStepSize;

	return Success();
}

S::UnsignedInt S::GUI::Arrows::GetStepSize() const
{
	return stepSize;
}

S::Void S::GUI::Arrows::OnMouseDownPlus()
{
	if (timer == NIL)
	{
		timer = new System::Timer();

		timer->onInterval.Connect(&Arrows::OnTimer, this);
		timer->Start(0);

		timerCount	= 0;
		timerDirection	= 1;
	}
}

S::Void S::GUI::Arrows::OnMouseDownMinus()
{
	if (timer == NIL)
	{
		timer = new System::Timer();

		timer->onInterval.Connect(&Arrows::OnTimer, this);
		timer->Start(0);

		timerCount	= 0;
		timerDirection	= -1;
	}
}

S::Void S::GUI::Arrows::OnMouseRelease()
{
	if (timer != NIL)
	{
		timer->Stop();

		DeleteObject(timer);

		timer = NIL;
	}
}

S::Void S::GUI::Arrows::OnTimer()
{
	if (timerCount == 0) { timer->Stop(); timer->Start(250); }
	if (timerCount == 1) { timer->Stop(); timer->Start(100); }

	Int	 prevValue	= *variable;
	Int	 difference	= timerDirection;

	for (Int n = 1; n <= 8; n++)
	{
		if (timerCount >= 8 * n) difference = timerDirection * (Int) Math::Pow(2, n);
	}

	*variable = Math::Min(Math::Max(*variable + difference * stepSize, startValue), endValue);

	if (*variable != prevValue) onValueChange.Emit(*variable);

	timerCount++;
}

S::Int S::GUI::Arrows::SetRange(Int rangeStart, Int rangeEnd)
{
	if (startValue == rangeStart && endValue == rangeEnd) return Success();

	startValue	= rangeStart;
	endValue	= rangeEnd;

	SetValue(*variable);

	onRangeChange.Emit(startValue, endValue);

	return Success();
}

S::Int S::GUI::Arrows::SetValue(Int newValue)
{
	Int	 prevValue	= *variable;

	*variable = Math::Min(Math::Max(newValue, startValue), endValue);

	if (*variable != prevValue) onValueChange.Emit(*variable);

	return Success();
}

S::Int S::GUI::Arrows::GetValue()
{
	return *variable;
}

S::Void S::GUI::Arrows::OnChangeSize(const Size &nSize)
{
	arrow1Hotspot->SetSize(nSize - Size(4 + (subtype == OR_HORZ ? GetWidth() / 2 : 0), 4 + (subtype == OR_HORZ ? 0 : GetHeight() / 2)));
	arrow2Hotspot->SetSize(nSize - Size(4 + (subtype == OR_HORZ ? GetWidth() / 2 : 0), 4 + (subtype == OR_HORZ ? 0 : GetHeight() / 2)));
}
