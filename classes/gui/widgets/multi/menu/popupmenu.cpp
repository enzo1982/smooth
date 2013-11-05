 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/gui/widgets/multi/menu/popupmenuentry.h>
#include <smooth/gui/widgets/multi/menu/popupmenuentrycheck.h>
#include <smooth/gui/widgets/multi/menu/popupmenuentryoption.h>
#include <smooth/gui/widgets/multi/menu/menubar.h>
#include <smooth/gui/widgets/multi/menu/menubarentry.h>
#include <smooth/gui/application/application.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>

const S::Short	 S::GUI::PopupMenu::classID = S::Object::RequestClassID();

S::Signal1<S::Void, S::Int>	 S::GUI::PopupMenu::internalOnOpenPopupMenu;

S::GUI::PopupMenu::PopupMenu()
{
	type		= classID;
	orientation	= OR_FREE;

	closedByClick	= False;

	toolWindow = NIL;

	internalRequestClose.SetParentObject(this);

	internalOnOpenPopupMenu.Connect(&PopupMenu::OnOpenPopupMenu, this);
}

S::GUI::PopupMenu::~PopupMenu()
{
	internalOnOpenPopupMenu.Disconnect(&PopupMenu::OnOpenPopupMenu, this);
}

S::GUI::MenuEntry *S::GUI::PopupMenu::AddEntry(const String &text, const Bitmap &bitmap, PopupMenu *popupMenu, Bool *bVar, Int *iVar, Int iCode)
{
	PopupMenuEntry	*newEntry = NIL;

	if	(bVar != NIL) newEntry = new PopupMenuEntryCheck(text, bVar);
	else if (iVar != NIL) newEntry = new PopupMenuEntryOption(text, iVar, iCode);
	else		      newEntry = new PopupMenuEntry(text, bitmap, popupMenu);

	newEntry->SetOwner(this);

	Add(newEntry);

	return newEntry;
}

S::Int S::GUI::PopupMenu::Show()
{
	if (IsVisible()) return Success();

	visible = True;

	if (!IsRegistered()) return Success();

	SetFlags(GetFlags() & ~MB_POPUPOPEN);

	closedByClick = False;

	/* Create tool window, add entries and display it.
	 */
	toolWindow = new ToolWindow(GetPosition(), GetSize());
	toolWindow->onPaint.Connect(&PopupMenu::OnToolWindowPaint, this);
	toolWindow->onLoseFocus.Connect(&internalRequestClose);

	for (Int k = 0; k < GetNOfObjects(); k++)
	{
		PopupMenuEntry	*entry = (PopupMenuEntry *) GetNthObject(k);

		if (entry->GetObjectType() != PopupMenuEntry::classID) continue;

		entry->SetRegisteredFlag(False);

		toolWindow->Add(entry);
	}

	Add(toolWindow);

	internalOnOpenPopupMenu.Emit(GetHandle());

	toolWindow->Show();

	/* Notify subscribers of popup open.
	 */
	onShow.Emit();

	return Success();
}

S::Int S::GUI::PopupMenu::Hide()
{
	if (!visible) return Success();

	visible = False;

	if (!IsRegistered()) return Success();

	/* Hide tool window, remove entries and delete it.
	 */
	toolWindow->Hide();

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		MenuEntry	*entry = (MenuEntry *) GetNthObject(i);

		if (entry->GetObjectType() != PopupMenuEntry::classID) continue;

		toolWindow->Remove(entry);

		entry->SetRegisteredFlag(True);
		entry->SetContainer(this);
	}

	DeleteObject(toolWindow);

	toolWindow = NIL;

	/* Notify subscribers of popup close.
	 */
	onHide.Emit();

	return Success();
}

S::Void S::GUI::PopupMenu::CalculateSize()
{
	SetSize(Size(50, 5));

	Int	 nextYPos		= 3;
	Int	 maxShortcutTextSize	= 0;

	for (Int l = 0; l < GetNOfObjects(); l++)
	{
		PopupMenuEntry	*entry = (PopupMenuEntry *) GetNthObject(l);

		if (entry->GetObjectType() != PopupMenuEntry::classID) continue;

		maxShortcutTextSize = Math::Max(maxShortcutTextSize, entry->GetShortcutTextSize());
	}

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		PopupMenuEntry	*entry = (PopupMenuEntry *) GetNthObject(i);

		if (entry->GetObjectType() != PopupMenuEntry::classID) continue;

		SetWidth(Math::Max(GetWidth(), 6 + entry->GetMinimumSize().cx - (entry->GetShortcutTextSize() > 0 ? entry->GetShortcutTextSize() - maxShortcutTextSize : 0)));
		SetHeight(GetHeight() + 5 + (entry->GetText() != NIL ? 11 : 0));

		entry->SetPosition(Point(3, nextYPos));

		nextYPos += entry->GetHeight() + 1;
	}

	for (Int j = 0; j < GetNOfObjects(); j++)
	{
		PopupMenuEntry	*entry = (PopupMenuEntry *) GetNthObject(j);

		if (entry->GetObjectType() != PopupMenuEntry::classID) continue;

		entry->SetWidth(GetWidth() - 6);
		entry->SetShortcutOffset(maxShortcutTextSize + 21);
	}
}

S::Void S::GUI::PopupMenu::OnOpenPopupMenu(Int handle)
{
	if (GetFlags() & MB_POPUPOPEN) return;

	if (handle != GetHandle()) internalRequestClose.Emit();
}

S::Void S::GUI::PopupMenu::OnToolWindowPaint()
{
	if (toolWindow == NIL) return;

	Surface	*surface = toolWindow->GetDrawSurface();
	Rect	 frame = Rect(Point(0, 0), GetRealSize());

	surface->Frame(frame, FRAME_UP);
}
