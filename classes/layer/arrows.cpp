 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/math.h>
#include <smooth/arrows.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/timer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_ARROWS = S::Object::RequestObjectID();

S::GUI::Arrows::Arrows(Point pos, Size size, Int sType, Int *var, Int rangeStart, Int rangeEnd)
{
	type		= OBJ_ARROWS;
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

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (subtype == OR_VERT)
	{
		if (size.cx == 0)	objectProperties->size.cx = Math::Round(18 * Setup::FontSize);
		else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = Math::Round(24 * Setup::FontSize);
		else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
	}
	else
	{
		if (size.cx == 0)	objectProperties->size.cx = Math::Round(24 * Setup::FontSize);
		else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = Math::Round(18 * Setup::FontSize);
		else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
	}
}

S::GUI::Arrows::~Arrows()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Arrows::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	HDC	 dc = GetContext(wnd);
	Point	 realPos = GetRealPosition();
	Rect	 frame;
	Point	 lineStart;
	Point	 lineEnd;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	Frame(dc, frame, FRAME_UP);

	if (subtype == OR_HORZ)
	{
		lineStart.x = (frame.left + frame.right) / 2;
		lineStart.y = frame.top + 1;
		lineEnd.x = lineStart.x;
		lineEnd.y = frame.bottom;

		Line(dc, lineStart, lineEnd, Setup::DividerDarkColor, PS_SOLID, 1);

		lineStart.x++;
		lineEnd.x++;

		Line(dc, lineStart, lineEnd, Setup::DividerLightColor, PS_SOLID, 1);

		lineStart.x = (frame.left + (frame.left + frame.right) / 2) / 2 - 1;
		lineStart.y = (frame.bottom + frame.top) / 2;
		lineEnd.x = lineStart.x;
		lineEnd.y = lineStart.y + 1;

		for (Int i = 0; i < 4; i++)
		{
			Line(dc, lineStart, lineEnd, Setup::TextColor, PS_SOLID, 1);

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
			Line(dc, lineStart, lineEnd, Setup::TextColor, PS_SOLID, 1);

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

		Line(dc, lineStart, lineEnd, Setup::DividerDarkColor, PS_SOLID, 1);

		lineStart.y++;
		lineEnd.y++;

		Line(dc, lineStart, lineEnd, Setup::DividerLightColor, PS_SOLID, 1);

		lineStart.x = (frame.right + frame.left) / 2;
		lineStart.y = (frame.top + (frame.top+frame.bottom) / 2) / 2 - 1;
		lineEnd.x = lineStart.x + 1;
		lineEnd.y = lineStart.y;

		for (Int k = 0; k < 4; k++)
		{
			Line(dc, lineStart, lineEnd, Setup::TextColor, PS_SOLID, 1);

			lineStart.x--;
			lineStart.y++;
			lineEnd.x++;
			lineEnd.y++;
		}

		lineStart.x = (frame.right + frame.left) / 2 - 3;
		lineStart.y = (frame.bottom + (frame.top + frame.bottom) / 2) / 2 - 1;
		lineEnd.x = lineStart.x + 7;
		lineEnd.y = lineStart.y;

		for (Int l = 0; l < 4; l++)
		{
			Line(dc, lineStart, lineEnd, Setup::TextColor, PS_SOLID, 1);

			lineStart.x++;
			lineStart.y++;
			lineEnd.x--;
			lineEnd.y++;
		}
	}

	FreeContext(wnd, dc);

	return Success;
}

S::Int S::GUI::Arrows::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Layer	*layer = (Layer *) myContainer->GetContainerObject();
	Window	*wnd = (Window *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Point	 realPos = GetRealPosition();
	Int	 retVal = Success;
	Rect	 frame;
	Rect	 arrow1Frame;
	Rect	 arrow2Frame;
	HDC	 dc;
	Int	 prevValue = *variable;

	frame.left	= realPos.x + 2;
	frame.top	= realPos.y + 2;
	frame.right	= realPos.x + objectProperties->size.cx - 3;
	frame.bottom	= realPos.y + objectProperties->size.cy - 3;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			dc = GetContext(wnd);

			if (!timerActive && timer == NIL && (arrow1Checked || arrow2Checked))
			{
				timer = new Timer();

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

				Frame(dc, frame, FRAME_DOWN);

				if (subtype == OR_HORZ)	(*variable)--;
				else			(*variable)++;

				if (*variable < startValue)	*variable = startValue;
				else if (*variable > endValue)	*variable = endValue;

				if (*variable != prevValue) onClick.Emit();

				retVal = Break;
			}
			else if (arrow2Checked)
			{
				arrow2Clicked = True;

				if (subtype == OR_VERT)	frame.top = realPos.y + objectProperties->size.cy / 2 + 2;
				else			frame.left = realPos.x + objectProperties->size.cx / 2 + 2;

				Frame(dc, frame, FRAME_DOWN);

				if (subtype == OR_HORZ)	(*variable)++;
				else			(*variable)--;

				if (*variable < startValue)	*variable = startValue;
				else if (*variable > endValue)	*variable = endValue;

				if (*variable != prevValue) onClick.Emit();

				retVal = Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONDBLCLK:
			return Process(SM_LBUTTONDOWN, 0, 0);
		case SM_LBUTTONUP:
			dc = GetContext(wnd);

			if (timerActive) timerActive = False;

			if (arrow1Clicked)
			{
				arrow1Clicked = False;
				arrow1Checked = False;

				if (subtype == OR_VERT)	frame.bottom = realPos.y + objectProperties->size.cy / 2 - 3;
				else			frame.right = realPos.x + objectProperties->size.cx / 2 - 3;

				frame.right++;
				frame.bottom++;
				Box(dc, frame, Setup::BackgroundColor, OUTLINED);
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
				Box(dc, frame, Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;

				Process(SM_MOUSEMOVE, 0, 0);

				retVal = Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSELEAVE:
			dc = GetContext(wnd);

			arrow1Frame = frame;
			arrow2Frame = frame;

			if (subtype == OR_VERT)	arrow1Frame.bottom = realPos.y + objectProperties->size.cy / 2 - 3;
			else			arrow1Frame.right = realPos.x + objectProperties->size.cx / 2 - 3;

			if (subtype == OR_VERT)	arrow2Frame.top = realPos.y + objectProperties->size.cy / 2 + 2;
			else			arrow2Frame.left = realPos.x + objectProperties->size.cx / 2 + 2;

			if (arrow1Checked && !IsMouseOn(wnd->hwnd, arrow1Frame, WINDOW))
			{
				if (timerActive) timerActive = False;

				arrow1Checked = False;
				arrow1Clicked = False;

				arrow1Frame.right++;
				arrow1Frame.bottom++;
				Box(dc, arrow1Frame, Setup::BackgroundColor, OUTLINED);
				arrow1Frame.right--;
				arrow1Frame.bottom--;
			}
			else if (arrow2Checked && !IsMouseOn(wnd->hwnd, arrow2Frame, WINDOW))
			{
				if (timerActive) timerActive = False;

				arrow2Checked = False;
				arrow2Clicked = False;

				arrow2Frame.right++;
				arrow2Frame.bottom++;
				Box(dc, arrow2Frame, Setup::BackgroundColor, OUTLINED);
				arrow2Frame.right--;
				arrow2Frame.bottom--;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			arrow1Frame = frame;
			arrow2Frame = frame;

			if (subtype == OR_VERT)	arrow1Frame.bottom = realPos.y + objectProperties->size.cy / 2 - 3;
			else			arrow1Frame.right = realPos.x + objectProperties->size.cx / 2 - 3;

			if (subtype == OR_VERT)	arrow2Frame.top = realPos.y + objectProperties->size.cy / 2 + 2;
			else			arrow2Frame.left = realPos.x + objectProperties->size.cx / 2 + 2;

			if (!arrow1Checked && IsMouseOn(wnd->hwnd, arrow1Frame, WINDOW))
			{
				arrow1Checked = True;

				Frame(dc, arrow1Frame, FRAME_UP);
			}
			else if (arrow1Checked && !IsMouseOn(wnd->hwnd, arrow1Frame, WINDOW))
			{
				if (timerActive) timerActive = False;

				arrow1Checked = False;
				arrow1Clicked = False;

				arrow1Frame.right++;
				arrow1Frame.bottom++;
				Box(dc, arrow1Frame, Setup::BackgroundColor, OUTLINED);
				arrow1Frame.right--;
				arrow1Frame.bottom--;
			}
			else if (!arrow2Checked && IsMouseOn(wnd->hwnd, arrow2Frame, WINDOW))
			{
				arrow2Checked = True;

				Frame(dc, arrow2Frame, FRAME_UP);
			}
			else if (arrow2Checked && !IsMouseOn(wnd->hwnd, arrow2Frame, WINDOW))
			{
				if (timerActive) timerActive = False;

				arrow2Checked = False;
				arrow2Clicked = False;

				arrow2Frame.right++;
				arrow2Frame.bottom++;
				Box(dc, arrow2Frame, Setup::BackgroundColor, OUTLINED);
				arrow2Frame.right--;
				arrow2Frame.bottom--;
			}

			FreeContext(wnd, dc);

			break;
#ifdef __WIN32__
		case WM_KILLFOCUS:
		case WM_ACTIVATEAPP:
			return Process(SM_LBUTTONUP, 0, 0);
#endif
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

		delete timer;

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

	if (*variable != prevValue) onClick.Emit();

	timerCount++;
}

S::Int S::GUI::Arrows::SetRange(Int rangeStart, Int rangeEnd)
{
	Int	 prevStartValue	= startValue;
	Int	 prevEndValue	= endValue;
	Int	 prevValue	= *variable;

	startValue	= rangeStart;
	endValue	= rangeEnd;

	*variable	= ((*variable) - prevStartValue) * ((endValue - startValue) / (prevEndValue - prevStartValue)) + startValue;

	if (*variable != prevValue) onClick.Emit();

	return Success;
}

S::Int S::GUI::Arrows::SetValue(Int newValue)
{
	if (newValue < startValue)	newValue = startValue;
	if (newValue > endValue)	newValue = endValue;

	if (*variable == newValue) return Success;

	*variable = newValue;

	onClick.Emit();

	return Success;
}

S::Int S::GUI::Arrows::GetValue()
{
	return *variable;
}
