 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_POPUPMENU_
#define __OBJSMOOTH_POPUPMENU_

#include <smooth/popupmenu.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/metrics.h>
#include <smooth/loop.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/popupview.h>
#include <smooth/toolwindow.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_POPUP = SMOOTH::RequestObjectID();
SMOOTHInt	 SMOOTHPopupMenu::status = POPUP_NORMAL;

SMOOTHPopupMenu::SMOOTHPopupMenu()
{
	type				= OBJ_POPUP;
	objectProperties->orientation	= OR_FREE;
	toolwnd				= NIL;
	popupView			= NIL;
	prevPopup			= NIL;
	nextPopup			= NIL;

	possibleContainers.AddEntry(OBJ_WINDOW);

	status = POPUP_NORMAL;
}

SMOOTHPopupMenu::~SMOOTHPopupMenu()
{
	if (visible) Hide();

	if (nextPopup != NIL) SMOOTH::DeleteObject(nextPopup);

	if (prevPopup != NIL) prevPopup->nextPopup = NIL;

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHPopupMenu::Show()
{
	if (!registered)	return SMOOTH::Error;
	if (visible)		return SMOOTH::Success;

	SMOOTHWindow		*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Error;
	if (wnd->hwnd == NIL) return SMOOTH::Error;

	EnterProtectedRegion();

	GetSize();

	if (objectProperties->pos.x + popupsize.cx >= LiSAGetDisplaySizeX() - wnd->GetObjectProperties()->pos.x) objectProperties->pos.x = LiSAGetDisplaySizeX() - wnd->GetObjectProperties()->pos.x - popupsize.cx - 1;
	if (objectProperties->pos.y + popupsize.cy >= LiSAGetDisplaySizeY() - wnd->GetObjectProperties()->pos.y) objectProperties->pos.y = LiSAGetDisplaySizeY() - wnd->GetObjectProperties()->pos.y - popupsize.cy - 1;

	visible = SMOOTH::True;

	toolwnd		= new SMOOTHToolWindow();
	popupView	= new SMOOTHPopupView(this);

	toolwnd->SetOwner(this);

	SMOOTHFloat	 oldMeasurement = SMOOTH::Setup::FontSize;

	SMOOTHSetMeasurement(SMT_PIXELS);

	toolwnd->SetMetrics(SMOOTHPoint(objectProperties->pos.x + wnd->GetObjectProperties()->pos.x, objectProperties->pos.y + wnd->GetObjectProperties()->pos.y), SMOOTHSize(popupsize.cx + 1, popupsize.cy + 1));

	SMOOTH::Setup::FontSize = oldMeasurement;

	toolwnd->RegisterObject(popupView);

	wnd->RegisterObject(toolwnd);

	LeaveProtectedRegion();

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHPopupMenu::Hide()
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Error;

	EnterProtectedRegion();

	if (nextPopup != NIL) nextPopup->Hide();

	if (toolwnd != NIL)
	{
		toolwnd->FreeOwner();

		wnd->UnregisterObject(toolwnd);

		toolwnd->UnregisterObject(popupView);

		SMOOTH::DeleteObject(popupView);
		SMOOTH::DeleteObject(toolwnd);

		popupView = NIL;
		toolwnd = NIL;
	}

	visible = SMOOTH::False;

	LeaveProtectedRegion();

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHPopupMenu::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	EnterProtectedRegion();

	SMOOTHInt	 retVal = SMOOTH::Success;

	switch (message)
	{
		case WM_ACTIVATE:
			if (LOWORD(wParam) == WA_INACTIVE && ((HWND) lParam) != wnd->hwnd)
			{
				wnd->Process(WM_KILLFOCUS, lParam, 0);
			}
			break;
	}

	LeaveProtectedRegion();

	return retVal;
}

SMOOTHVoid SMOOTHPopupMenu::MenuToPopup(SMOOTHMenu *menu)
{
	for (SMOOTHInt i = 0; i < menu->entries.GetNOfEntries(); i++)
	{
		SMOOTHMenu::Entry	*entry		= menu->entries.GetNthEntry(i);
		SMOOTHMenu::Entry	*newEntry	= AddEntry(entry->text, entry->bitmap, NULLPROC, entry->popup, entry->bVar, entry->iVar, entry->iCode, entry->orientation);

		newEntry->tooltip = entry->tooltip;
		newEntry->description = entry->description;

		newEntry->proc = entry->proc;
		newEntry->procParam = entry->procParam;
	}
}

#endif
