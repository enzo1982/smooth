 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_SCROLLBAR_
#define __OBJSMOOTH_SCROLLBAR_

#include <math.h>

#include <smooth/scrollbar.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/mathtools.h>
#include <smooth/metrics.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/timer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_SCROLLBAR = SMOOTH::RequestObjectID();

SMOOTHScrollbar::SMOOTHScrollbar(SMOOTHPoint pos, SMOOTHSize size, SMOOTHInt subType, SMOOTHInt *var, SMOOTHInt rangeStart, SMOOTHInt rangeEnd, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_SCROLLBAR;
	variable			= var;
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;
	button1Checked			= SMOOTH::False;
	button1Clicked			= SMOOTH::False;
	button2Checked			= SMOOTH::False;
	button2Clicked			= SMOOTH::False;
	button3Checked			= SMOOTH::False;
	button3Clicked			= SMOOTH::False;
	subtype				= subType;
	startValue			= rangeStart;
	endValue			= rangeEnd;
	timerActive			= SMOOTH::False;
	timer				= NIL;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = roundtoint(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = roundtoint(pos.y * SMOOTH::Setup::FontSize);

	if (subtype == OR_VERT)
	{
		if (size.cx == 0)	objectProperties->size.cx = roundtoint(18 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = roundtoint(120 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
	}
	else
	{
		if (size.cx == 0)	objectProperties->size.cx = roundtoint(120 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cx = roundtoint(size.cx * SMOOTH::Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = roundtoint(18 * SMOOTH::Setup::FontSize);
		else			objectProperties->size.cy = roundtoint(size.cy * SMOOTH::Setup::FontSize);
	}
}

SMOOTHScrollbar::~SMOOTHScrollbar()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHScrollbar::Paint(SMOOTHInt message)
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
	SMOOTHRect	 frame1;
	SMOOTHRect	 frame2;
	SMOOTHRect	 frame3;
	SMOOTHPoint	 lineStart;
	SMOOTHPoint	 lineEnd;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (subtype == OR_HORZ)
	{
		frame1.left	= frame.left;
		frame1.top	= frame.top;
		frame1.right	= frame.left + (frame.bottom - frame.top);
		frame1.bottom	= frame.bottom;

		frame2.left	= frame.right - (frame.bottom - frame.top);
		frame2.top	= frame.top;
		frame2.right	= frame.right;
		frame2.bottom	= frame.bottom;

		frame3.left	= frame.left + (frame.bottom - frame.top) + (SMOOTHInt) (((SMOOTHFloat) objectProperties->size.cx - (3 * (frame.bottom - frame.top) + 1)) / ((SMOOTHFloat) (endValue - startValue)) * ((SMOOTHFloat) (*variable - startValue)));
		frame3.top	= frame.top;
		frame3.right	= frame3.left + (frame.bottom - frame.top);
		frame3.bottom	= frame.bottom;
	}
	else
	{
		frame1.left	= frame.left;
		frame1.top	= frame.top;
		frame1.right	= frame.right;
		frame1.bottom	= frame.top + (frame.right - frame.left);

		frame2.left	= frame.left;
		frame2.top	= frame.bottom - (frame.right - frame.left);
		frame2.right	= frame.right;
		frame2.bottom	= frame.bottom;

		frame3.left	= frame.left;
		frame3.top	= frame.top + (frame.right - frame.left) + (SMOOTHInt) (((SMOOTHFloat) objectProperties->size.cy - (3 * (frame.right - frame.left) + 1)) / ((SMOOTHFloat) (endValue - startValue)) * ((SMOOTHFloat) (*variable - startValue)));
		frame3.right	= frame.right;
		frame3.bottom	= frame3.top + (frame.right - frame.left);
	}

	if (!button1Clicked && !button2Clicked) Box(dc, frame, SMOOTH::Setup::LightGrayColor, FILLED);

	if (!button3Clicked)
	{
		Box(dc, frame3, SMOOTH::Setup::BackgroundColor, FILLED);
	}
	else
	{
		frame3.bottom--;
		frame3.right--;
		frame3.left++;
		frame3.top++;
		Box(dc, frame3, SMOOTH::Setup::LightGrayColor, FILLED);
		frame3.bottom++;
		frame3.right++;
		frame3.left--;
		frame3.top--;
	}

	frame3.bottom--;
	frame3.right--;
	Frame(dc, frame3, FRAME_UP);
	frame3.bottom++;
	frame3.right++;

	if (button1Clicked || button2Clicked) return SMOOTH::Success;

	Box(dc, frame1, SMOOTH::Setup::BackgroundColor, FILLED);
	Box(dc, frame2, SMOOTH::Setup::BackgroundColor, FILLED);

	frame1.bottom--;
	frame1.right--;
	Frame(dc, frame1, FRAME_UP);
	frame1.bottom++;
	frame1.right++;
	frame2.bottom--;
	frame2.right--;
	Frame(dc, frame2, FRAME_UP);
	frame2.bottom++;
	frame2.right++;

	if (subtype == OR_HORZ)
	{
		lineStart.x = frame.left + (frame.bottom - frame.top) / 2 - 2;
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

		lineStart.x = frame.right - (frame.bottom - frame.top) / 2 - 2;
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
		lineStart.x = (frame.right + frame.left) / 2;
		lineStart.y = (frame.top + (frame.right - frame.left) / 2) - 2;
		lineEnd.x = lineStart.x + 1;
		lineEnd.y = lineStart.y;

		for (SMOOTHInt i = 0; i < 4; i++)
		{
			Line(dc, lineStart, lineEnd, SMOOTH::Setup::TextColor, PS_SOLID, 1);
			lineStart.x--;
			lineStart.y++;
			lineEnd.x++;
			lineEnd.y++;
		}

		lineStart.x = (frame.right + frame.left) / 2 - 3;
		lineStart.y = (frame.bottom - (frame.right - frame.left) / 2) - 2;
		lineEnd.x = lineStart.x + 7;
		lineEnd.y = lineStart.y;

		for (SMOOTHInt j = 0; j < 4; j++)
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

SMOOTHInt SMOOTHScrollbar::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
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
	SMOOTHRect	 frame1;
	SMOOTHRect	 frame2;
	SMOOTHRect	 frame3;
	SMOOTHRect	 actionArea;
	SMOOTHInt	 prevValue = *variable;
	SMOOTHFloat	 buffer;
	HDC		 dc;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + objectProperties->size.cx - 1;
	frame.bottom	= realPos.y + objectProperties->size.cy - 1;

	if (subtype == OR_HORZ)
	{
		frame1.left	= frame.left + 2;
		frame1.top	= frame.top + 2;
		frame1.right	= frame.left + (frame.bottom - frame.top) - 3;
		frame1.bottom	= frame.bottom - 3;

		frame2.left	= frame.right - (frame.bottom - frame.top) + 2;
		frame2.top	= frame.top + 2;
		frame2.right	= frame.right - 3;
		frame2.bottom	= frame.bottom - 3;

		frame3.left	= frame.left + (frame.bottom - frame.top) + (SMOOTHInt) (((SMOOTHFloat) objectProperties->size.cx - (3 * (frame.bottom - frame.top) + 1)) / ((SMOOTHFloat) (endValue - startValue)) * ((SMOOTHFloat) (*variable - startValue)));
		frame3.top	= frame.top;
		frame3.right	= frame3.left + (frame.bottom - frame.top);
		frame3.bottom	= frame.bottom;

		actionArea.left		= frame.left + (frame.bottom - frame.top);
		actionArea.top		= frame.top;
		actionArea.right	= frame.right - (frame.bottom - frame.top);
		actionArea.bottom	= frame.bottom;
	}
	else
	{
		frame1.left	= frame.left + 2;
		frame1.top	= frame.top + 2;
		frame1.right	= frame.right - 3;
		frame1.bottom	= frame.top + (frame.right - frame.left) - 3;

		frame2.left	= frame.left + 2;
		frame2.top	= frame.bottom - (frame.right - frame.left) + 2;
		frame2.right	= frame.right - 3;
		frame2.bottom	= frame.bottom - 3;

		frame3.left	= frame.left;
		frame3.top	= frame.top + (frame.right - frame.left) + (SMOOTHInt) (((SMOOTHFloat) objectProperties->size.cy - (3 * (frame.right - frame.left) + 1)) / ((SMOOTHFloat) (endValue - startValue)) * ((SMOOTHFloat) (*variable - startValue)));
		frame3.right	= frame.right;
		frame3.bottom	= frame3.top + (frame.right - frame.left);

		actionArea.left		= frame.left;
		actionArea.top		= frame.top + (frame.right - frame.left);
		actionArea.right	= frame.right;
		actionArea.bottom	= frame.bottom - (frame.right - frame.left);
	}

	switch (message)
	{
		case SM_LBUTTONDOWN:
			dc = GetContext(wnd);

			if (!timerActive && timer == NIL && (button1Checked || button2Checked))
			{
				timer = new SMOOTHTimer();

				wnd->RegisterObject(timer);

				timer->SetProc(SMOOTHProc(SMOOTHScrollbar, this, TimerProc));
				timer->Start(200);

				timerCount = 1;
				timerActive = SMOOTH::True;
			}

			if (button1Checked)
			{
				if (!button1Clicked) Frame(dc, frame1, FRAME_DOWN);

				button1Clicked = SMOOTH::True;

				(*variable)--;

				if (*variable < startValue)	*variable = startValue;
				else if (*variable > endValue)	*variable = endValue;

				if (*variable != prevValue)
				{
					SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

					Box(dc, frame3, SMOOTH::Setup::LightGrayColor, FILLED);

					Paint(SP_PAINT);
				}

				retVal = SMOOTH::Break;
			}
			else if (button2Checked)
			{
				if (!button2Clicked) Frame(dc, frame2, FRAME_DOWN);

				button2Clicked = SMOOTH::True;

				(*variable)++;

				if (*variable < startValue)	*variable = startValue;
				else if (*variable > endValue)	*variable = endValue;

				if (*variable != prevValue)
				{
					SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

					Box(dc, frame3, SMOOTH::Setup::LightGrayColor, FILLED);

					Paint(SP_PAINT);
				}

				retVal = SMOOTH::Break;
			}

			if (IsMouseOn(wnd->hwnd, frame3, WINDOW) && !button3Clicked)
			{
				button3Clicked = SMOOTH::True;

				frame3.left++;
				frame3.top++;
				frame3.right--;
				frame3.bottom--;
				Box(dc, frame3, SMOOTH::Setup::LightGrayColor, FILLED);
				frame3.left--;
				frame3.top--;
				frame3.right++;
				frame3.bottom++;

				if (subtype == OR_HORZ)	mouseBias = (frame3.left + (frame.bottom - frame.top) / 2) - MouseX(wnd->hwnd, WINDOW);
				else			mouseBias = (frame3.top + (frame.right - frame.left) / 2) - MouseY(wnd->hwnd, WINDOW);

				retVal = SMOOTH::Break;
			}
			else if (IsMouseOn(wnd->hwnd, actionArea, WINDOW) && !button3Clicked)
			{
				mouseBias = 0;

				button3Clicked = SMOOTH::True;
				Process(SM_MOUSEMOVE, 0, 0);
				button3Clicked = SMOOTH::False;

				Paint(SP_PAINT);

				retVal = SMOOTH::Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONDBLCLK:
			return Process(SM_LBUTTONDOWN, 0, 0);
		case SM_LBUTTONUP:
			dc = GetContext(wnd);

			if (timerActive) timerActive = SMOOTH::False;

			if (button1Clicked)
			{
				button1Clicked = SMOOTH::False;
				button1Checked = SMOOTH::False;

				frame.right++;
				frame.bottom++;
				Box(dc, frame1, SMOOTH::Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;

				Process(SM_MOUSEMOVE, 0, 0);

				retVal = SMOOTH::Break;
			}
			else if (button2Clicked)
			{
				button2Clicked = SMOOTH::False;
				button2Checked = SMOOTH::False;

				frame.right++;
				frame.bottom++;
				Box(dc, frame2, SMOOTH::Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;

				Process(SM_MOUSEMOVE, 0, 0);

				retVal = SMOOTH::Break;
			}

			if (button3Clicked)
			{
				button3Clicked = SMOOTH::False;

				frame3.left++;
				frame3.top++;
				frame3.right--;
				frame3.bottom--;
				Box(dc, frame3, SMOOTH::Setup::BackgroundColor, FILLED);
				frame3.left--;
				frame3.top--;
				frame3.right++;
				frame3.bottom++;

				retVal = SMOOTH::Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			if (!button1Checked && IsMouseOn(wnd->hwnd, frame1, WINDOW))
			{
				button1Checked = SMOOTH::True;

				Frame(dc, frame1, FRAME_UP);
			}
			else if (button1Checked && !IsMouseOn(wnd->hwnd, frame1, WINDOW))
			{
				if (timerActive) timerActive = SMOOTH::False;

				button1Checked = SMOOTH::False;
				button1Clicked = SMOOTH::False;

				frame1.right++;
				frame1.bottom++;
				Box(dc, frame1, SMOOTH::Setup::BackgroundColor, OUTLINED);
				frame1.right--;
				frame1.bottom--;
			}
			else if (!button2Checked && IsMouseOn(wnd->hwnd, frame2, WINDOW))
			{
				button2Checked = SMOOTH::True;

				Frame(dc, frame2, FRAME_UP);
			}
			else if (button2Checked && !IsMouseOn(wnd->hwnd, frame2, WINDOW))
			{
				if (timerActive) timerActive = SMOOTH::False;

				button2Checked = SMOOTH::False;
				button2Clicked = SMOOTH::False;

				frame2.right++;
				frame2.bottom++;
				Box(dc, frame2, SMOOTH::Setup::BackgroundColor, OUTLINED);
				frame2.right--;
				frame2.bottom--;
			}

			if (button3Clicked)
			{
				if (subtype == OR_HORZ)
				{
					buffer = ((SMOOTHFloat) (endValue - startValue)) / (((SMOOTHFloat) objectProperties->size.cx - (3 * (frame.bottom - frame.top) + 1)) / ((SMOOTHFloat) (MouseX(wnd->hwnd, WINDOW) + mouseBias - (realPos.x + (frame.bottom - frame.top) + (frame.bottom - frame.top) / 2))));

					*variable = startValue + roundtoint(buffer);

					if (*variable < startValue)	*variable = startValue;
					if (*variable > endValue)	*variable = endValue;
				}
				else
				{
					buffer = ((SMOOTHFloat) (endValue - startValue)) / (((SMOOTHFloat) objectProperties->size.cy - (3 * (frame.right - frame.left) + 1)) / ((SMOOTHFloat) (MouseY(wnd->hwnd, WINDOW) + mouseBias - (realPos.y + (frame.right - frame.left) + (frame.right - frame.left) / 2))));

					*variable = startValue + roundtoint(buffer);

					if (*variable < startValue)	*variable = startValue;
					if (*variable > endValue)	*variable = endValue;
				}

				if (*variable != prevValue)
				{
					Box(dc, frame3, SMOOTH::Setup::BackgroundColor, FILLED);

					Paint(SP_PAINT);

					SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);
				}
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

SMOOTHVoid SMOOTHScrollbar::TimerProc()
{
	if (!registered)		return;
	if (!active || !visible)	return;

	SMOOTHLayer	*layer = (SMOOTHLayer *) myContainer->GetContainerObject();
	SMOOTHWindow	*wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

	if (wnd == NIL) return;
	if (wnd->hwnd == NIL) return;

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
		timer->Start(10);
	}

	if (button1Clicked || button2Clicked)
	{
		Process(SM_LBUTTONDOWN, 0, 0);
	}

	timerCount++;
}

SMOOTHInt SMOOTHScrollbar::SetRange(SMOOTHInt rangeStart, SMOOTHInt rangeEnd)
{
	SMOOTHInt	 prevStartValue	= startValue;
	SMOOTHInt	 prevEndValue	= endValue;

	startValue	= rangeStart;
	endValue	= rangeEnd;

	*variable	= ((*variable) - prevStartValue) * ((endValue - startValue) / (prevEndValue - prevStartValue)) + startValue;

	SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);

	return SMOOTH::Success;
}

#endif
