 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/window/titlebar.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/window/tooltip.h>
#include <smooth/objectproperties.h>
#include <smooth/toolwindow.h>
#include <smooth/graphics/surface.h>
#include <smooth/application.h>
#include <smooth/misc/binary.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/window/window.h>
#include <smooth/input.h>
#include <smooth/system/event.h>

const S::Int	 S::GUI::Titlebar::classID = S::Object::RequestClassID();

S::GUI::Titlebar::Titlebar(Int buttons)
{
	type				= classID;
	paintActive			= True;
	flags				= buttons;
	closechk			= False;
	minchk				= False;
	maxchk				= False;
	closeclk			= False;
	minclk				= False;
	maxclk				= False;
	objectProperties->orientation	= OR_TOP;
	objectProperties->size.cy	= 19;
	subtype				= WO_NOSEPARATOR;

	possibleContainers.AddEntry(Window::classID);

	objectProperties->font.SetWeight(FW_BOLD);
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

	objectProperties->text = wnd->GetObjectProperties()->text;

	titleFrame.left		= objectProperties->pos.x;
	titleFrame.top		= objectProperties->pos.y;
	titleFrame.right	= titleFrame.left + objectProperties->size.cx - 1;
	titleFrame.bottom	= titleFrame.top + objectProperties->size.cy - 1;

	if (icon != NIL) titleFrame.left += 18;

	surface->Frame(titleFrame, FRAME_UP);

	titleGradient.left	= titleFrame.left + 1;
	titleGradient.top	= titleFrame.top + 1;
	titleGradient.right	= titleFrame.right;
	titleGradient.bottom	= titleFrame.bottom;

	if (paintActive)	surface->Gradient(titleGradient, Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
	else			surface->Gradient(titleGradient, Setup::InactiveGradientStartColor, Setup::InactiveGradientEndColor, OR_HORZ);

	titleText.left		= titleGradient.left + 3;
	titleText.top		= titleGradient.top + 2;
	titleText.right		= titleGradient.right - 4;
	titleText.bottom	= titleGradient.bottom - 2;

	if (paintActive)	objectProperties->font.SetColor(Setup::GradientTextColor);
	else			objectProperties->font.SetColor(Setup::InactiveGradientTextColor);

	surface->SetText(objectProperties->text, titleText, objectProperties->font);

	if (icon != NIL)
	{
		iconRect.left	= titleFrame.left - 18;
		iconRect.top	= titleFrame.top + 1;
		iconRect.right	= iconRect.left + 16;
		iconRect.bottom	= iconRect.top + 16;

		surface->BlitFromBitmap(icon, Rect(Point(0, 0), icon.GetSize()), iconRect);
	}

	tButtonRect.left	= titleGradient.right - 38 + (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON) ? 0 : 20);
	tButtonRect.right	= tButtonRect.left + 34 - (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON) ? 0 : 20);
	tButtonRect.top		= titleGradient.top + 2;
	tButtonRect.bottom	= tButtonRect.top + 13;

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
	start.x		= button.left;
	start.y		= button.top;
	end.x		= button.right;
	end.y		= button.bottom;

	if (Binary::IsFlagSet(flags, TB_CLOSEBUTTON))	buttonColor = Setup::TextColor;
	else						buttonColor = Setup::GrayTextColor;

	surface->Line(start, end, buttonColor);

	start.x++;
	end.x++;

	surface->Line(start, end, buttonColor);

	start.y		= button.bottom - 1;
	end.y		= button.top - 1;
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

	titleFrame.left		= objectProperties->pos.x;
	titleFrame.top		= objectProperties->pos.y;
	titleFrame.right	= titleFrame.left + objectProperties->size.cx - 1;
	titleFrame.bottom	= titleFrame.top + objectProperties->size.cy;

	if (wnd->GetIcon() != NIL) titleFrame.left += 18;

	tButtonRect.left	= titleFrame.right - 38;
	tButtonRect.right	= tButtonRect.left + 34;
	tButtonRect.top		= titleFrame.top + 4;
	tButtonRect.bottom	= tButtonRect.top + 13;

	if (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON))
	{
		minButton.left		= tButtonRect.left + 2;
		minButton.right		= minButton.left + 10;
		minButton.top		= tButtonRect.top + 1;
		minButton.bottom	= minButton.top + 10;

		maxButton.left		= tButtonRect.left + 12;
		maxButton.right		= maxButton.left + 10;
		maxButton.top		= tButtonRect.top + 1;
		maxButton.bottom	= maxButton.top + 10;
	}

	closeButton.left	= tButtonRect.left + 22;
	closeButton.right	= closeButton.left + 10;
	closeButton.top		= tButtonRect.top + 1;
	closeButton.bottom	= closeButton.top + 10;

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
							if (rWnd->GetContainer()->GetContainerObject()->GetObjectType() != Application::classID)	rWnd = rWnd->GetContainer()->GetContainerWindow();
							else												break;
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

						SetWindowPos((HWND) wnd->GetSystemWindow(), 0, Input::MouseX() - cpwp.cx, Input::MouseY() - cpwp.cy, wnd->GetObjectProperties()->size.cx, wnd->GetObjectProperties()->size.cy, 0);

						Rect	 wndRect = wnd->GetWindowRect();

						wnd->GetObjectProperties()->pos = Point(wndRect.left, wndRect.top);
						wnd->GetObjectProperties()->size = Size(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top);
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
				minButton.right++;
				minButton.bottom++;
				surface->Box(minButton, Setup::BackgroundColor, OUTLINED);
				minButton.right--;
				minButton.bottom--;
			}
			if (Binary::IsFlagSet(flags, TB_MAXBUTTON) && maxchk && !wnd->IsMouseOn(maxButton))
			{
				maxchk		= False;
				maxclk		= False;
				maxButton.right++;
				maxButton.bottom++;
				surface->Box(maxButton, Setup::BackgroundColor, OUTLINED);
				maxButton.right--;
				maxButton.bottom--;
			}
			if (Binary::IsFlagSet(flags, TB_CLOSEBUTTON) && closechk && !wnd->IsMouseOn(closeButton))
			{
				closechk	= False;
				closeclk	= False;
				closeButton.right++;
				closeButton.bottom++;
				surface->Box(closeButton, Setup::BackgroundColor, OUTLINED);
				closeButton.right--;
				closeButton.bottom--;
			}

			if (Binary::IsFlagSet(flags, TB_MINBUTTON) && !minchk && wnd->IsMouseOn(minButton))
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
			if (Binary::IsFlagSet(flags, TB_MAXBUTTON) && !maxchk && wnd->IsMouseOn(maxButton))
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
			if (Binary::IsFlagSet(flags, TB_CLOSEBUTTON) && !closechk && wnd->IsMouseOn(closeButton))
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
