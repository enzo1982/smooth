 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/special/tooltip.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/i18n.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/system/timer.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Tooltip::classID = S::Object::RequestClassID();

S::GUI::Tooltip::Tooltip()
{
	type		= classID;
	orientation	= OR_FREE;
	toolWindow	= NIL;
	timeOut		= 5000;
	timer		= NIL;

	font.SetColor(Setup::TooltipTextColor);

	possibleContainers.AddEntry(Window::classID);
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

	if (registered && container != NIL) container->UnregisterObject(this);
}

S::Int S::GUI::Tooltip::Show()
{
	if (visible) return Success;

	visible = True;

	if (!registered) return Success;

	Window	*wnd = (Window *) container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Rect	 wndRect;

	wndRect.left	= 0;
	wndRect.top	= 0;
	wndRect.bottom	= font.GetTextSizeY(text) + 4;
	wndRect.right	= font.GetTextSizeX(text) + 6;

	toolWindow = new ToolWindow();

	toolWindow->SetMetrics(Point(pos.x + wnd->pos.x, pos.y + wnd->pos.y - wndRect.bottom), Size(wndRect.right, wndRect.bottom));
	toolWindow->SetOwner(this);
	toolWindow->onPaint.Connect(&Tooltip::DrawTooltip, this);

	wnd->RegisterObject(toolWindow);

	if (timeOut != 0)
	{
		timer = new System::Timer();

		timer->onInterval.Connect(&Tooltip::TimerProc, this);
		timer->Start(timeOut);
	}

	onShow.Emit();

	return Success;
}

S::Int S::GUI::Tooltip::Hide()
{
	if (!visible) return Success;

	visible = False;

	if (!registered) return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	if (toolWindow != NIL)
	{
		toolWindow->FreeOwner();

		wnd->UnregisterObject(toolWindow);

		DeleteObject(toolWindow);

		toolWindow = NIL;
	}

	onHide.Emit();

	return Success;
}

S::Int S::GUI::Tooltip::DrawTooltip()
{
	if (!visible)		return Success;
	if (!registered)	return Success;

	Window	*wnd = (Window *) container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Rect	 wndRect;

	wndRect.left	= 0;
	wndRect.top	= 0;
	wndRect.bottom	= font.GetTextSizeY(text) + 4;
	wndRect.right	= font.GetTextSizeX(text) + 6;

	Surface	*surface = toolWindow->GetDrawSurface();

	surface->Box(wndRect, Setup::TooltipColor, FILLED);
	surface->Box(wndRect, RGB(0, 0, 0), OUTLINED);

	wndRect.left	+= 2;
	wndRect.top	+= 1;

	surface->SetText(text, wndRect, font);

	return Success;
}

S::Int S::GUI::Tooltip::SetTimeout(Int mSeconds)
{
	timeOut = mSeconds;

	return Success;
}

S::Void S::GUI::Tooltip::TimerProc()
{
	Hide();

	timer->Stop();

	DeleteObject(timer);

	timer = NIL;
}
