 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/gui/widgets/multi/menu/popupview.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/gui/application/application.h>

const S::Int	 S::GUI::PopupMenu::classID = S::Object::RequestClassID();

S::GUI::PopupMenu::PopupMenu(Menu *menu)
{
	type		= classID;
	orientation	= OR_FREE;
	toolwnd		= NIL;
	popupView	= NIL;
	prevPopup	= NIL;
	nextPopup	= NIL;

	possibleContainers.AddEntry(Window::classID);

	realMenu = new Menu();

	for (Int i = 0; i < menu->GetNOfObjects(); i++)
	{
		MenuEntry	*entry = (MenuEntry *) menu->GetNthObject(i);
		MenuEntry	*nEntry = realMenu->AddEntry(entry->GetText(), entry->bitmap, entry->popup, entry->bVar, entry->iVar, entry->iCode, entry->GetOrientation());

		nEntry->SetTooltipText(entry->GetTooltipText());
		nEntry->SetStatusText(entry->description);

		nEntry->onClick.Connect(&entry->onClick);
 	}

	sizeset = False;

	GetSize();
}

S::GUI::PopupMenu::~PopupMenu()
{
	if (IsVisible()) Hide();

	if (popupView != NIL) DeleteObject(popupView);
	if (toolwnd != NIL) DeleteObject(toolwnd);

	if (nextPopup != NIL) DeleteObject(nextPopup);

	if (prevPopup != NIL) prevPopup->nextPopup = NIL;

	delete realMenu;

	if (registered && container != NIL) container->UnregisterObject(this);
}

S::Int S::GUI::PopupMenu::Show()
{
	if (!registered)	return Error;
	if (visible)		return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Error;

	EnterProtectedRegion();

	GetSize();

	if (pos.x + popupsize.cx >= LiSAGetDisplaySizeX() - wnd->pos.x) pos.x = LiSAGetDisplaySizeX() - wnd->pos.x - popupsize.cx - 1;
	if (pos.y + popupsize.cy >= LiSAGetDisplaySizeY() - wnd->pos.y) pos.y = LiSAGetDisplaySizeY() - wnd->pos.y - popupsize.cy - 1;

	visible = True;

	toolwnd		= new ToolWindow();
	popupView	= new PopupView(this, realMenu);

	toolwnd->SetOwner(this);
	toolwnd->SetMetrics(pos + wnd->pos, Size(popupsize.cx + 1, popupsize.cy + 1));
	toolwnd->RegisterObject(popupView);

	wnd->RegisterObject(toolwnd);

	onShow.Emit();

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::PopupMenu::Hide()
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Window	*wnd = container->GetContainerWindow();

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

	onHide.Emit();

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::PopupMenu::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	EnterProtectedRegion();

	Int	 retVal = Success;

	switch (message)
	{
#ifdef __WIN32__
		case WM_KILLFOCUS:
			if (lParam == -1) break;

			while (wnd->GetContainer()->GetObjectType() != Application::classID) wnd = wnd->GetContainer()->GetContainerWindow();

			wnd->Process(WM_KILLFOCUS, wParam, -1);

			break;
#endif
	}

	LeaveProtectedRegion();

	return retVal;
}


S::Void S::GUI::PopupMenu::GetSize()
{
	if (!sizeset)
	{
		popupsize.cx = GetSizeX();
		popupsize.cy = GetSizeY();

		sizeset = True;
	}
}

S::Int S::GUI::PopupMenu::GetSizeX()
{
	Int	 mSize = 50;
	Int	 greatest = 0;

	if (realMenu->GetNOfObjects() == 0) return mSize;

	for (Int i = 0; i < realMenu->GetNOfObjects(); i++)
	{
		MenuEntry	*entry = (MenuEntry *) realMenu->GetNthObject(i);

		if (entry->textSize.cx > greatest)
		{
			mSize		= 50 + entry->textSize.cx;
			greatest	= entry->textSize.cx;
		}
	}

	return mSize;
}

S::Int S::GUI::PopupMenu::GetSizeY()
{
	Int	 mSize = 4;

	if (realMenu->GetNOfObjects() == 0) return mSize;

	for (Int i = 0; i < realMenu->GetNOfObjects(); i++)
	{
		MenuEntry	*entry = (MenuEntry *) realMenu->GetNthObject(i);

		if (entry->type == SM_SEPARATOR)	mSize = mSize + 5;
		else					mSize = mSize + 16;
	}

	return mSize;
}
