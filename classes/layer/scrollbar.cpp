 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/scrollbar.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/math.h>
#include <smooth/metrics.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/timer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_SCROLLBAR = S::Object::RequestObjectID();

S::GUI::Scrollbar::Scrollbar(Point pos, Size size, Int subType, Int *var, Int rangeStart, Int rangeEnd)
{
	type		= OBJ_SCROLLBAR;
	variable	= var;
	button1Checked	= False;
	button1Clicked	= False;
	button2Checked	= False;
	button2Clicked	= False;
	button3Checked	= False;
	button3Clicked	= False;
	subtype		= subType;
	startValue	= rangeStart;
	endValue	= rangeEnd;
	timerActive	= False;
	timer		= NIL;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	if (subtype == OR_VERT)
	{
		if (size.cx == 0)	objectProperties->size.cx = Math::Round(18 * Setup::FontSize);
		else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = Math::Round(120 * Setup::FontSize);
		else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
	}
	else
	{
		if (size.cx == 0)	objectProperties->size.cx = Math::Round(120 * Setup::FontSize);
		else			objectProperties->size.cx = Math::Round(size.cx * Setup::FontSize);
		if (size.cy == 0)	objectProperties->size.cy = Math::Round(18 * Setup::FontSize);
		else			objectProperties->size.cy = Math::Round(size.cy * Setup::FontSize);
	}
}

S::GUI::Scrollbar::~Scrollbar()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Scrollbar::Paint(Int message)
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
	Rect	 frame1;
	Rect	 frame2;
	Rect	 frame3;
	Point	 lineStart;
	Point	 lineEnd;

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

		frame3.left	= frame.left + (frame.bottom - frame.top) + (Int) (((Float) objectProperties->size.cx - (3 * (frame.bottom - frame.top) + 1)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
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
		frame3.top	= frame.top + (frame.right - frame.left) + (Int) (((Float) objectProperties->size.cy - (3 * (frame.right - frame.left) + 1)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
		frame3.right	= frame.right;
		frame3.bottom	= frame3.top + (frame.right - frame.left);
	}

	if (!button1Clicked && !button2Clicked) Box(dc, frame, Setup::LightGrayColor, FILLED);

	if (!button3Clicked)
	{
		Box(dc, frame3, Setup::BackgroundColor, FILLED);
	}
	else
	{
		frame3.bottom--;
		frame3.right--;
		frame3.left++;
		frame3.top++;
		Box(dc, frame3, Setup::LightGrayColor, FILLED);
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

	if (button1Clicked || button2Clicked) return Success;

	Box(dc, frame1, Setup::BackgroundColor, FILLED);
	Box(dc, frame2, Setup::BackgroundColor, FILLED);

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

		for (Int i = 0; i < 4; i++)
		{
			Line(dc, lineStart, lineEnd, Setup::TextColor, PS_SOLID, 1);
			lineStart.x++;
			lineStart.y--;
			lineEnd.x++;
			lineEnd.y++;
		}

		lineStart.x = frame.right - (frame.bottom - frame.top) / 2 - 2;
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
		lineStart.x = (frame.right + frame.left) / 2;
		lineStart.y = (frame.top + (frame.right - frame.left) / 2) - 2;
		lineEnd.x = lineStart.x + 1;
		lineEnd.y = lineStart.y;

		for (Int i = 0; i < 4; i++)
		{
			Line(dc, lineStart, lineEnd, Setup::TextColor, PS_SOLID, 1);
			lineStart.x--;
			lineStart.y++;
			lineEnd.x++;
			lineEnd.y++;
		}

		lineStart.x = (frame.right + frame.left) / 2 - 3;
		lineStart.y = (frame.bottom - (frame.right - frame.left) / 2) - 2;
		lineEnd.x = lineStart.x + 7;
		lineEnd.y = lineStart.y;

		for (Int j = 0; j < 4; j++)
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

S::Int S::GUI::Scrollbar::Process(Int message, Int wParam, Int lParam)
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
	Rect	 frame1;
	Rect	 frame2;
	Rect	 frame3;
	Rect	 actionArea;
	Int	 prevValue = *variable;
	Float	 buffer;
	UINT	 scrolllines;
	HDC	 dc;

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

		frame3.left	= frame.left + (frame.bottom - frame.top) + (Int) (((Float) objectProperties->size.cx - (3 * (frame.bottom - frame.top) + 1)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
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
		frame3.top	= frame.top + (frame.right - frame.left) + (Int) (((Float) objectProperties->size.cy - (3 * (frame.right - frame.left) + 1)) / ((Float) (endValue - startValue)) * ((Float) (*variable - startValue)));
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
				timer = new Timer();

				timer->onInterval.Connect(&Scrollbar::TimerProc, this);
				timer->Start(200);

				timerCount = 1;
				timerActive = True;
			}

			if (button1Checked)
			{
				if (!button1Clicked) Frame(dc, frame1, FRAME_DOWN);

				button1Clicked = True;

				(*variable)--;

				if (*variable < startValue)	*variable = startValue;
				else if (*variable > endValue)	*variable = endValue;

				if (*variable != prevValue)
				{
					onClick.Emit();

					Box(dc, frame3, Setup::LightGrayColor, FILLED);

					Paint(SP_PAINT);
				}

				retVal = Break;
			}
			else if (button2Checked)
			{
				if (!button2Clicked) Frame(dc, frame2, FRAME_DOWN);

				button2Clicked = True;

				(*variable)++;

				if (*variable < startValue)	*variable = startValue;
				else if (*variable > endValue)	*variable = endValue;

				if (*variable != prevValue)
				{
					onClick.Emit();

					Box(dc, frame3, Setup::LightGrayColor, FILLED);

					Paint(SP_PAINT);
				}

				retVal = Break;
			}

			if (IsMouseOn(wnd->hwnd, frame3, WINDOW) && !button3Clicked)
			{
				button3Clicked = True;

				frame3.left++;
				frame3.top++;
				frame3.right--;
				frame3.bottom--;
				Box(dc, frame3, Setup::LightGrayColor, FILLED);
				frame3.left--;
				frame3.top--;
				frame3.right++;
				frame3.bottom++;

				if (subtype == OR_HORZ)	mouseBias = (frame3.left + (frame.bottom - frame.top) / 2) - MouseX(wnd->hwnd, WINDOW);
				else			mouseBias = (frame3.top + (frame.right - frame.left) / 2) - MouseY(wnd->hwnd, WINDOW);

				retVal = Break;
			}
			else if (IsMouseOn(wnd->hwnd, actionArea, WINDOW) && !button3Clicked)
			{
				mouseBias = 0;

				button3Clicked = True;
				Process(SM_MOUSEMOVE, 0, 0);
				button3Clicked = False;

				Paint(SP_PAINT);

				retVal = Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEWHEEL:
			SystemParametersInfo(104, NIL, &scrolllines, NIL);

			*variable -= (wParam / 120 * scrolllines);

			if (*variable < startValue)	*variable = startValue;
			if (*variable > endValue)	*variable = endValue;

			if (*variable != prevValue)
			{
				Paint(SP_PAINT);

				onClick.Emit();
			}

			retVal = Break;

			break;
		case WM_KEYDOWN:
			SystemParametersInfo(104, NIL, &scrolllines, NIL);

			switch (wParam)
			{
				case VK_PRIOR:
					*variable -= scrolllines;
					retVal = Break;
					break;
				case VK_NEXT:
					*variable += scrolllines;
					retVal = Break;
					break;
			}

			if (*variable < startValue)	*variable = startValue;
			if (*variable > endValue)	*variable = endValue;

			if (*variable != prevValue)
			{
				Paint(SP_PAINT);

				onClick.Emit();
			}

			break;
		case SM_LBUTTONDBLCLK:
			return Process(SM_LBUTTONDOWN, 0, 0);
		case SM_LBUTTONUP:
			dc = GetContext(wnd);

			if (timerActive) timerActive = False;

			if (button1Clicked)
			{
				button1Clicked = False;
				button1Checked = False;

				frame.right++;
				frame.bottom++;
				Box(dc, frame1, Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;

				Process(SM_MOUSEMOVE, 0, 0);

				retVal = Break;
			}
			else if (button2Clicked)
			{
				button2Clicked = False;
				button2Checked = False;

				frame.right++;
				frame.bottom++;
				Box(dc, frame2, Setup::BackgroundColor, OUTLINED);
				frame.right--;
				frame.bottom--;

				Process(SM_MOUSEMOVE, 0, 0);

				retVal = Break;
			}

			if (button3Clicked)
			{
				button3Clicked = False;

				frame3.left++;
				frame3.top++;
				frame3.right--;
				frame3.bottom--;
				Box(dc, frame3, Setup::BackgroundColor, FILLED);
				frame3.left--;
				frame3.top--;
				frame3.right++;
				frame3.bottom++;

				retVal = Break;
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			if (!button1Checked && IsMouseOn(wnd->hwnd, frame1, WINDOW))
			{
				button1Checked = True;

				Frame(dc, frame1, FRAME_UP);
			}
			else if (button1Checked && !IsMouseOn(wnd->hwnd, frame1, WINDOW))
			{
				if (timerActive) timerActive = False;

				button1Checked = False;
				button1Clicked = False;

				frame1.right++;
				frame1.bottom++;
				Box(dc, frame1, Setup::BackgroundColor, OUTLINED);
				frame1.right--;
				frame1.bottom--;
			}
			else if (!button2Checked && IsMouseOn(wnd->hwnd, frame2, WINDOW))
			{
				button2Checked = True;

				Frame(dc, frame2, FRAME_UP);
			}
			else if (button2Checked && !IsMouseOn(wnd->hwnd, frame2, WINDOW))
			{
				if (timerActive) timerActive = False;

				button2Checked = False;
				button2Clicked = False;

				frame2.right++;
				frame2.bottom++;
				Box(dc, frame2, Setup::BackgroundColor, OUTLINED);
				frame2.right--;
				frame2.bottom--;
			}

			if (button3Clicked)
			{
				if (subtype == OR_HORZ)	buffer = ((Float) (endValue - startValue)) / (((Float) objectProperties->size.cx - (3 * (frame.bottom - frame.top) + 1)) / ((Float) (MouseX(wnd->hwnd, WINDOW) + mouseBias - (realPos.x + (frame.bottom - frame.top) + (frame.bottom - frame.top) / 2))));
				else			buffer = ((Float) (endValue - startValue)) / (((Float) objectProperties->size.cy - (3 * (frame.right - frame.left) + 1)) / ((Float) (MouseY(wnd->hwnd, WINDOW) + mouseBias - (realPos.y + (frame.right - frame.left) + (frame.right - frame.left) / 2))));

				*variable = startValue + Math::Round(buffer);

				if (*variable < startValue)	*variable = startValue;
				if (*variable > endValue)	*variable = endValue;

				if (*variable != prevValue)
				{
					Box(dc, frame3, Setup::BackgroundColor, FILLED);

					Paint(SP_PAINT);

					onClick.Emit();
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

S::Void S::GUI::Scrollbar::TimerProc()
{
	if (!registered)		return;
	if (!active || !visible)	return;

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
		timer->Start(10);
	}

	if (button1Clicked || button2Clicked)
	{
		Process(SM_LBUTTONDOWN, 0, 0);
	}

	timerCount++;
}

S::Int S::GUI::Scrollbar::SetRange(Int rangeStart, Int rangeEnd)
{
	Int	 prevStartValue	= startValue;
	Int	 prevEndValue	= endValue;

	startValue	= rangeStart;
	endValue	= rangeEnd;

	*variable	= ((*variable) - prevStartValue) * ((endValue - startValue) / (prevEndValue - prevStartValue)) + startValue;

	onClick.Emit();

	return Success;
}
