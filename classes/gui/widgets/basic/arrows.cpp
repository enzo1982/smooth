 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/misc/math.h>
#include <smooth/gui/widgets/basic/arrows.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/objectproperties.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/system/timer.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

const S::Int	 S::GUI::Arrows::classID = S::Object::RequestClassID();

S::GUI::Arrows::Arrows(Point pos, Size size, Int sType, Int *var, Int rangeStart, Int rangeEnd)
{
	type		= classID;
	subtype		= sType;

	arrow1Checked	= False;
	arrow1Clicked	= False;
	arrow2Checked	= False;
	arrow2Clicked	= False;

	startValue	= rangeStart;
	endValue	= rangeEnd;

	variable	= var;

	timerActive	= False;
	timer		= NIL;

	possibleContainers.AddEntry(Layer::classID);

	objectProperties->pos	= pos;
	objectProperties->size	= size;

	if (subtype == OR_VERT)
	{
		if (objectProperties->size.cx == 0) objectProperties->size.cx = 18;
		if (objectProperties->size.cy == 0) objectProperties->size.cy = 24;
	}
	else
	{
		if (objectProperties->size.cx == 0) objectProperties->size.cx = 24;
		if (objectProperties->size.cy == 0) objectProperties->size.cy = 18;
	}
}

S::GUI::Arrows::~Arrows()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);

	if (timer != NIL)
	{
		timer->Stop();

		DeleteObject(timer);

		timer = NIL;
	}
}

S::Int S::GUI::Arrows::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Point	 lineStart;
	Point	 lineEnd;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	surface->Frame(frame, FRAME_UP);

	if (subtype == OR_HORZ)
	{
		lineStart.x = (frame.left + frame.right) / 2;
		lineStart.y = frame.top + 1;
		lineEnd.x = lineStart.x;
		lineEnd.y = frame.bottom;

		surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);

		lineStart.x++;
		lineEnd.x++;

		surface->Line(lineStart, lineEnd, Setup::DividerLightColor);

		lineStart.x = (frame.left + (frame.left + frame.right) / 2) / 2 - 1;
		lineStart.y = (frame.bottom + frame.top) / 2;
		lineEnd.x = lineStart.x;
		lineEnd.y = lineStart.y + 1;

		for (Int i = 0; i < 4; i++)
		{
			surface->Line(lineStart, lineEnd, Setup::TextColor);

			lineStart.x++;
			lineStart.y--;
			lineEnd.x++;
			lineEnd.y++;
		}

		lineStart.x = (frame.right + (frame.left + frame.right) / 2) / 2 - 1;
		lineStart.y = (frame.bottom + frame.top) / 2 - 3;
		lineEnd.x = lineStart.x;
		lineEnd.y = lineStart.y + 7;

		for (Int j = 0; j < 4; j++)
		{
			surface->Line(lineStart, lineEnd, Setup::TextColor);

			lineStart.x++;
			lineStart.y++;
			lineEnd.x++;
			lineEnd.y--;
		}
	}
	else if (subtype == OR_VERT)
	{
		lineStart.x = frame.left + 1;
		lineStart.y = (frame.top + frame.bottom) / 2;
		lineEnd.x = frame.right;
		lineEnd.y = lineStart.y;

		surface->Line(lineStart, lineEnd, Setup::DividerDarkColor);

		lineStart.y++;
		lineEnd.y++;

		surface->Line(lineStart, lineEnd, Setup::DividerLightColor);

		lineStart.x = (frame.right + frame.left) / 2 + (Setup::rightToLeft ? 1 : 0);
		lineStart.y = (frame.top + (frame.top+frame.bottom) / 2) / 2 - 1;
		lineEnd.x = lineStart.x + 1;
		lineEnd.y = lineStart.y;

		for (Int k = 0; k < 4; k++)
		{
			surface->Line(lineStart, lineEnd, Setup::TextColor);

			lineStart.x--;
			lineStart.y++;
			lineEnd.x++;
			lineEnd.y++;
		}

		lineStart.x = (frame.right + frame.left) / 2 - 3 + (Setup::rightToLeft ? 1 : 0);
		lineStart.y = (frame.bottom + (frame.top + frame.bottom) / 2) / 2 - 1;
		lineEnd.x = lineStart.x + 7;
		lineEnd.y = lineStart.y;

		for (Int l = 0; l < 4; l++)
		{
			surface->Line(lineStart, lineEnd, Setup::TextColor);

			lineStart.x++;
			lineStart.y++;
			lineEnd.x--;
			lineEnd.y++;
		}
	}

	return Success;
}

S::Int S::GUI::Arrows::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Rect	 frame;
	Rect	 arrow1Frame;
	Rect	 arrow2Frame;
	Int	 prevValue = *variable;

	frame.left	= realPos.x + 2;
	frame.top	= realPos.y + 2;
	frame.right	= realPos.x + objectProperties->size.cx - 3;
	frame.bottom	= realPos.y + objectProperties->size.cy - 3;

	switch (message)
	{
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			if (!timerActive && timer == NIL && (arrow1Checked || arrow2Checked))
			{
				timer = new System::Timer();

				timer->onInterval.Connect(&Arrows::TimerProc, this);
				timer->Start(250);

				timerCount = 1;
				timerActive = True;
			}

			if (arrow1Checked)
			{
				arrow1Clicked = True;

				if (subtype == OR_VERT)	frame.bottom = realPos.y + objectProperties->size.cy / 2 - 3;
				else			frame.right = realPos.x + objectProperties->size.cx / 2 - 3;

				surface->Frame(frame, FRAME_DOWN);

				if (subtype == OR_HORZ)	(*variable)--;
				else			(*variable)++;

				if (*variable < startValue)	*variable = startValue;
				else if (*variable > endValue)	*variable = endValue;

				if (*variable != prevValue) onClick.Emit(wnd->MouseX(), wnd->MouseY());

				retVal = Break;
			}
			else if (arrow2Checked)
			{
				arrow2Clicked = True;

				if (subtype == OR_VERT)	frame.top = realPos.y + objectProperties->size.cy / 2 + 2;
				else			frame.left = realPos.x + objectProperties->size.cx / 2 + 2;

				surface->Frame(frame, FRAME_DOWN);

				if (subtype == OR_HORZ)	(*variable)++;
				else			(*variable)--;

				if (*variable < startValue)	*variable = startValue;
				else if (*variable > endValue)	*variable = endValue;

				if (*variable != prevValue) onClick.Emit(wnd->MouseX(), wnd->MouseY());

				retVal = Break;
			}

			break;
		case SM_LBUTTONUP:
			if (timerActive) timerActive = False;

			if (arrow1Clicked)
			{
				arrow1Clicked = False;
				arrow1Checked = False;

				if (subtype == OR_VERT)	frame.bottom = realPos.y + objectProperties->size.cy / 2 - 3;
				else			frame.right = realPos.x + objectProperties->size.cx / 2 - 3;

				frame.right++;
				frame.bottom++;
				surface->Box(frame, Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;

				Process(SM_MOUSEMOVE, 0, 0);

				retVal = Break;
			}
			else if (arrow2Clicked)
			{
				arrow2Clicked = False;
				arrow2Checked = False;

				if (subtype == OR_VERT)	frame.top = realPos.y + objectProperties->size.cy / 2 + 2;
				else			frame.left = realPos.x + objectProperties->size.cx / 2 + 2;

				frame.right++;
				frame.bottom++;
				surface->Box(frame, Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;

				Process(SM_MOUSEMOVE, 0, 0);

				retVal = Break;
			}

			break;
		case SM_MOUSEMOVE:
			arrow1Frame = frame;
			arrow2Frame = frame;

			if (subtype == OR_VERT)	arrow1Frame.bottom = realPos.y + objectProperties->size.cy / 2 - 3;
			else			arrow1Frame.right = realPos.x + objectProperties->size.cx / 2 - 3;

			if (subtype == OR_VERT)	arrow2Frame.top = realPos.y + objectProperties->size.cy / 2 + 2;
			else			arrow2Frame.left = realPos.x + objectProperties->size.cx / 2 + 2;

			if (!arrow1Checked && wnd->IsMouseOn(arrow1Frame))
			{
				arrow1Checked = True;

				surface->Frame(arrow1Frame, FRAME_UP);
			}
			else if (arrow1Checked && !wnd->IsMouseOn(arrow1Frame))
			{
				if (timerActive) timerActive = False;

				arrow1Checked = False;
				arrow1Clicked = False;

				arrow1Frame.right++;
				arrow1Frame.bottom++;
				surface->Box(arrow1Frame, Setup::BackgroundColor, OUTLINED);
				arrow1Frame.right--;
				arrow1Frame.bottom--;
			}
			else if (!arrow2Checked && wnd->IsMouseOn(arrow2Frame))
			{
				arrow2Checked = True;

				surface->Frame(arrow2Frame, FRAME_UP);
			}
			else if (arrow2Checked && !wnd->IsMouseOn(arrow2Frame))
			{
				if (timerActive) timerActive = False;

				arrow2Checked = False;
				arrow2Clicked = False;

				arrow2Frame.right++;
				arrow2Frame.bottom++;
				surface->Box(arrow2Frame, Setup::BackgroundColor, OUTLINED);
				arrow2Frame.right--;
				arrow2Frame.bottom--;
			}

			break;
		case WM_KILLFOCUS:
		case WM_ACTIVATEAPP:
			return Process(SM_LBUTTONUP, 0, 0);
	}

	return retVal;
}

S::Void S::GUI::Arrows::TimerProc()
{
	if (!registered)		return;
	if (!active || !visible)	return;

	Int	 prevValue = *variable;
	Int	 plus = 1;

	if (!timerActive && timer != NIL)
	{
		timer->Stop();

		DeleteObject(timer);

		timer = NIL;

		return;
	}

	if (timerCount == 1)
	{
		timer->Stop();
		timer->Start(100);
	}

	for (Int n = 1; n < 10; n++)
	{
		if (timerCount >= 20 * n && *variable % (Int) Math::Pow(10, n) == 0)
		{
			plus = (Int) Math::Pow(10, n);
		}
	}

	if (arrow1Clicked)
	{
		if (subtype == OR_HORZ)	(*variable) -= plus;
		else			(*variable) += plus;
	}
	else if (arrow2Clicked)
	{
		if (subtype == OR_HORZ)	(*variable) += plus;
		else			(*variable) -= plus;
	}


	if (*variable < startValue)	*variable = startValue;
	else if (*variable > endValue)	*variable = endValue;

	if (*variable != prevValue) onClick.Emit(0, 0);

	timerCount++;
}

S::Int S::GUI::Arrows::SetRange(Int rangeStart, Int rangeEnd)
{
	Int	 prevStartValue	= startValue;
	Int	 prevEndValue	= endValue;
	Int	 prevValue	= *variable;

	startValue	= rangeStart;
	endValue	= rangeEnd;

	*variable	= (Int) (((Float) (*variable) - prevStartValue) * ((Float) (endValue - startValue) / (prevEndValue - prevStartValue)) + startValue);
	*variable	= (Int) Math::Max(rangeStart, Math::Min(rangeEnd, *variable));

	if (*variable != prevValue) onClick.Emit(0, 0);

	return Success;
}

S::Int S::GUI::Arrows::SetValue(Int newValue)
{
	if (newValue < startValue)	newValue = startValue;
	if (newValue > endValue)	newValue = endValue;

	if (*variable == newValue) return Success;

	*variable = newValue;

	onClick.Emit(0, 0);

	return Success;
}

S::Int S::GUI::Arrows::GetValue()
{
	return *variable;
}
