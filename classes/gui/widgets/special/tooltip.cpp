 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/special/tooltip.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/system/timer.h>
#include <smooth/system/screen.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>

const S::Short	 S::GUI::Tooltip::classID = S::Object::RequestClassID();

S::GUI::Tooltip::Tooltip() : Widget(Point(), Size())
{
	type		= classID;
	orientation	= OR_FREE;
	toolWindow	= NIL;
	timeOut		= 5000;
	timer		= NIL;

	layer		= NIL;

	font.SetColor(Setup::TooltipTextColor);
}

S::GUI::Tooltip::~Tooltip()
{
	if (timer != NIL)
	{
		timer->Stop();

		DeleteObject(timer);
	}

	if (toolWindow != NIL) DeleteObject(toolWindow);
}

S::Int S::GUI::Tooltip::Show()
{
	if (visible) return Success();

	visible = True;

	if (!IsRegistered()) return Success();

	Window	*window	     = container->GetContainerWindow();
	Surface	*surface     = GetDrawSurface();

	Rect	 vScreen     = System::Screen::GetVirtualScreenMetrics();
	Float	 scaleFactor = surface->GetSurfaceDPI() / 96.0;

	Size	 tSize	     = (layer != NIL ? layer->GetSize() : Size(font.GetUnscaledTextSizeX(text) + 6, font.GetUnscaledTextSizeY(text) + 4));
	Size	 sSize	     = tSize * scaleFactor;
	Point	 tPos	     = Point(GetX(), GetY() - sSize.cy + 1);

	if (tPos.y + window->GetY() < vScreen.top + 2) tPos.y = GetY() + vScreen.top + 2;

	if (!IsRightToLeft())
	{
		if (tPos.x + window->GetX() + sSize.cx > vScreen.right - 2) tPos.x = -window->GetX() + vScreen.right - 2 - sSize.cx;
		if (tPos.x + window->GetX()	       < vScreen.left  + 2) tPos.x = -window->GetX() + vScreen.left + 2;
	}
	else
	{
		if (window->GetWidth() - tPos.x + window->GetX()	    > vScreen.right - 2) tPos.x = -vScreen.right + 2 + window->GetWidth() + window->GetX();
		if (window->GetWidth() - tPos.x - sSize.cx + window->GetX() < vScreen.left  + 2) tPos.x = -(vScreen.left + 2) + window->GetWidth() + window->GetX() - sSize.cx;
	}

	toolWindow = new ToolWindow(tPos, tSize);
	toolWindow->onPaint.Connect(&Tooltip::OnToolWindowPaint, this);
	toolWindow->onEvent.Connect(&Tooltip::OnToolWindowEvent, this);
	toolWindow->SetBackgroundColor(Setup::TooltipColor);

	if (layer != NIL) toolWindow->Add(layer);

	Add(toolWindow);

	if (timeOut != 0)
	{
		timer = new System::Timer();

		timer->onInterval.Connect(&Tooltip::OnTimer, this);
		timer->Start(timeOut);
	}

	onShow.Emit();

	return Success();
}

S::Int S::GUI::Tooltip::Hide()
{
	if (!visible) return Success();

	visible = False;

	if (!IsRegistered()) return Success();

	if (toolWindow != NIL)
	{
		Remove(toolWindow);

		if (layer != NIL) toolWindow->Remove(layer);

		toolWindow->onPaint.Disconnect(&Tooltip::OnToolWindowPaint, this);
		toolWindow->onEvent.Disconnect(&Tooltip::OnToolWindowEvent, this);

		DeleteObject(toolWindow);

		toolWindow = NIL;
	}

	onHide.Emit();

	return Success();
}

S::Void S::GUI::Tooltip::OnToolWindowPaint()
{
	Surface	*surface = toolWindow->GetDrawSurface();
	Rect	 tRect	 = Rect(Point(0, 0), surface->GetSize());

	surface->Box(tRect, Color(0, 0, 0), Rect::Outlined);

	surface->SetText(text, tRect + Point(2, 1), font);
}

S::Void S::GUI::Tooltip::OnToolWindowEvent(Int message, Int wParam, Int lParam)
{
	static Bool	 forwarding = False;

	if (forwarding) return;

	Window	*window	= container->GetContainerWindow();

	if (window == NIL) return;

	forwarding = True;

	EnterProtectedRegion();

	switch (message)
	{
		/* Forward mouse wheel messages posted to
		 * the tool window to our parent window.
		 */
		case SM_MOUSEWHEEL:
			window->Process(message, wParam, lParam);

			break;

		/* Pass on keyboard events as well and hide
		 * the tooltip immediately after char events.
		 */
		case SM_KEYDOWN:
		case SM_KEYUP:
#ifdef __WIN32__
			window->Process(message, wParam, lParam);
#endif
			break;
		case SM_CHAR:
#ifdef __WIN32__
			window->Process(message, wParam, lParam);
#endif
			Hide();

			break;
	}

	LeaveProtectedRegion();

	forwarding = False;
}

S::Void S::GUI::Tooltip::OnTimer()
{
	Hide();

	timer->Stop();

	DeleteObject(timer);

	timer = NIL;
}
