 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/misc/binary.h>

const S::Short	 S::GUI::Titlebar::classID = S::Object::RequestClassID();

S::GUI::Titlebar::Titlebar(Int buttons) : Widget(Point(), Size(0, 19))
{
	type		= classID;
	paintActive	= True;
	flags		= buttons;
	orientation	= OR_TOP;
	subtype		= WO_NOSEPARATOR;

	font.SetWeight(Font::Bold);

	minHotspot	= new HotspotSimpleButton(Point(37, 5), Size(11, 11));
	minHotspot->SetOrientation(OR_UPPERRIGHT);
	minHotspot->hitTest.Connect(&Titlebar::ButtonHitTest, this);
	minHotspot->onLeftButtonClick.Connect(&Titlebar::OnMinButtonClick, this);

	if (Binary::IsFlagSet(flags, TB_MINBUTTON)) Add(minHotspot);

	maxHotspot	= new HotspotSimpleButton(Point(27, 5), Size(11, 11));
	maxHotspot->SetOrientation(OR_UPPERRIGHT);
	maxHotspot->hitTest.Connect(&Titlebar::ButtonHitTest, this);
	maxHotspot->onLeftButtonClick.Connect(&Titlebar::OnMaxButtonClick, this);

	if (Binary::IsFlagSet(flags, TB_MAXBUTTON)) Add(maxHotspot);

	closeHotspot	= new HotspotSimpleButton(Point(17, 5), Size(11, 11));
	closeHotspot->SetOrientation(OR_UPPERRIGHT);
	closeHotspot->hitTest.Connect(&Titlebar::ButtonHitTest, this);
	closeHotspot->onLeftButtonClick.Connect(&Titlebar::OnCloseButtonClick, this);

	if (Binary::IsFlagSet(flags, TB_CLOSEBUTTON)) Add(closeHotspot);

	dragHotspot	= new Hotspot(Point(0, 0), Size(4096, 19));
	dragHotspot->onMouseDragStart.Connect(&Titlebar::OnMouseDragStart, this);
	dragHotspot->hitTest.Connect(&Titlebar::DragHitTest, this);
	dragHotspot->onMouseDrag.Connect(&Titlebar::OnMouseDrag, this);

	if (Binary::IsFlagSet(flags, TB_MAXBUTTON)) dragHotspot->onLeftButtonDoubleClick.Connect(&Titlebar::OnMaxButtonClick, this);

	Add(dragHotspot);

	closeShortcut	= new Shortcut(0, VK_ESCAPE);
	closeShortcut->onKeyDown.Connect(&Titlebar::OnCloseButtonClick, this);

	if (Binary::IsFlagSet(flags, TB_CLOSEBUTTON)) Add(closeShortcut);
}

S::GUI::Titlebar::~Titlebar()
{
	DeleteObject(minHotspot);
	DeleteObject(maxHotspot);
	DeleteObject(closeHotspot);
	DeleteObject(dragHotspot);
	DeleteObject(closeShortcut);
}

S::Int S::GUI::Titlebar::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface = GetDrawSurface();
	Window	*window	 = container->GetContainerWindow();

	Rect	 titleFrame	= Rect(GetPosition(), GetSize());
	Rect	 buttonRect	= Rect(Point(titleFrame.right - 39 + (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON) ? 0 : 20), titleFrame.top + 3), Size(35 - (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON) ? 0 : 20), 14));
	Rect	 button;
	Point	 start;
	Point	 end;
	Int	 buttonColor;
	Bitmap	 icon		= window->GetIcon();

	if (icon != NIL) titleFrame.left += 18;

	text = window->GetText();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			surface->Frame(titleFrame, FRAME_UP);

			if (paintActive)	surface->Gradient(titleFrame + Point(1, 1) - Size(2, 2), Setup::GradientStartColor, Setup::GradientEndColor, OR_HORZ);
			else			surface->Gradient(titleFrame + Point(1, 1) - Size(2, 2), Setup::InactiveGradientStartColor, Setup::InactiveGradientEndColor, OR_HORZ);

			if (paintActive)	font.SetColor(Setup::GradientTextColor);
			else			font.SetColor(Setup::InactiveGradientTextColor);

			surface->SetText(text, titleFrame + Point(4, 3) - Size(9, 6), font);

			if (icon != NIL) surface->BlitFromBitmap(icon, Rect(Point(0, 0), icon.GetSize()), Rect(Point(titleFrame.left - 18, titleFrame.top + 1), Size(16, 16)));

			surface->Box(buttonRect, Setup::BackgroundColor, Rect::Filled);
			surface->Frame(buttonRect, FRAME_DOWN);

			button = Rect(Point(buttonRect.left + 4, buttonRect.top + 9), Size(7, 2));

			if (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON))
			{
				if (Binary::IsFlagSet(flags, TB_MINBUTTON))	buttonColor = Setup::TextColor;
				else						buttonColor = Setup::GrayTextColor;

				surface->Box(button, buttonColor, Rect::Filled);

				button.left	= button.right + 3;
				button.right	= button.left + 7;
			}

			button.top	= button.top - 4;

			if (Binary::IsFlagSet(flags, TB_MINBUTTON) || Binary::IsFlagSet(flags, TB_MAXBUTTON))
			{
				if (Binary::IsFlagSet(flags, TB_MAXBUTTON))	buttonColor = Setup::TextColor;
				else						buttonColor = Setup::GrayTextColor;

				if (window->IsMaximized())
				{
					surface->Box(button + Point(1, -1) - Size(1, 1), buttonColor, Rect::Outlined);
					surface->Box(button + Point(1, -2) - Size(1, 0), buttonColor, Rect::Outlined);
					surface->Box(button + Point(-1, 2) - Size(1, 1), Setup::BackgroundColor, Rect::Filled);
					surface->Box(button + Point(-1, 2) - Size(1, 1), buttonColor, Rect::Outlined);
					surface->Box(button + Point(-1, 1) - Size(1, 0), buttonColor, Rect::Outlined);
				}
				else
				{
					surface->Box(button, buttonColor, Rect::Outlined);
					surface->Box(button - Point(0, 1) + Size(0, 1), buttonColor, Rect::Outlined);
				}

				button.left	= button.right + 3;
				button.right	= button.left + 7;
			}

			start	= Point(button.left + (IsRightToLeft() ? 1 : 0), button.top - 1);
			end	= Point(button.right + (IsRightToLeft() ? 1 : 0), button.bottom);

			if (Binary::IsFlagSet(flags, TB_CLOSEBUTTON))	buttonColor = Setup::TextColor;
			else						buttonColor = Setup::GrayTextColor;

			surface->Line(start, end, buttonColor);
			surface->Line(start + Point(1, 0), end - Point(0, 1), buttonColor);
			surface->Line(start + Point(0, 1), end - Point(1, 0), buttonColor);

			start.y		= button.bottom - 1;
			end.y		= button.top - 2;

			surface->Line(start, end, buttonColor);
			surface->Line(start + Point(1, 0), end + Point(0, 1), buttonColor);
			surface->Line(start - Point(0, 1), end - Point(1, 0), buttonColor);

			break;
	}

	return Success();
}

S::Int S::GUI::Titlebar::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())			return Error();
	if (!IsActive() || !IsVisible())	return Success();

	Window	*window = container->GetContainerWindow();
	Bool	 prevPaintActive = paintActive;

	switch (message)
	{
#ifdef __WIN32__
		case WM_SETFOCUS:
		case WM_KILLFOCUS:
		case SM_WINDOWTITLECHANGED:
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

			if (message == SM_WINDOWTITLECHANGED || paintActive != prevPaintActive) Paint(SP_PAINT);

			break;
#endif
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
	return (mousePos.x >= 1 && mousePos.y >= 1 && mousePos.x <= 9 && mousePos.y <= 9);
}

S::Bool S::GUI::Titlebar::DragHitTest(const Point &mousePos)
{
	Window	*window		= container->GetContainerWindow();
	Rect	 titleFrame	= Rect(Point(0, 0), GetSize() - Size(1, 0));
	Rect	 buttonRect	= Rect(Point(titleFrame.right - 39, 3), Size(34, 13));

	if (window->GetIcon() != NIL) titleFrame.left += 18;

	return ((mousePos.x >= titleFrame.left && mousePos.y >= titleFrame.top && mousePos.x <= titleFrame.right && mousePos.y <= titleFrame.bottom) &&
	       !(mousePos.x >= buttonRect.left && mousePos.y >= buttonRect.top && mousePos.x <= buttonRect.right && mousePos.y <= buttonRect.bottom));
}
