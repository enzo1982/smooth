 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_ARROWS_
#define __OBJSMOOTH_ARROWS_

#include <math.h>

#include <smooth/arrows.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/timer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_ARROWS = SMOOTH::RequestObjectID();

SMOOTHArrows::SMOOTHArrows(SMOOTHPoint pos, SMOOTHSize size, SMOOTHInt sType, SMOOTHInt *var, SMOOTHInt rangeStart, SMOOTHInt rangeEnd, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_ARROWS;
	subtype				= sType;

	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;

	arrow1Checked			= SMOOTH::False;
	arrow1Clicked			= SMOOTH::False;
	arrow2Checked			= SMOOTH::False;
	arrow2Clicked			= SMOOTH::False;

	startValue			= rangeStart;
	endValue			= rangeEnd;

	variable			= var;

	timerActive			= SMOOTH::False;
	timer				= NIL;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (subtype == OR_VERT)
	{
		if (size.cx == 0)	objectProperties->size.cx = roundtoint(18 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = roundtoint(24 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
	}
	else
	{
		if (size.cx == 0)	objectProperties->size.cx = roundtoint(24 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = roundtoint(18 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
	}
}

SMOOTHArrows::~SMOOTHArrows()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHArrows::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	HDC		 dc = GetContext(wnd);
	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHRect	 frame;
	SMOOTHPoint	 lineStart;
	SMOOTHPoint	 lineEnd;

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

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

		lineStart.x++;
		lineEnd.x++;

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerLightColor, PS_SOLID, 1);

		lineStart.x = (frame.left + (frame.left + frame.right) / 2) / 2 - 1;
		lineStart.y = (frame.bottom + frame.top) / 2;
		lineEnd.x = lineStart.x;
		lineEnd.y = lineStart.y + 1;

		for (SMOOTHInt i = 0; i < 4; i++)
		{
			Line(dc, lineStart, lineEnd, SMOOTH::Setup::TextColor, PS_SOLID, 1);

			lineStart.x++;
			lineStart.y--;
			lineEnd.x++;
			lineEnd.y++;
		}

		lineStart.x = (frame.right + (frame.left + frame.right) / 2) / 2 - 1;
		lineStart.y = (frame.bottom + frame.top) / 2 - 3;
		lineEnd.x = lineStart.x;
		lineEnd.y = lineStart.y + 7;

		for (SMOOTHInt j = 0; j < 4; j++)
		{
			Line(dc, lineStart, lineEnd, SMOOTH::Setup::TextColor, PS_SOLID, 1);

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

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerDarkColor, PS_SOLID, 1);

		lineStart.y++;
		lineEnd.y++;

		Line(dc, lineStart, lineEnd, SMOOTH::Setup::DividerLightColor, PS_SOLID, 1);

		lineStart.x = (frame.right + frame.left) / 2;
		lineStart.y = (frame.top + (frame.top+frame.bottom) / 2) / 2 - 1;
		lineEnd.x = lineStart.x + 1;
		lineEnd.y = lineStart.y;

		for (SMOOTHInt k = 0; k < 4; k++)
		{
			Line(dc, lineStart, lineEnd, SMOOTH::Setup::TextColor, PS_SOLID, 1);

			lineStart.x--;
			lineStart.y++;
			lineEnd.x++;
			lineEnd.y++;
		}

		lineStart.x = (frame.right + frame.left) / 2 - 3;
		lineStart.y = (frame.bottom + (frame.top + frame.bottom) / 2) / 2 - 1;
		lineEnd.x = lineStart.x + 7;
		lineEnd.y = lineStart.y;

		for (SMOOTHInt l = 0; l < 4; l++)
		{
			Line(dc, lineStart, lineEnd, SMOOTH::Setup::TextColor, PS_SOLID, 1);

			lineStart.x++;
			lineStart.y++;
			lineEnd.x--;
			lineEnd.y++;
		}
	}

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHArrows::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHPoint	 realPos = GetRealPosition();
	SMOOTHInt	 retVal = SMOOTH::Success;
	SMOOTHRect	 frame;
	SMOOTHRect	 arrow1Frame;
	SMOOTHRect	 arrow2Frame;
	HDC		 dc;
	SMOOTHInt	 prevValue = *variable;

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
				timer = new SMOOTHTimer();

				wnd->RegisterObject(timer);

				timer->SetProc(SMOOTHProc(SMOOTHArrows, this, TimerProc));
				timer->Start(250);

				timerCount = 1;
				timerActive = SMOOTH::True;
			}

			if (arrow1Checked)
			{
				arrow1Clicked = SMOOTH::True;

				if (subtype == OR_VERT)	frame.bottom = realPos.y + objectProperties->size.cy / 2 - 3;
				else			frame.right = realPos.x + objectProperties->size.cx / 2 - 3;

				Frame(dc, frame, FRAME_DOWN);

				if (subtype == OR_HORZ)	(*variable)--;
				else			(*variable)++;

				if (*variable < startValue)	*variable = startValue;
				else if (*variable > endValue)	*variable = endValue;

				if (*variable != prevValue) SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

				retVal = SMOOTH::Break;
			}
			else if (arrow2Checked)
			{
				arrow2Clicked = SMOOTH::True;

				if (subtype == OR_VERT)	frame.top = realPos.y + objectProperties->size.cy / 2 + 2;
				else			frame.left = realPos.x + objectProperties->size.cx / 2 + 2;

				Frame(dc, frame, FRAME_DOWN);

				if (subtype == OR_HORZ)	(*variable)++;
				else			(*variable)--;

				if (*variable < startValue)	*variable = startValue;
				else if (*variable > endValue)	*variable = endValue;

				if (*variable != prevValue) SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

				retVal = SMOOTH::Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONDBLCLK:
			return Process(SM_LBUTTONDOWN, 0, 0);
		case SM_LBUTTONUP:
			dc = GetContext(wnd);

			if (timerActive) timerActive = SMOOTH::False;

			if (arrow1Clicked)
			{
				arrow1Clicked = SMOOTH::False;
				arrow1Checked = SMOOTH::False;

				if (subtype == OR_VERT)	frame.bottom = realPos.y + objectProperties->size.cy / 2 - 3;
				else			frame.right = realPos.x + objectProperties->size.cx / 2 - 3;

				frame.right++;
				frame.bottom++;
				Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;

				Process(SM_MOUSEMOVE, 0, 0);

				retVal = SMOOTH::Break;
			}
			else if (arrow2Clicked)
			{
				arrow2Clicked = SMOOTH::False;
				arrow2Checked = SMOOTH::False;

				if (subtype == OR_VERT)	frame.top = realPos.y + objectProperties->size.cy / 2 + 2;
				else			frame.left = realPos.x + objectProperties->size.cx / 2 + 2;

				frame.right++;
				frame.bottom++;
				Box(dc, frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;

				Process(SM_MOUSEMOVE, 0, 0);

				retVal = SMOOTH::Break;
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
				if (timerActive) timerActive = SMOOTH::False;

				arrow1Checked = SMOOTH::False;
				arrow1Clicked = SMOOTH::False;

				arrow1Frame.right++;
				arrow1Frame.bottom++;
				Box(dc, arrow1Frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
				arrow1Frame.right--;
				arrow1Frame.bottom--;
			}
			else if (arrow2Checked && !IsMouseOn(wnd->hwnd, arrow2Frame, WINDOW))
			{
				if (timerActive) timerActive = SMOOTH::False;

				arrow2Checked = SMOOTH::False;
				arrow2Clicked = SMOOTH::False;

				arrow2Frame.right++;
				arrow2Frame.bottom++;
				Box(dc, arrow2Frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
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
				arrow1Checked = SMOOTH::True;

				Frame(dc, arrow1Frame, FRAME_UP);
			}
			else if (arrow1Checked && !IsMouseOn(wnd->hwnd, arrow1Frame, WINDOW))
			{
				if (timerActive) timerActive = SMOOTH::False;

				arrow1Checked = SMOOTH::False;
				arrow1Clicked = SMOOTH::False;

				arrow1Frame.right++;
				arrow1Frame.bottom++;
				Box(dc, arrow1Frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
				arrow1Frame.right--;
				arrow1Frame.bottom--;
			}
			else if (!arrow2Checked && IsMouseOn(wnd->hwnd, arrow2Frame, WINDOW))
			{
				arrow2Checked = SMOOTH::True;

				Frame(dc, arrow2Frame, FRAME_UP);
			}
			else if (arrow2Checked && !IsMouseOn(wnd->hwnd, arrow2Frame, WINDOW))
			{
				if (timerActive) timerActive = SMOOTH::False;

				arrow2Checked = SMOOTH::False;
				arrow2Clicked = SMOOTH::False;

				arrow2Frame.right++;
				arrow2Frame.bottom++;
				Box(dc, arrow2Frame, SMOOTH::Setup::BackgroundColor, OUTLINED);
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

SMOOTHVoid SMOOTHArrows::TimerProc()
{
	if (!registered)		return;
	if (!active || !visible)	return;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return;
	if (wnd->hwnd == NIL) return;

	SMOOTHInt	 prevValue = *variable;
	SMOOTHInt	 plus = 1;

	if (!timerActive && timer != NIL)
	{
		timer->Stop();

		wnd->UnregisterObject(timer);

		delete timer;

		timer = NIL;

		return;
	}

	if (timerCount == 1)
	{
		timer->Stop();
		timer->Start(100);
	}

	for (SMOOTHInt n = 1; n < 10; n++)
	{
		if (timerCount >= 20 * n && *variable % (SMOOTHInt) pow(10, n) == 0)
		{
			plus = (SMOOTHInt) pow(10, n);
		}
	}

	if (arrow1Clicked)
	{
		if (subtype == OR_HORZ)	(*variable) -= plus;
		else			(*variable) += plus;

		if (*variable < startValue)	*variable = startValue;
		else if (*variable > endValue)	*variable = endValue;

		if (*variable != prevValue) SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);
	}
	else if (arrow2Clicked)
	{
		if (subtype == OR_HORZ)	(*variable) += plus;
		else			(*variable) -= plus;

		if (*variable < startValue)	*variable = startValue;
		else if (*variable > endValue)	*variable = endValue;

		if (*variable != prevValue) SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);
	}

	timerCount++;
}

SMOOTHInt SMOOTHArrows::SetRange(SMOOTHInt rangeStart, SMOOTHInt rangeEnd)
{
	SMOOTHInt	 prevStartValue	= startValue;
	SMOOTHInt	 prevEndValue	= endValue;

	startValue	= rangeStart;
	endValue	= rangeEnd;

	*variable	= ((*variable) - prevStartValue) * ((endValue - startValue) / (prevEndValue - prevStartValue)) + startValue;

	SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHArrows::SetValue(SMOOTHInt newValue)
{
	if (newValue < startValue)	newValue = startValue;
	if (newValue > endValue)	newValue = endValue;

	if (*variable == newValue) return SMOOTH::Success;

	*variable = newValue;

	SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHArrows::GetValue()
{
	return *variable;
}

#endif
