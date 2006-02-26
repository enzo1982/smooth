 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/gui/widgets/multi/menu/popupmenuentry.h>
#include <smooth/gui/widgets/multi/menu/menubarentry.h>
#include <smooth/gui/application/application.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>

const S::Int	 S::GUI::PopupMenu::classID = S::Object::RequestClassID();

S::Signal0<S::Void>	 S::GUI::PopupMenu::internalClosePopups;

S::GUI::PopupMenu::PopupMenu()
{
	type		= classID;
	orientation	= OR_FREE;
	prevPopup	= NIL;
	nextPopup	= NIL;

	toolWindow	= NIL;
}

S::GUI::PopupMenu::~PopupMenu()
{
	if (IsVisible()) Hide();

	if (nextPopup != NIL) DeleteObject(nextPopup);

	if (prevPopup != NIL) prevPopup->nextPopup = NIL;

	if (toolWindow != NIL)
	{
		if (toolWindow->IsRegistered() && toolWindow->GetContainer() != NIL) toolWindow->GetContainer()->UnregisterObject(toolWindow);

		DeleteObject(toolWindow);
	}
}

S::GUI::MenuEntry *S::GUI::PopupMenu::AddEntry(const String &text, const Bitmap &bitmap, Menu *popupMenu, Bool *bVar, Int *iVar, Int iCode)
{
	MenuEntry	*newEntry = new PopupMenuEntry(text, bitmap, popupMenu, bVar, iVar, iCode);

	RegisterObject(newEntry);

	SetSize(Size(50, 5));

	Int	 nextYPos = 3;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		MenuEntry	*entry = (MenuEntry *) GetNthObject(i);

		SetWidth(Math::Max(GetWidth(), 6 + entry->GetWidth()));
		SetHeight(GetHeight() + 5 + (entry->GetText() != NIL ? 11 : 0));

		entry->SetPosition(Point(3, nextYPos));

		nextYPos += entry->GetHeight() + 1;
	}

	for (Int j = 0; j < GetNOfObjects(); j++)
	{
		MenuEntry	*entry = (MenuEntry *) GetNthObject(j);

		entry->SetWidth(GetWidth() - 6);
	}

	return newEntry;
}

S::Int S::GUI::PopupMenu::Show()
{
	if (IsVisible()) return Success();

	visible = True;

	if (!IsRegistered()) return Success();

	toolWindow = new ToolWindow(GetPosition(), GetSize());
	toolWindow->onPaint.Connect(&PopupMenu::OnToolWindowPaint, this);
	toolWindow->onLoseFocus.Connect(&PopupMenu::InternalClosePopups, this);

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		MenuEntry	*entry = (MenuEntry *) GetNthObject(i);

		if (entry->GetObjectType() != PopupMenuEntry::classID) continue;

		entry->SetRegisteredFlag(False);

		toolWindow->RegisterObject(entry);

		entry->Activate();
	}

	RegisterObject(toolWindow);

	internalClosePopups.Connect(&PopupMenu::InternalClosePopups, this);

	onShow.Emit();

	return Success();
}

S::Int S::GUI::PopupMenu::Hide()
{
	if (!visible) return Success();

	visible = False;

	if (!IsRegistered()) return Success();

	if (toolWindow != NIL)
	{
		internalClosePopups.Disconnect(&PopupMenu::InternalClosePopups, this);

		toolWindow->onLoseFocus.Disconnect(&PopupMenu::InternalClosePopups, this);

		UnregisterObject(toolWindow);

		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			MenuEntry	*entry = (MenuEntry *) GetNthObject(i);

			if (entry->GetObjectType() != PopupMenuEntry::classID) continue;

			toolWindow->UnregisterObject(entry);

			entry->SetRegisteredFlag(True);
			entry->SetContainer(this);

			entry->Deactivate();
		}

		DeleteObject(toolWindow);

		toolWindow = NIL;
	}

	onHide.Emit();

	return Success();
}

S::Void S::GUI::PopupMenu::InternalClosePopups()
{
	if (container == NIL) return;

	if (container->GetObjectType() == MenubarEntry::classID)	((MenubarEntry *) container)->ClosePopupMenu();
	else if (container->GetObjectType() == PopupMenuEntry::classID)	((PopupMenuEntry *) container)->ClosePopupMenu();
}

S::Void S::GUI::PopupMenu::OnToolWindowPaint()
{
	Surface	*surface = toolWindow->GetDrawSurface();
	Rect	 frame = Rect(Point(0, 0), GetSize());

	surface->Frame(frame, FRAME_UP);
}
