 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/window.h>
#include <smooth/tooltip.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/toolwindow.h>
#include <smooth/timer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_TOOLTIP = S::Object::RequestObjectID();

S::Tooltip::Tooltip()
{
	type				= OBJ_TOOLTIP;
	objectProperties->orientation	= OR_FREE;
	toolWindow			= NIL;
	objectProperties->fontColor	= SMOOTH::Setup::TooltipTextColor;
	timeOut				= 5000;
	timer				= NIL;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

S::Tooltip::~Tooltip()
{
	if (timer != NIL)
	{
		timer->Stop();

		if (timer->IsRegistered() && timer->GetContainer() != NIL) timer->GetContainer()->UnregisterObject(timer);

		DeleteObject(timer);
	}

	if (toolWindow != NIL)
	{
		toolWindow->FreeOwner();

		if (toolWindow->IsRegistered() && toolWindow->GetContainer() != NIL) toolWindow->GetContainer()->UnregisterObject(toolWindow);

		DeleteObject(toolWindow);
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::Tooltip::Show()
{
	if (visible) return Success;

	visible = True;

	if (!registered) return Success;

	Window	*wnd = (Window *) myContainer->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Float	 measurement = SMOOTH::Setup::FontSize;
	Rect	 wndRect;
	HDC	 dc = GetContext(NIL);

	wndRect.left	= 0;
	wndRect.top	= 0;
	wndRect.bottom	= 16;

#ifdef __WIN32__
	wndRect.right	= GetTextSizeX(dc, objectProperties->text, I18N_DEFAULTFONT, -MulDiv(I18N_SMALLFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), FW_NORMAL) + 6;
#endif

	FreeContext(NIL, dc);

	SetMeasurement(SMT_PIXELS);

	toolWindow = new ToolWindow();

	toolWindow->SetMetrics(Point(objectProperties->pos.x + wnd->GetObjectProperties()->pos.x, objectProperties->pos.y + wnd->GetObjectProperties()->pos.y - wndRect.bottom), Size(wndRect.right, wndRect.bottom));
	toolWindow->SetOwner(this);

	wnd->RegisterObject(toolWindow);

	SMOOTH::Setup::FontSize = measurement;

	dc = GetContext(toolWindow);

	Box(dc, wndRect, SMOOTH::Setup::TooltipColor, FILLED);
	Box(dc, wndRect, RGB(0, 0, 0), OUTLINED);

	wndRect.left	+= 2;
	wndRect.top	+= 1;

	::SetText(dc, objectProperties->text, wndRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);

	FreeContext(toolWindow, dc);

	if (timeOut != 0)
	{
		timer = new Timer();

		wnd->RegisterObject(timer);

		timer->onInterval.Connect(&Tooltip::TimerProc, this);
		timer->Start(timeOut);
	}

	return Success;
}

S::Int S::Tooltip::Hide()
{
	if (!visible) return Success;

	visible = False;

	if (!registered) return Success;

	Window	*wnd = (Window *) myContainer->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	if (toolWindow != NIL)
	{
		toolWindow->FreeOwner();

		wnd->UnregisterObject(toolWindow);

		DeleteObject(toolWindow);

		toolWindow = NIL;
	}

	return Success;
}

S::Int S::Tooltip::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = (Window *) myContainer->GetContainerObject();
	Int	 retVal = Success;

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	switch (message)
	{
	}

	return retVal;
}

S::Int S::Tooltip::SetTimeout(Int mSeconds)
{
	timeOut = mSeconds;

	return Success;
}

S::Void S::Tooltip::TimerProc()
{
	Hide();

	timer->Stop();

	if (timer->IsRegistered() && timer->GetContainer() != NIL) timer->GetContainer()->UnregisterObject(timer);

	DeleteObject(timer);

	timer = NIL;
}
