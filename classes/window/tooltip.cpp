 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_TOOLTIP_
#define __OBJSMOOTH_TOOLTIP_

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

SMOOTHInt	 OBJ_TOOLTIP = SMOOTH::RequestObjectID();

SMOOTHTooltip::SMOOTHTooltip()
{
	type				= OBJ_TOOLTIP;
	objectProperties->orientation	= OR_FREE;
	toolWindow			= NIL;
	objectProperties->fontColor	= SMOOTH::Setup::TooltipTextColor;
	timeOut				= 5000;
	timer				= NIL;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

SMOOTHTooltip::~SMOOTHTooltip()
{
	if (timer != NIL)
	{
		timer->Stop();

		if (timer->IsRegistered() && timer->GetContainer() != NIL) timer->GetContainer()->UnregisterObject(timer);

		SMOOTH::DeleteObject(timer);
	}

	if (toolWindow != NIL)
	{
		toolWindow->FreeOwner();

		if (toolWindow->IsRegistered() && toolWindow->GetContainer() != NIL) toolWindow->GetContainer()->UnregisterObject(toolWindow);

		SMOOTH::DeleteObject(toolWindow);
	}

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHTooltip::Show()
{
	if (visible) return SMOOTH::Success;

	visible = SMOOTH::True;

	if (!registered) return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHFloat	 measurement = SMOOTH::Setup::FontSize;
	SMOOTHRect	 wndRect;
	HDC		 dc = GetContext(NIL);

	wndRect.left	= 0;
	wndRect.top	= 0;
	wndRect.bottom	= 16;

#ifdef __WIN32__
	wndRect.right	= GetTextSizeX(dc, objectProperties->text, I18N_DEFAULTFONT, -MulDiv(I18N_SMALLFONTSIZE, GetDeviceCaps(dc, LOGPIXELSY), 72), FW_NORMAL) + 6;
#endif

	FreeContext(NIL, dc);

	SMOOTHSetMeasurement(SMT_PIXELS);

	toolWindow = new SMOOTHToolWindow();

	toolWindow->SetMetrics(SMOOTHPoint(objectProperties->pos.x + wnd->GetObjectProperties()->pos.x, objectProperties->pos.y + wnd->GetObjectProperties()->pos.y - wndRect.bottom), SMOOTHSize(wndRect.right, wndRect.bottom));
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
		timer = new SMOOTHTimer();

		wnd->RegisterObject(timer);

		timer->SetProc(SMOOTHProc(SMOOTHTooltip, this, TimerProc));
		timer->Start(timeOut);
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHTooltip::Hide()
{
	if (!visible) return SMOOTH::Success;

	visible = SMOOTH::False;

	if (!registered) return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	if (toolWindow != NIL)
	{
		toolWindow->FreeOwner();

		wnd->UnregisterObject(toolWindow);

		SMOOTH::DeleteObject(toolWindow);

		toolWindow = NIL;
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHTooltip::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();
	SMOOTHInt	 retVal = SMOOTH::Success;

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	switch (message)
	{
	}

	return retVal;
}

SMOOTHInt SMOOTHTooltip::SetTimeout(SMOOTHInt mSeconds)
{
	timeOut = mSeconds;

	return SMOOTH::Success;
}

SMOOTHVoid SMOOTHTooltip::TimerProc()
{
	Hide();

	timer->Stop();

	if (timer->IsRegistered() && timer->GetContainer() != NIL) timer->GetContainer()->UnregisterObject(timer);

	SMOOTH::DeleteObject(timer);

	timer = NIL;
}

#endif
