 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/titlebar.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/tooltip.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/toolwindow.h>
#include <smooth/toolkit.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_TITLEBAR = S::Object::RequestObjectID();

S::GUI::Titlebar::Titlebar(Bool minButton, Bool maxButton, Bool closeButton)
{
	type				= OBJ_TITLEBAR;
	min				= minButton;
	max				= maxButton;
	close				= closeButton;
	closechk			= False;
	minchk				= False;
	maxchk				= False;
	closeclk			= False;
	minclk				= False;
	maxclk				= False;
	objectProperties->orientation	= OR_TOP;
	objectProperties->size.cy	= METRIC_TITLEBARHEIGHT;
	subtype				= WO_NOSEPARATOR;

	possibleContainers.AddEntry(OBJ_WINDOW);

#ifdef __WIN32__
	objectProperties->fontWeight	= FW_BOLD;
#endif
}

S::GUI::Titlebar::~Titlebar()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Titlebar::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Window	*wnd = (Window *) myContainer->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	EnterProtectedRegion();

	Rect	 titleFrame;
	Rect	 titleGradient;
	Rect	 titleText;
	Rect	 iconRect;
	Rect	 tButtonRect;
	Rect	 button;
	Point	 start;
	Point	 end;
	Int	 buttonColor;
	HBITMAP	 icon = wnd->icon;

	objectProperties->text = wnd->GetObjectProperties()->text;

	titleFrame.left		= objectProperties->pos.x;
	titleFrame.top		= objectProperties->pos.y;
	titleFrame.right	= titleFrame.left + objectProperties->size.cx - 1;
	titleFrame.bottom	= titleFrame.top + objectProperties->size.cy - 1;

	if (icon != NIL) titleFrame.left = titleFrame.left + METRIC_TITLEBARHEIGHT - 1;

	surface->Frame(titleFrame, FRAME_UP);

	titleGradient.left	= titleFrame.left + 1;
	titleGradient.top	= titleFrame.top + 1;
	titleGradient.right	= titleFrame.right;
	titleGradient.bottom	= titleFrame.bottom;

	Bool	 paintActive = False;

#ifdef __WIN32__
	if (GetActiveWindow() == wnd->hwnd)
	{
		paintActive = True;
	}
	else
	{
		if (Window::GetWindow(GetActiveWindow()) != NIL)
		{
			if (Window::GetWindow(GetActiveWindow())->type == OBJ_TOOLWINDOW)
			{
				paintActive = True;
			}
		}
	}

	if (paintActive)	surface->Gradient(titleGradient, Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
	else			surface->Gradient(titleGradient, Setup::InactiveGradientStartColor, Setup::InactiveGradientEndColor, OR_HORZ);
#endif

	titleText.left		= titleGradient.left + METRIC_TBTEXTOFFSETX;
	titleText.top		= titleGradient.top + METRIC_TBTEXTOFFSETY;
	titleText.right		= titleGradient.right - METRIC_TBBUTTONBOXOFFSETX + METRIC_TBBUTTONBOXWIDTH;
	titleText.bottom	= titleGradient.bottom - METRIC_TBTEXTOFFSETY;

	if (paintActive)	surface->SetText(objectProperties->text, titleText, objectProperties->font, objectProperties->fontSize, Setup::GradientTextColor, objectProperties->fontWeight);
	else			surface->SetText(objectProperties->text, titleText, objectProperties->font, objectProperties->fontSize, Setup::InactiveGradientTextColor, objectProperties->fontWeight);

	if (icon != NIL)
	{
		iconRect.left	= titleFrame.left - METRIC_TITLEBARHEIGHT + METRIC_TBICONOFFSET - 2;
		iconRect.top	= titleFrame.top + 1;
		iconRect.right	= iconRect.left + METRIC_TBICONSIZE;
		iconRect.bottom	= iconRect.top + METRIC_TBICONSIZE;

		surface->BlitFromBitmap(icon, Rect(Point(0, 0), Size(GetBitmapSizeX(icon), GetBitmapSizeY(icon))), iconRect);
	}

	tButtonRect.left	= titleGradient.right - METRIC_TBBUTTONBOXOFFSETX;
	tButtonRect.right	= tButtonRect.left + METRIC_TBBUTTONBOXWIDTH;
	tButtonRect.top		= titleGradient.top + METRIC_TBBUTTONBOXOFFSETY;
	tButtonRect.bottom	= tButtonRect.top + METRIC_TBBUTTONBOXHEIGHT;

	surface->Box(tButtonRect, Setup::BackgroundColor, FILLED);
	surface->Frame(tButtonRect, FRAME_DOWN);

	button.left	= tButtonRect.left + METRIC_TBBUTTONOFFSETX;
	button.right	= button.left + METRIC_TBBUTTONSIZE;
	button.bottom	= tButtonRect.top + METRIC_TBBUTTONOFFSETY + METRIC_TBBUTTONSIZE - 1;
	button.top	= button.bottom - 2;

	if (min)	buttonColor = Setup::TextColor;
	else		buttonColor = Setup::GrayTextColor;

	surface->Box(button, buttonColor, FILLED);

	button.left	= button.right + 3;
	button.right	= button.left + METRIC_TBBUTTONSIZE;
	button.top	= tButtonRect.top + METRIC_TBBUTTONOFFSETY;

	if (max)	buttonColor = Setup::TextColor;
	else		buttonColor = Setup::GrayTextColor;

	surface->Box(button, buttonColor, OUTLINED);
	button.top--;
	surface->Box(button, buttonColor, OUTLINED);

	button.left	= button.right + 3;
	button.right	= button.left + METRIC_TBBUTTONSIZE + 1;
	start.x		= button.left;
	start.y		= button.top;
	end.x		= button.right-1;
	end.y		= button.bottom;

	if (close)	buttonColor = Setup::TextColor;
	else		buttonColor = Setup::GrayTextColor;

	surface->Line(start, end, buttonColor);

	start.x++;
	end.x++;

	surface->Line(start, end, buttonColor);

	start.y		= button.bottom-1;
	end.y		= button.top-1;
	start.x--;
	end.x--;

	surface->Line(start, end, buttonColor);

	start.x++;
	end.x++;

	surface->Line(start, end, buttonColor);

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::Titlebar::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Point	 m;
	Point	 mPos;
	Size	 cpwp;
	Rect	 rect;
	Rect	 wRect;
	Rect	 titleFrame;
	Rect	 tButtonRect;
	Rect	 updateRect;
	Rect	 minButton;
	Rect	 maxButton;
	Rect	 closeButton;
	Rect	 workArea;
	Rect	 wndRect;
	Int	 leftButton;
	Int	 retVal = Success;

#ifdef __WIN32__
	MSG		 msg;
#endif


	titleFrame.left		= objectProperties->pos.x;
	titleFrame.top		= objectProperties->pos.y;
	titleFrame.right	= titleFrame.left + objectProperties->size.cx - 1;
	titleFrame.bottom	= titleFrame.top + objectProperties->size.cy;

	if (wnd->icon != NIL) titleFrame.left = titleFrame.left + METRIC_TITLEBARHEIGHT - 1;

	tButtonRect.left	= titleFrame.right - METRIC_TBBUTTONBOXOFFSETX;
	tButtonRect.right	= tButtonRect.left + METRIC_TBBUTTONBOXWIDTH;
	tButtonRect.top		= titleFrame.top + METRIC_TBBUTTONBOXOFFSETY + 2;
	tButtonRect.bottom	= tButtonRect.top + METRIC_TBBUTTONBOXHEIGHT;

	minButton.left		= tButtonRect.left + METRIC_TBBUTTONOFFSETX - 2;
	minButton.right		= minButton.left + METRIC_TBBUTTONSIZE + 3;
	minButton.top		= tButtonRect.top + METRIC_TBBUTTONOFFSETY - 4;
	minButton.bottom	= minButton.top + METRIC_TBBUTTONSIZE + 3;

	maxButton.left		= minButton.right;
	maxButton.right		= maxButton.left + METRIC_TBBUTTONSIZE + 3;
	maxButton.top		= tButtonRect.top + METRIC_TBBUTTONOFFSETY - 4;
	maxButton.bottom	= maxButton.top + METRIC_TBBUTTONSIZE + 3;

	closeButton.left	= maxButton.right;
	closeButton.right	= closeButton.left + METRIC_TBBUTTONSIZE + 3;
	closeButton.top		= tButtonRect.top + METRIC_TBBUTTONOFFSETY - 4;
	closeButton.bottom	= closeButton.top + METRIC_TBBUTTONSIZE + 3;

	switch (message)
	{
#ifdef __WIN32__
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		case SM_WINDOWTITLECHANGED:
			Paint(SP_PAINT);
			break;
		case SM_LBUTTONDBLCLK:
			if (max && wnd->IsMouseOn(titleFrame) && !wnd->IsMouseOn(tButtonRect))
			{
				if (wnd->maximized)
				{
					SetWindowPos(wnd->hwnd, 0, nonmaxrect.left, nonmaxrect.top, nonmaxrect.right-nonmaxrect.left, nonmaxrect.bottom-nonmaxrect.top, 0);
					wnd->maximized = False;

					if (Setup::enableUnicode)	SetWindowLongW(wnd->hwnd, GWL_STYLE, origwndstyle);
					else				SetWindowLongA(wnd->hwnd, GWL_STYLE, origwndstyle);
				}
				else
				{
					{
						RECT	 rect;

						if (Setup::enableUnicode)	SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
						else				SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0);

						workArea = rect;

						GetWindowRect(wnd->hwnd, &rect);

						nonmaxrect = rect;
					}

					SetWindowPos(wnd->hwnd, 0, workArea.left - 2, workArea.top - 2, workArea.right - workArea.left + 4, workArea.bottom - workArea.top + 4, 0);
					wnd->maximized = True;

					if (Setup::enableUnicode)	origwndstyle = GetWindowLongW(wnd->hwnd, GWL_STYLE);
					else				origwndstyle = GetWindowLongA(wnd->hwnd, GWL_STYLE);

					if (Setup::enableUnicode)	SetWindowLongW(wnd->hwnd, GWL_STYLE, (origwndstyle ^ WS_THICKFRAME) | WS_DLGFRAME);
					else				SetWindowLongA(wnd->hwnd, GWL_STYLE, (origwndstyle ^ WS_THICKFRAME) | WS_DLGFRAME);
				}

				retVal = Break;
			}
			break;
		case SM_LBUTTONDOWN:
			if (wnd->IsMouseOn(titleFrame) && !wnd->IsMouseOn(tButtonRect))
			{
				wnd->Process(SM_LOOSEFOCUS, 0, 0);

				if (GetSystemMetrics(SM_SWAPBUTTON))	leftButton = VK_RBUTTON;
				else					leftButton = VK_LBUTTON;

				{
					POINT	 mp = mPos;
					RECT	 wr = wRect;

					GetCursorPos(&mp);
					GetWindowRect(wnd->hwnd, &wr);

					mPos = mp;
					wRect = wr;
				}

				cpwp.cx = mPos.x - wRect.left;
				cpwp.cy = mPos.y - wRect.top;

				if (!wnd->maximized)
				{
					do
					{
						if (peekLoop > 0)
						{
							if (Setup::enableUnicode)	PeekMessageW(&msg, 0, 0, 0, PM_REMOVE);
							else				PeekMessageA(&msg, 0, 0, 0, PM_REMOVE);
						}
						else
						{
							if (Setup::enableUnicode)	GetMessageW(&msg, NIL, 0, 0);
							else				GetMessageA(&msg, NIL, 0, 0);
						}

						TranslateMessage(&msg);

						if (Setup::enableUnicode)	DispatchMessageW(&msg);
						else				DispatchMessageA(&msg);

						if (peekLoop > 0)
						{
							if (Setup::enableUnicode)	PostMessageW(NIL, SM_EXECUTEPEEK, 0, 0);
							else				PostMessageA(NIL, SM_EXECUTEPEEK, 0, 0);
						}

						{
							POINT	 mp = m;

							GetCursorPos(&mp);

							m = mp;
						}

						SetWindowPos(wnd->hwnd, 0, m.x-cpwp.cx, m.y-cpwp.cy, wnd->GetObjectProperties()->size.cx, wnd->GetObjectProperties()->size.cy, 0);

						{
							RECT wRect = wndRect;

							GetWindowRect(wnd->hwnd, &wRect);

							wndRect = wRect;
						}

						wnd->GetObjectProperties()->pos.x	= wndRect.left;
						wnd->GetObjectProperties()->pos.y	= wndRect.top;
						wnd->GetObjectProperties()->size.cx	= wndRect.right - wndRect.left;
						wnd->GetObjectProperties()->size.cy	= wndRect.bottom - wndRect.top;
					}
					while (GetAsyncKeyState(leftButton) != 0);
				}

				retVal = Break;
			}
			else
			{
				if (minchk || maxchk || closechk)
				{
					if (minchk)
					{
						minclk = True;
						surface->Frame(minButton, FRAME_DOWN);
					}

					if (maxchk)
					{
						maxclk = True;
						surface->Frame(maxButton, FRAME_DOWN);
					}

					if (closechk)
					{
						closeclk = True;
						surface->Frame(closeButton, FRAME_DOWN);
					}

					retVal = Break;
				}
			}
			break;
		case SM_LBUTTONUP:
			if (minclk)
			{
				minclk = False;
				ShowWindow(wnd->hwnd, SW_MINIMIZE);
			}
			if (maxclk)
			{
				maxclk = False;

				if (wnd->maximized)
				{
					SetWindowPos(wnd->hwnd, 0, nonmaxrect.left, nonmaxrect.top, nonmaxrect.right - nonmaxrect.left, nonmaxrect.bottom - nonmaxrect.top, 0);
					wnd->maximized = False;

					if (Setup::enableUnicode)	SetWindowLongW(wnd->hwnd, GWL_STYLE, origwndstyle);
					else				SetWindowLongA(wnd->hwnd, GWL_STYLE, origwndstyle);
				}
				else
				{
					{
						RECT rect;

						if (Setup::enableUnicode)	SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
						else				SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0);

						workArea = rect;

						GetWindowRect(wnd->hwnd, &rect);

						nonmaxrect = rect;
					}

					SetWindowPos(wnd->hwnd, 0, workArea.left - 2, workArea.top - 2, workArea.right - workArea.left + 4, workArea.bottom - workArea.top + 4, 0);
					wnd->maximized = True;

					if (Setup::enableUnicode)	origwndstyle = GetWindowLongW(wnd->hwnd, GWL_STYLE);
					else				origwndstyle = GetWindowLongA(wnd->hwnd, GWL_STYLE);

					if (Setup::enableUnicode)	SetWindowLongW(wnd->hwnd, GWL_STYLE, (origwndstyle ^ WS_THICKFRAME) | WS_DLGFRAME);
					else				SetWindowLongA(wnd->hwnd, GWL_STYLE, (origwndstyle ^ WS_THICKFRAME) | WS_DLGFRAME);
				}
			}
			if (closeclk)
			{
				closeclk = False;
				wnd->Close();
			}
			break;
#endif
		case SM_MOUSELEAVE:
			if (minchk && !wnd->IsMouseOn(minButton))
			{
				minchk		= False;
				minclk		= False;
				minButton.right++;
				minButton.bottom++;
				surface->Box(minButton, Setup::BackgroundColor, OUTLINED);
				minButton.right--;
				minButton.bottom--;
			}
			if (maxchk && !wnd->IsMouseOn(maxButton))
			{
				maxchk		= False;
				maxclk		= False;
				maxButton.right++;
				maxButton.bottom++;
				surface->Box(maxButton, Setup::BackgroundColor, OUTLINED);
				maxButton.right--;
				maxButton.bottom--;
			}
			if (closechk && !wnd->IsMouseOn(closeButton))
			{
				closechk	= False;
				closeclk	= False;
				closeButton.right++;
				closeButton.bottom++;
				surface->Box(closeButton, Setup::BackgroundColor, OUTLINED);
				closeButton.right--;
				closeButton.bottom--;
			}

			break;
		case SM_MOUSEMOVE:
			if (minchk && !wnd->IsMouseOn(minButton))
			{
				minchk		= False;
				minclk		= False;
				minButton.right++;
				minButton.bottom++;
				surface->Box(minButton, Setup::BackgroundColor, OUTLINED);
				minButton.right--;
				minButton.bottom--;
			}
			if (maxchk && !wnd->IsMouseOn(maxButton))
			{
				maxchk		= False;
				maxclk		= False;
				maxButton.right++;
				maxButton.bottom++;
				surface->Box(maxButton, Setup::BackgroundColor, OUTLINED);
				maxButton.right--;
				maxButton.bottom--;
			}
			if (closechk && !wnd->IsMouseOn(closeButton))
			{
				closechk	= False;
				closeclk	= False;
				closeButton.right++;
				closeButton.bottom++;
				surface->Box(closeButton, Setup::BackgroundColor, OUTLINED);
				closeButton.right--;
				closeButton.bottom--;
			}

			if (min && !minchk && wnd->IsMouseOn(minButton))
			{
				maxchk		= False;
				maxclk		= False;
				maxButton.right++;
				maxButton.bottom++;
				surface->Box(maxButton, Setup::BackgroundColor, OUTLINED);
				maxButton.right--;
				maxButton.bottom--;
				closechk	= False;
				closeclk	= False;
				closeButton.right++;
				closeButton.bottom++;
				surface->Box(closeButton, Setup::BackgroundColor, OUTLINED);
				closeButton.right--;
				closeButton.bottom--;
				minchk		= True;
				surface->Frame(minButton, FRAME_UP);
			}
			if (max && !maxchk && wnd->IsMouseOn(maxButton))
			{
				minchk		= False;
				minclk		= False;
				minButton.right++;
				minButton.bottom++;
				surface->Box(minButton, Setup::BackgroundColor, OUTLINED);
				minButton.right--;
				minButton.bottom--;
				closechk	= False;
				closeclk	= False;
				closeButton.right++;
				closeButton.bottom++;
				surface->Box(closeButton, Setup::BackgroundColor, OUTLINED);
				closeButton.right--;
				closeButton.bottom--;
				maxchk		= True;
				surface->Frame(maxButton, FRAME_UP);
			}
			if (close && !closechk && wnd->IsMouseOn(closeButton))
			{
				minchk		= False;
				minclk		= False;
				minButton.right++;
				minButton.bottom++;
				surface->Box(minButton, Setup::BackgroundColor, OUTLINED);
				minButton.right--;
				minButton.bottom--;
				maxchk		= False;
				maxclk		= False;
				maxButton.right++;
				maxButton.bottom++;
				surface->Box(maxButton, Setup::BackgroundColor, OUTLINED);
				maxButton.right--;
				maxButton.bottom--;
				closechk	= True;
				surface->Frame(closeButton, FRAME_UP);
			}

			break;
	}

	return retVal;
}
