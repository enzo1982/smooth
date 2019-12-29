 /* The smooth Class Library
  * Copyright (C) 1998-2019 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/titlebar.h>
#include <smooth/gui/widgets/hotspot/simplebutton.h>
#include <smooth/gui/widgets/special/shortcut.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/application/application.h>
#include <smooth/misc/math.h>
#include <smooth/misc/binary.h>
#include <smooth/input/keyboard.h>
#include <smooth/system/screen.h>

#ifdef __WIN32__
#	include <smooth/backends/win32/backendwin32.h>
#endif

const S::Short	 S::GUI::Titlebar::classID = S::Object::RequestClassID();

S::GUI::Titlebar::Titlebar(Int buttons) : Widget(Point(), Size(0, 19))
{
	type		= classID;
	paintActive	= True;
	flags		= buttons;
	orientation	= OR_TOP;
	subtype		= WO_NOSEPARATOR;

	font.SetWeight(Font::Bold);

#ifdef __WIN32__
	static Bool	 flatStyle = Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 6, 2);

	if (flatStyle)
	{
		minHotspot   = new Hotspot(Point(39, 4), Size(11, 11));
		maxHotspot   = new Hotspot(Point(28, 4), Size(11, 11));
		closeHotspot = new Hotspot(Point(17, 4), Size(11, 11));
	}
	else
	{
		minHotspot   = new HotspotSimpleButton(Point(37, 5), Size(11, 11));
		maxHotspot   = new HotspotSimpleButton(Point(27, 5), Size(11, 11));
		closeHotspot = new HotspotSimpleButton(Point(17, 5), Size(11, 11));
	}

	minHotspot->SetOrientation(OR_UPPERRIGHT);
	maxHotspot->SetOrientation(OR_UPPERRIGHT);
	closeHotspot->SetOrientation(OR_UPPERRIGHT);

	minHotspot->hitTest.Connect(&Titlebar::ButtonHitTest, this);
	maxHotspot->hitTest.Connect(&Titlebar::ButtonHitTest, this);
	closeHotspot->hitTest.Connect(&Titlebar::ButtonHitTest, this);

	minHotspot->onLeftButtonClick.Connect(&Titlebar::OnMinButtonClick, this);
	maxHotspot->onLeftButtonClick.Connect(&Titlebar::OnMaxButtonClick, this);
	closeHotspot->onLeftButtonClick.Connect(&Titlebar::OnCloseButtonClick, this);

	if (Binary::IsFlagSet(flags, TB_MINBUTTON)) Add(minHotspot);
	if (Binary::IsFlagSet(flags, TB_MAXBUTTON)) Add(maxHotspot);
	if (Binary::IsFlagSet(flags, TB_CLOSEBUTTON)) Add(closeHotspot);

	dragHotspot	= new Hotspot(Point(0, 0), Size(4096, 19));
	dragHotspot->onMouseDragStart.Connect(&Titlebar::OnMouseDragStart, this);
	dragHotspot->hitTest.Connect(&Titlebar::DragHitTest, this);
	dragHotspot->onMouseDrag.Connect(&Titlebar::OnMouseDrag, this);

	if (Binary::IsFlagSet(flags, TB_MAXBUTTON)) dragHotspot->onLeftButtonDoubleClick.Connect(&Titlebar::OnMaxButtonClick, this);

	Add(dragHotspot);

	closeShortcut	= new Shortcut(0, Input::Keyboard::KeyEscape);
	closeShortcut->onKeyDown.Connect(&Titlebar::OnCloseButtonClick, this);

	if ( Binary::IsFlagSet(flags, TB_CLOSEBUTTON) &&
	    !Binary::IsFlagSet(flags, TB_MINBUTTON)   &&
	    !Binary::IsFlagSet(flags, TB_MAXBUTTON)) Add(closeShortcut);
#else
	SetHeight(0);
#endif
}

S::GUI::Titlebar::~Titlebar()
{
#ifdef __WIN32__
	DeleteObject(minHotspot);
	DeleteObject(maxHotspot);
	DeleteObject(closeHotspot);
	DeleteObject(dragHotspot);
	DeleteObject(closeShortcut);
#endif
}

S::Int S::GUI::Titlebar::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

#ifdef __WIN32__
	Surface		*surface	    = GetDrawSurface();
	Window		*window		    = container->GetContainerWindow();

	static Bool	 flatStyle	    = Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 6, 2);

	Rect		 titleFrame	    = Rect(GetRealPosition(), GetRealSize());
	Rect		 buttonRect	    = Rect(Point(titleFrame.right - Math::Round(4 * surface->GetSurfaceDPI() / 96.0) - Math::Round((5 + 10 * (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON) ? 3 : 1)) * surface->GetSurfaceDPI() / 96.0), titleFrame.top + (flatStyle ? 2 : 3)),
						   Size(Math::Round((5 + 10 * (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON) ? 3 : 1)) * surface->GetSurfaceDPI() / 96.0), titleFrame.GetHeight() - 5));
	Rect		 button;
	Point		 start;
	Point		 end;
	Bitmap		 icon		    = window->GetIcon();

	text = window->GetText();

	if (flatStyle && (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON))) buttonRect.left -= 2;

	Color		 titlebarStartColor = paintActive ? Setup::TitlebarStartColor : Setup::InactiveTitlebarStartColor;
	Color		 titlebarEndColor   = paintActive ? Setup::TitlebarEndColor   : Setup::InactiveTitlebarEndColor;
	Color		 titlebarTextColor  = paintActive ? Setup::TitlebarTextColor  : Setup::InactiveTitlebarTextColor;
	Color		 buttonColor;

	switch (message)
	{
		case SP_PAINT:
			surface->StartPaint(titleFrame - Point(1, 1) + Size(2, 1));

			if (flatStyle)
			{
				icon.SetBackgroundColor(titlebarStartColor);

				surface->Gradient(titleFrame - Point(1, 1) + Size(2, 1), titlebarStartColor, titlebarEndColor, OR_HORZ);
			}

			if (icon != NIL) titleFrame.left += titleFrame.GetHeight() - 1;

			if (!flatStyle)
			{
				icon.SetBackgroundColor(GetBackgroundColor());

				surface->Frame(titleFrame, FRAME_UP);
				surface->Gradient(titleFrame + Point(1, 1) - Size(2, 2), titlebarStartColor, titlebarEndColor, OR_HORZ);
			}

			font.SetColor(titlebarTextColor);

			surface->SetText(text, titleFrame + Point(4, Math::Ceil(Float(titleFrame.GetHeight() - font.GetScaledTextSizeY()) / 2) - 1 - (flatStyle ? 1 : 0)) - Size(9, 6), font);

			if (icon != NIL) surface->BlitFromBitmap(icon, Rect(Point(0, 0), icon.GetSize()), Rect(Point(titleFrame.left - 16 * surface->GetSurfaceDPI() / 96.0 - (flatStyle ? 0 : 2), titleFrame.top + (flatStyle ? 1 : 2)), Size(16, 16) * surface->GetSurfaceDPI() / 96.0));

			if (!flatStyle)
			{
				surface->Box(buttonRect, Setup::BackgroundColor, Rect::Filled);
				surface->Frame(buttonRect, FRAME_DOWN);
			}

			button = Rect(Point(buttonRect.left + Math::Round(4 * surface->GetSurfaceDPI() / 96.0), buttonRect.bottom - Math::Round(5 * surface->GetSurfaceDPI() / 96.0)), Size(Math::Round(7 * surface->GetSurfaceDPI() / 96.0), 2));

			if (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON))
			{
				if (Binary::IsFlagSet(flags, TB_MINBUTTON)) buttonColor = Setup::TextColor;
				else					    buttonColor = Setup::InactiveTextColor;

				surface->Box(button, buttonColor, Rect::Filled);

				button = button + Point(button.GetWidth() + Math::Round(2 * surface->GetSurfaceDPI() / 96.0) + (flatStyle ? 2 : 1), 0);
			}

			button.top	= buttonRect.top + Math::Round(5 * surface->GetSurfaceDPI() / 96.0);

			if (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON))
			{
				if (Binary::IsFlagSet(flags, TB_MAXBUTTON)) buttonColor = Setup::TextColor;
				else					    buttonColor = Setup::InactiveTextColor;

				if (window->IsMaximized())
				{
					surface->Box(button + Point(1, -1) - Size(1, 1), buttonColor, Rect::Outlined);
					surface->Box(button + Point(1, -2) - Size(1, 0), buttonColor, Rect::Outlined);
					surface->Box(button + Point(-1, 2) - Size(1, 1), flatStyle ? titlebarEndColor : Setup::BackgroundColor, Rect::Filled);
					surface->Box(button + Point(-1, 2) - Size(1, 1), buttonColor, Rect::Outlined);
					surface->Box(button + Point(-1, 1) - Size(1, 0), buttonColor, Rect::Outlined);
				}
				else
				{
					surface->Box(button, buttonColor, Rect::Outlined);
					surface->Box(button - Point(0, 1) + Size(0, 1), buttonColor, Rect::Outlined);
				}

				button = button + Point(button.GetWidth() + Math::Round(2 * surface->GetSurfaceDPI() / 96.0) + (flatStyle ? 2 : 1), 0);
			}

			start	= Point(button.left + (IsRightToLeft() ? 1 : 0), button.top - 1);
			end	= Point(button.right + (IsRightToLeft() ? 1 : 0), button.bottom);

			if (Binary::IsFlagSet(flags, TB_CLOSEBUTTON)) buttonColor = Setup::TextColor;
			else					      buttonColor = Setup::InactiveTextColor;

			surface->Line(start, end, buttonColor);
			surface->Line(start + Point(1, 0), end - Point(0, 1), buttonColor);
			surface->Line(start + Point(0, 1), end - Point(1, 0), buttonColor);

			start	= Point(button.left + (IsRightToLeft() ? 1 : 0), button.bottom - 1);
			end	= Point(button.right + (IsRightToLeft() ? 1 : 0), button.top - 2);

			surface->Line(start, end, buttonColor);
			surface->Line(start + Point(1, 0), end + Point(0, 1), buttonColor);
			surface->Line(start - Point(0, 1), end - Point(1, 0), buttonColor);

			surface->EndPaint();

			break;
	}
#endif

	return Success();
}

S::Int S::GUI::Titlebar::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())		 return Error();
	if (!IsActive() || !IsVisible()) return Success();

#ifdef __WIN32__
	Window	*window = container->GetContainerWindow();
#endif

	Bool	 prevPaintActive = paintActive;

	switch (message)
	{
		case SM_GETFOCUS:
		case SM_LOSEFOCUS:
		case SM_WINDOWTITLECHANGED:
#ifdef __WIN32__
			if (GetActiveWindow() == (HWND) window->GetSystemWindow() && (!paintActive || message == SM_WINDOWTITLECHANGED))
			{
				paintActive = True;
			}
			else if (GetActiveWindow() != (HWND) window->GetSystemWindow())
			{
				paintActive = False;

				Widget	*remoteParent = Window::GetWindow(GetActiveWindow());

				if (remoteParent != NIL)
				{
					if (remoteParent->GetObjectType() == ToolWindow::classID)
					{
						do
						{
							remoteParent = remoteParent->GetContainer();

							if (remoteParent == window) { paintActive = True; break; }
						}
						while (remoteParent != NIL);
					}
				}
			}
#endif
			if (message == SM_WINDOWTITLECHANGED || paintActive != prevPaintActive) Paint(SP_PAINT);

			break;
	}

	return Widget::Process(message, wParam, lParam);
}

S::Void S::GUI::Titlebar::OnMouseDragStart(const Point &mousePos)
{
	startMousePos = mousePos;
}

S::Void S::GUI::Titlebar::OnMouseDrag(const Point &mousePos)
{
	Window	*window	= container->GetContainerWindow();

	if (window->IsMaximized() && mousePos != startMousePos)
	{
		Rect	 nonMaxRect = window->GetRestoredWindowRect();
		Rect	 workArea   = System::Screen::GetActiveScreenWorkArea();

		window->Restore();
		window->SetPosition(Point(workArea.left + Math::Max(0, Math::Min(mousePos.x - nonMaxRect.GetWidth() / 2, workArea.GetWidth() - nonMaxRect.GetWidth())), workArea.top + mousePos.y - startMousePos.y));

		startMousePos.x -= window->GetX();
	}

	if (!window->IsMaximized()) window->SetPosition(window->GetPosition() - (Point((IsRightToLeft() ? window->GetWidth() - startMousePos.x : startMousePos.x), startMousePos.y) - Point((IsRightToLeft() ? window->GetWidth() - mousePos.x : mousePos.x), mousePos.y)));
}

S::Void S::GUI::Titlebar::OnMinButtonClick()
{
	Window	*window	= container->GetContainerWindow();

	window->Minimize();
}

S::Void S::GUI::Titlebar::OnMaxButtonClick()
{
	Window	*window	= container->GetContainerWindow();

	if (window->IsMaximized()) window->Restore();
	else			   window->Maximize();
}

S::Void S::GUI::Titlebar::OnCloseButtonClick()
{
	Window	*window	= container->GetContainerWindow();

	window->Close();
}

S::Bool S::GUI::Titlebar::ButtonHitTest(const Point &mousePos)
{
	Surface	*surface = GetDrawSurface();

	return (mousePos.x >= 1 && mousePos.y >= 1 && mousePos.x <= 9 * surface->GetSurfaceDPI() / 96.0 && mousePos.y <= 9 * surface->GetSurfaceDPI() / 96.0);
}

S::Bool S::GUI::Titlebar::DragHitTest(const Point &mousePos)
{
	Surface	*surface    = GetDrawSurface();
	Window	*window	    = container->GetContainerWindow();

#ifdef __WIN32__
	static Bool	 flatStyle = Backends::BackendWin32::IsWindowsVersionAtLeast(VER_PLATFORM_WIN32_NT, 6, 2);
#else
	static Bool	 flatStyle = False;
#endif

	Rect	 titleFrame = Rect(Point(0,0), GetRealSize());
	Rect	 buttonRect = Rect(Point(titleFrame.right - Math::Round(4 * surface->GetSurfaceDPI() / 96.0) - Math::Round((5 + 10 * (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON) ? 3 : 1)) * surface->GetSurfaceDPI() / 96.0) + (flatStyle ? 2 : 0), titleFrame.top + (flatStyle ? 4 : 3)),
				   Size(Math::Round((5 + 10 * (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON) ? 3 : 1)) * surface->GetSurfaceDPI() / 96.0) - (flatStyle ? 5 : 1), titleFrame.GetHeight() - (flatStyle ? 9 : 6)));

	if (!flatStyle && window->GetIcon() != NIL) titleFrame.left += titleFrame.GetHeight() - 1;

	if (flatStyle && (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON))) buttonRect.left -= 2;

	return ((mousePos.x >= titleFrame.left && mousePos.y >= titleFrame.top && mousePos.x <= titleFrame.right && mousePos.y <= titleFrame.bottom) &&
	       !(mousePos.x >= buttonRect.left && mousePos.y >= buttonRect.top && mousePos.x <= buttonRect.right && mousePos.y <= buttonRect.bottom));
}
