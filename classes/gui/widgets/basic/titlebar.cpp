 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/titlebar.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/gui/widgets/special/tooltip.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/application/application.h>
#include <smooth/misc/binary.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/gui/window/window.h>
#include <smooth/basic/input.h>
#include <smooth/system/event.h>

const S::Int	 S::GUI::Titlebar::classID = S::Object::RequestClassID();

S::GUI::Titlebar::Titlebar(Int buttons)
{
	type		= classID;
	paintActive	= True;
	flags		= buttons;
	closechk	= False;
	minchk		= False;
	maxchk		= False;
	closeclk	= False;
	minclk		= False;
	maxclk		= False;
	orientation	= OR_TOP;
	size.cy		= 19;
	subtype		= WO_NOSEPARATOR;

	possibleContainers.AddEntry(Window::classID);

	font.SetWeight(FW_BOLD);
}

S::GUI::Titlebar::~Titlebar()
{
}

S::Int S::GUI::Titlebar::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface	*surface	= container->GetDrawSurface();
	Window	*wnd		= container->GetContainerWindow();

	if (wnd == NIL) return Success;

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
	Bitmap	 icon = wnd->GetIcon();

	text = wnd->GetText();

	titleFrame.left		= pos.x;
	titleFrame.top		= pos.y;
	titleFrame.right	= titleFrame.left + size.cx;
	titleFrame.bottom	= titleFrame.top + size.cy;

	if (icon != NIL) titleFrame.left += 18;

	surface->Frame(titleFrame, FRAME_UP);

	titleGradient.left	= titleFrame.left + 1;
	titleGradient.top	= titleFrame.top + 1;
	titleGradient.right	= titleFrame.right - 1;
	titleGradient.bottom	= titleFrame.bottom - 1;

	if (paintActive)	surface->Gradient(titleGradient, Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
	else			surface->Gradient(titleGradient, Setup::InactiveGradientStartColor, Setup::InactiveGradientEndColor, OR_HORZ);

	titleText.left		= titleGradient.left + 3;
	titleText.top		= titleGradient.top + 2;
	titleText.right		= titleGradient.right - 4;
	titleText.bottom	= titleGradient.bottom - 2;

	if (paintActive)	font.SetColor(Setup::GradientTextColor);
	else			font.SetColor(Setup::InactiveGradientTextColor);

	surface->SetText(text, titleText, font);

	if (icon != NIL)
	{
		iconRect.left	= titleFrame.left - 18;
		iconRect.top	= titleFrame.top + 1;
		iconRect.right	= iconRect.left + 16;
		iconRect.bottom	= iconRect.top + 16;

		surface->BlitFromBitmap(icon, Rect(Point(0, 0), icon.GetSize()), iconRect);
	}

	tButtonRect.left	= titleGradient.right - 38 + (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON) ? 0 : 20);
	tButtonRect.right	= tButtonRect.left + 35 - (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON) ? 0 : 20);
	tButtonRect.top		= titleGradient.top + 2;
	tButtonRect.bottom	= tButtonRect.top + 14;

	surface->Box(tButtonRect, Setup::BackgroundColor, FILLED);
	surface->Frame(tButtonRect, FRAME_DOWN);

	button.left	= tButtonRect.left + 4;
	button.right	= button.left + 7;
	button.bottom	= tButtonRect.top + 11;
	button.top	= button.bottom - 2;

	if (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON))
	{
		if (Binary::IsFlagSet(flags, TB_MINBUTTON))	buttonColor = Setup::TextColor;
		else						buttonColor = Setup::GrayTextColor;

		surface->Box(button, buttonColor, FILLED);

		button.left	= button.right + 3;
		button.right	= button.left + 7;
	}

	button.top	= tButtonRect.top + 5;

	if (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON))
	{
		if (Binary::IsFlagSet(flags, TB_MAXBUTTON))	buttonColor = Setup::TextColor;
		else						buttonColor = Setup::GrayTextColor;

		if (wnd->IsMaximized())
		{
			button.top--;
			button.left++;
			button.bottom -= 2;
			surface->Box(button, buttonColor, OUTLINED);
			button.top--;
			surface->Box(button, buttonColor, OUTLINED);
			button.top += 4;
			button.bottom += 3;
			button.left -= 2;
			button.right -= 2;
			surface->Box(button, Setup::BackgroundColor, FILLED);
			surface->Box(button, buttonColor, OUTLINED);
			button.top--;
			surface->Box(button, buttonColor, OUTLINED);
			button.left++;
			button.top--;
			button.bottom--;
			button.right += 2;
		}
		else
		{
			surface->Box(button, buttonColor, OUTLINED);
			button.top--;
			surface->Box(button, buttonColor, OUTLINED);
			button.top++;
		}

		button.left	= button.right + 3;
		button.right	= button.left + 7;
	}

	button.top--;
	start.x		= button.left + (Setup::rightToLeft ? 1 : 0);
	start.y		= button.top;
	end.x		= button.right + (Setup::rightToLeft ? 1 : 0);
	end.y		= button.bottom;

	if (Binary::IsFlagSet(flags, TB_CLOSEBUTTON))	buttonColor = Setup::TextColor;
	else						buttonColor = Setup::GrayTextColor;

	surface->Line(start, end, buttonColor);

	start.x++;
	end.y--;

	surface->Line(start, end, buttonColor);

	start.x--;
	start.y++;
	end.x--;
	end.y++;

	surface->Line(start, end, buttonColor);

	start.y		= button.bottom - 1;
	end.y		= button.top - 1;
	end.x++;

	surface->Line(start, end, buttonColor);

	start.x++;
	end.y++;

	surface->Line(start, end, buttonColor);

	start.x--;
	start.y--;
	end.x--;
	end.y--;

	surface->Line(start, end, buttonColor);

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::Titlebar::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Failure;
	if (!active || !visible)	return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface	*surface = container->GetDrawSurface();
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
	Bool	 paint = False;
	Int	 retVal = Success;

	titleFrame.left		= pos.x;
	titleFrame.top		= pos.y;
	titleFrame.right	= titleFrame.left + size.cx - 1;
	titleFrame.bottom	= titleFrame.top + size.cy;

	if (wnd->GetIcon() != NIL) titleFrame.left += 18;

	tButtonRect.left	= titleFrame.right - 38;
	tButtonRect.right	= tButtonRect.left + 35;
	tButtonRect.top		= titleFrame.top + 4;
	tButtonRect.bottom	= tButtonRect.top + 14;

	if (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON))
	{
		minButton.left		= tButtonRect.left + 2;
		minButton.right		= minButton.left + 11;
		minButton.top		= tButtonRect.top + 1;
		minButton.bottom	= minButton.top + 11;

		maxButton.left		= tButtonRect.left + 12;
		maxButton.right		= maxButton.left + 11;
		maxButton.top		= tButtonRect.top + 1;
		maxButton.bottom	= maxButton.top + 11;
	}

	closeButton.left	= tButtonRect.left + 22;
	closeButton.right	= closeButton.left + 11;
	closeButton.top		= tButtonRect.top + 1;
	closeButton.bottom	= closeButton.top + 11;

	switch (message)
	{
#ifdef __WIN32__
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		case SM_WINDOWTITLECHANGED:
			if (GetActiveWindow() == (HWND) wnd->GetSystemWindow() && (!paintActive || message == SM_WINDOWTITLECHANGED))
			{
				paintActive	= True;
				paint		= True;
			}
			else if (GetActiveWindow() != (HWND) wnd->GetSystemWindow())
			{
				if (Window::GetWindow(GetActiveWindow()) != NIL)
				{
					if (Window::GetWindow(GetActiveWindow())->GetObjectType() == ToolWindow::classID)
					{
						Window	*rWnd = Window::GetWindow(GetActiveWindow());

						while (True)
						{
							if (rWnd->GetContainer()->GetObjectType() != Application::classID)	rWnd = rWnd->GetContainer()->GetContainerWindow();
							else									break;
						}

						if (rWnd == wnd && !paintActive)
						{
							paintActive	= True;
							paint		= True;
						}
						else if (rWnd != wnd && paintActive)
						{
							paintActive	= False;
							paint		= True;
						}
					}
					else if (paintActive)
					{
						paintActive	= False;
						paint		= True;
					}
				}
				else if (paintActive)
				{
					paintActive	= False;
					paint		= True;
				}
			}

			if (paint) Paint(SP_PAINT);

			break;
#endif
		case SM_LBUTTONDBLCLK:
			if (Binary::IsFlagSet(flags, TB_MAXBUTTON) && wnd->IsMouseOn(titleFrame) && !wnd->IsMouseOn(tButtonRect))
			{
				if (wnd->IsMaximized())	wnd->Restore();
				else			wnd->Maximize();

				retVal = Break;
			}
			break;
		case SM_LBUTTONDOWN:
			if (wnd->IsMouseOn(titleFrame) && !wnd->IsMouseOn(tButtonRect))
			{
				wnd->Process(SM_LOOSEFOCUS, 0, 0);

#ifdef __WIN32__
				Int	 leftButton;

				if (GetSystemMetrics(SM_SWAPBUTTON))	leftButton = VK_RBUTTON;
				else					leftButton = VK_LBUTTON;

				wRect = wnd->GetWindowRect();

				cpwp.cx = Input::MouseX() - wRect.left;
				cpwp.cy = Input::MouseY() - wRect.top;

				if (!wnd->IsMaximized())
				{
					System::EventProcessor	*event = new System::EventProcessor();

					do
					{
						if (wnd->IsMaximized()) break;

						if (peekLoop > 0)	event->ProcessNextEvent(False);
						else			event->ProcessNextEvent(True);

						SetWindowPos((HWND) wnd->GetSystemWindow(), 0, Input::MouseX() - cpwp.cx, Input::MouseY() - cpwp.cy, wnd->size.cx, wnd->size.cy, 0);

						Rect	 wndRect = wnd->GetWindowRect();

						wnd->pos = Point(wndRect.left, wndRect.top);
						wnd->size = Size(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top);
					}
					while (GetAsyncKeyState(leftButton) != 0);

					delete event;
				}
#endif

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

				wnd->Minimize();
			}
			if (maxclk)
			{
				maxclk = False;

				if (wnd->IsMaximized())	wnd->Restore();
				else			wnd->Maximize();
			}
			if (closeclk)
			{
				closeclk = False;
				wnd->Close();
			}
			break;
		case SM_MOUSEMOVE:
			if (Binary::IsFlagSet(flags, TB_MINBUTTON) && minchk && !wnd->IsMouseOn(minButton))
			{
				minchk		= False;
				minclk		= False;

				surface->Box(minButton, Setup::BackgroundColor, OUTLINED);
			}
			if (Binary::IsFlagSet(flags, TB_MAXBUTTON) && maxchk && !wnd->IsMouseOn(maxButton))
			{
				maxchk		= False;
				maxclk		= False;

				surface->Box(maxButton, Setup::BackgroundColor, OUTLINED);
			}
			if (Binary::IsFlagSet(flags, TB_CLOSEBUTTON) && closechk && !wnd->IsMouseOn(closeButton))
			{
				closechk	= False;
				closeclk	= False;

				surface->Box(closeButton, Setup::BackgroundColor, OUTLINED);
			}

			if (Binary::IsFlagSet(flags, TB_MINBUTTON) && !minchk && wnd->IsMouseOn(minButton))
			{
				maxchk		= False;
				maxclk		= False;

				surface->Box(maxButton, Setup::BackgroundColor, OUTLINED);

				closechk	= False;
				closeclk	= False;

				surface->Box(closeButton, Setup::BackgroundColor, OUTLINED);

				minchk		= True;

				surface->Frame(minButton, FRAME_UP);
			}
			if (Binary::IsFlagSet(flags, TB_MAXBUTTON) && !maxchk && wnd->IsMouseOn(maxButton))
			{
				minchk		= False;
				minclk		= False;

				surface->Box(minButton, Setup::BackgroundColor, OUTLINED);

				closechk	= False;
				closeclk	= False;

				surface->Box(closeButton, Setup::BackgroundColor, OUTLINED);

				maxchk		= True;

				surface->Frame(maxButton, FRAME_UP);
			}
			if (Binary::IsFlagSet(flags, TB_CLOSEBUTTON) && !closechk && wnd->IsMouseOn(closeButton))
			{
				minchk		= False;
				minclk		= False;

				surface->Box(minButton, Setup::BackgroundColor, OUTLINED);

				maxchk		= False;
				maxclk		= False;

				surface->Box(maxButton, Setup::BackgroundColor, OUTLINED);

				closechk	= True;

				surface->Frame(closeButton, FRAME_UP);
			}

			break;
	}

	return retVal;
}
