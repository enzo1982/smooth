 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/special/tooltip.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/system/timer.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>

const S::Int	 S::GUI::Tooltip::classID = S::Object::RequestClassID();

S::GUI::Tooltip::Tooltip() : Widget(Point(), Size())
{
	type		= classID;
	orientation	= OR_FREE;
	toolWindow	= NIL;
	timeOut		= 5000;
	timer		= NIL;

	font.SetColor(Setup::TooltipTextColor);
}

S::GUI::Tooltip::~Tooltip()
{
	if (timer != NIL)
	{
		timer->Stop();

		DeleteObject(timer);
	}

	if (toolWindow != NIL)
	{
		toolWindow->FreeOwner();

		if (toolWindow->IsRegistered() && toolWindow->GetContainer() != NIL) toolWindow->GetContainer()->UnregisterObject(toolWindow);

		DeleteObject(toolWindow);
	}
}

S::Int S::GUI::Tooltip::Show()
{
	if (IsVisible()) return Success();

	visible = True;

	if (!IsRegistered()) return Success();

	Window	*wnd = (Window *) container->GetContainerWindow();

	if (wnd == NIL) return Success();

	Rect	 wndRect = Rect(Point(0, 0), Size(font.GetTextSizeX(text) + 6, font.GetTextSizeY(text) + 4));
	Point	 tPos	 = Point(Math::Max(2, GetX() + wnd->GetX()), GetY() + wnd->GetY() - wndRect.bottom);
	Size	 tSize	 = Size(wndRect.right, wndRect.bottom);

	if (tPos.x + tSize.cx > LiSAGetDisplaySizeX() - 2 && !Setup::rightToLeft)				tPos.x = Math::Max(10, GetX() + wnd->GetX() - tSize.cx);
	if (wnd->GetWidth() - ((tPos.x - wnd->GetX()) + tSize.cx) + wnd->GetX() < 2 && Setup::rightToLeft)	tPos.x = Math::Max(10, GetX() + wnd->GetX() - tSize.cx);
	if (tPos.y < 0)												tPos.y = GetY() + wnd->GetY() + 1;

	toolWindow = new ToolWindow(tPos, tSize);
	toolWindow->SetOwner(this);
	toolWindow->onPaint.Connect(&Tooltip::OnToolWindowPaint, this);

	wnd->RegisterObject(toolWindow);

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

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success();

	if (toolWindow != NIL)
	{
		toolWindow->FreeOwner();

		wnd->UnregisterObject(toolWindow);

		DeleteObject(toolWindow);

		toolWindow = NIL;
	}

	onHide.Emit();

	return Success();
}

S::Void S::GUI::Tooltip::OnToolWindowPaint()
{
	if (!IsRegistered() || !IsVisible()) return;

	Rect	 wndRect = Rect(Point(0, 0), Size(font.GetTextSizeX(text) + 6, font.GetTextSizeY(text) + 4));
	Surface	*surface = toolWindow->GetDrawSurface();

	surface->Box(wndRect, Setup::TooltipColor, FILLED);
	surface->Box(wndRect, Color(0, 0, 0), OUTLINED);

	wndRect.left	+= 2;
	wndRect.top	+= 1;

	surface->SetText(text, wndRect, font);
}

S::Int S::GUI::Tooltip::SetTimeout(Int mSeconds)
{
	timeOut = mSeconds;

	return Success();
}

S::Void S::GUI::Tooltip::OnTimer()
{
	Hide();

	timer->Stop();

	DeleteObject(timer);

	timer = NIL;
}
