 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/list.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>
#include <smooth/foreach.h>

const S::Short	 S::GUI::List::classID = S::Object::RequestClassID();

S::GUI::List::List() : Widget(Point(), Size())
{
	type	= classID;

	onSelectEntry.SetParentObject(this);
	onMarkEntry.SetParentObject(this);

	onChangeEntryOrder.SetParentObject(this);
}

S::GUI::List::~List()
{
	RemoveAllEntries();
}

S::Int S::GUI::List::EnableLocking(Bool enable)
{
	if (enable)
	{
		createdEntry.EnableLocking();
		elementOrder.EnableLocking();
	}
	else
	{
		createdEntry.DisableLocking();
		elementOrder.DisableLocking();
	}

	return Widget::EnableLocking(enable);
}

S::GUI::ListEntry *S::GUI::List::AddEntry(const String &text, Bool marked)
{
	ListEntry	*newEntry = new ListEntry(text);

	newEntry->SetMark(marked);

	if (Add(newEntry) == Success())
	{
		createdEntry.Set(newEntry->GetHandle(), True);

		return newEntry;
	}

	DeleteObject(newEntry);

	return NIL;
}

S::GUI::ListEntrySeparator *S::GUI::List::AddSeparator()
{
	ListEntrySeparator	*newEntry = new ListEntrySeparator();

	if (Add(newEntry) == Success())
	{
		createdEntry.Set(newEntry->GetHandle(), True);

		return newEntry;
	}

	DeleteObject(newEntry);

	return NIL;
}

S::Int S::GUI::List::Add(Widget *widget)
{
	if (widget == NIL) return Error();

	if (widget->GetObjectType() == ListEntry::classID) widget->Hide();

	if (Widget::Add(widget) == Success() && widget->GetObjectType() == ListEntry::classID)
	{
		elementOrder.Add((ListEntry *) widget, widget->GetHandle());
		createdEntry.Add(False, widget->GetHandle());

		Paint(SP_UPDATE);
		Process(SM_MOUSEMOVE, 0, 0);

		onChangeEntryOrder.Emit();

		return Success();
	}

	return Error();
}

S::Int S::GUI::List::Remove(Widget *widget)
{
	if (widget == NIL) return Error();

	if (Widget::Remove(widget) == Success() && widget->GetObjectType() == ListEntry::classID)
	{
		Int	 entryHandle = widget->GetHandle();

		if (createdEntry.Get(entryHandle)) DeleteObject(widget);

		elementOrder.Remove(entryHandle);
		createdEntry.Remove(entryHandle);

		Paint(SP_UPDATE);
		Process(SM_MOUSEMOVE, 0, 0);

		onChangeEntryOrder.Emit();

		return Success();
	}

	return Error();
}

S::Int S::GUI::List::RemoveAllEntries()
{
	Surface	*surface = NIL;

	if (IsVisible()) surface = GetDrawSurface();

	Rect	 frame = Rect(GetRealPosition(), GetRealSize());

	if (surface != NIL)
	{
		surface->StartPaint(frame);

		Hide();
	}

	while (Length() > 0) Remove(GetNthEntry(Length() - 1));

	if (surface != NIL)
	{
		Show();

		surface->EndPaint();
	}

	return Success();
}

S::Int S::GUI::List::MoveEntry(Int n, Int m)
{
	if (n == m)			    return Success();
	if (n >= Length() || m >= Length()) return Error();
	if (n <  0	  || m <  0	  ) return Error();

	elementOrder.MoveNth(n, m);

	onChangeEntryOrder.Emit();

	return Success();
}

S::Int S::GUI::List::SelectEntry(const ListEntry *entryToSelect)
{
	foreach (ListEntry *entry, elementOrder)
	{
		if (entry != entryToSelect) continue;

		entry->Select();

		return Success();
	}

	return Error();
}

S::GUI::ListEntry *S::GUI::List::GetSelectedEntry() const
{
	foreach (ListEntry *entry, elementOrder)
	{
		if (entry->IsSelected()) return entry;
	}

	return NIL;
}

S::Int S::GUI::List::SelectNthEntry(Int n)
{
	if (n < 0 || n >= Length()) return Error();

	ListEntry	*entry = GetNthEntry(n);

	entry->Select();

	return Success();
}

S::Int S::GUI::List::GetSelectedEntryNumber() const
{
	for (Int i = 0; i < elementOrder.Length(); i++)
	{
		if (elementOrder.GetNth(i)->IsSelected()) return i;
	}

	return -1;
}

S::Int S::GUI::List::GetEntryNumber(const String &entryText) const
{
	for (Int i = 0; i < elementOrder.Length(); i++)
	{
		ListEntry	*entry = elementOrder.GetNth(i);

		if (entry->GetText() == entryText) return i;
	}

	return -1;
}

S::GUI::ListEntry *S::GUI::List::GetEntry(const String &entryText) const
{
	return GetNthEntry(GetEntryNumber(entryText));
}

S::Int S::GUI::List::SelectEntry(const String &entryText)
{
	return SelectNthEntry(GetEntryNumber(entryText));
}
