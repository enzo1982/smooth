 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

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

S::Int	 S::OBJ_POPUP = S::Object::RequestObjectID();
S::Int	 S::GUI::PopupMenu::status = POPUP_NORMAL;

S::GUI::PopupMenu::PopupMenu(Menu *menu)
{
	type				= OBJ_POPUP;
	objectProperties->orientation	= OR_FREE;
	toolwnd				= NIL;
	popupView			= NIL;
	prevPopup			= NIL;
	nextPopup			= NIL;

	possibleContainers.AddEntry(OBJ_WINDOW);

	status = POPUP_NORMAL;

	realMenu = new Menu();

	for (Int i = 0; i < menu->GetNOfEntries(); i++)
	{
		Menu::Entry	*entry = menu->entries.GetNthEntry(i);
		Menu::Entry	*nEntry = realMenu->AddEntry(entry->text, entry->bitmap, entry->popup, entry->bVar, entry->iVar, entry->iCode, entry->orientation);

		nEntry->SetTooltip(entry->tooltip);
		nEntry->SetStatusText(entry->description);

		nEntry->onClick.Connect(&entry->onClick);
 	}
}

S::GUI::PopupMenu::~PopupMenu()
{
	if (visible) Hide();

	if (nextPopup != NIL) DeleteObject(nextPopup);

	if (prevPopup != NIL) prevPopup->nextPopup = NIL;

	delete realMenu;

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::PopupMenu::Show()
{
	if (!registered)	return Error;
	if (visible)		return Success;

	Window		*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Error;
	if (wnd->hwnd == NIL) return Error;

	EnterProtectedRegion();

	realMenu->GetSize();

	if (objectProperties->pos.x + realMenu->popupsize.cx >= LiSAGetDisplaySizeX() - wnd->GetObjectProperties()->pos.x) objectProperties->pos.x = LiSAGetDisplaySizeX() - wnd->GetObjectProperties()->pos.x - realMenu->popupsize.cx - 1;
	if (objectProperties->pos.y + realMenu->popupsize.cy >= LiSAGetDisplaySizeY() - wnd->GetObjectProperties()->pos.y) objectProperties->pos.y = LiSAGetDisplaySizeY() - wnd->GetObjectProperties()->pos.y - realMenu->popupsize.cy - 1;

	visible = True;

	toolwnd		= new ToolWindow();
	popupView	= new PopupView(this, realMenu);

	toolwnd->SetOwner(this);

	Float	 oldMeasurement = Setup::FontSize;

	SetMeasurement(SMT_PIXELS);

	toolwnd->SetMetrics(Point(objectProperties->pos.x + wnd->GetObjectProperties()->pos.x, objectProperties->pos.y + wnd->GetObjectProperties()->pos.y), Size(realMenu->popupsize.cx + 1, realMenu->popupsize.cy + 1));

	Setup::FontSize = oldMeasurement;

	toolwnd->RegisterObject(popupView);

	wnd->RegisterObject(toolwnd);

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::PopupMenu::Hide()
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Error;

	EnterProtectedRegion();

	if (nextPopup != NIL) nextPopup->Hide();

	if (toolwnd != NIL)
	{
		toolwnd->FreeOwner();
		toolwnd->Close();

		wnd->UnregisterObject(toolwnd);

		toolwnd->UnregisterObject(popupView);

		DeleteObject(popupView);
		DeleteObject(toolwnd);

		popupView = NIL;
		toolwnd = NIL;
	}

	visible = False;

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::PopupMenu::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	EnterProtectedRegion();

	Int	 retVal = Success;

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
