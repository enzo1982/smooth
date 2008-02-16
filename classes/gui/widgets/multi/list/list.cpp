 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/list.h>
#include <smooth/misc/math.h>

const S::Int	 S::GUI::List::classID = S::Object::RequestClassID();

S::GUI::List::List() : Widget(Point(), Size())
{
	type	= classID;

	onSelectEntry.SetParentObject(this);
	onMarkEntry.SetParentObject(this);
}

S::GUI::List::~List()
{
	RemoveAllEntries();
}

S::GUI::ListEntry *S::GUI::List::AddEntry(const String &text)
{
	ListEntry	*newEntry = new ListEntry(text);

	newEntry->Hide();

	if (Add(newEntry) == Success())
	{
		elementOrder.Add(newEntry, newEntry->GetHandle());
		createdEntry.Add(True, newEntry->GetHandle());

		Paint(SP_UPDATE);

		return newEntry;
	}

	DeleteObject(newEntry);

	return NIL;
}

S::Int S::GUI::List::AddEntry(ListEntry *entry)
{
	entry->Hide();

	if (Add(entry) == Success())
	{
		elementOrder.Add(entry, entry->GetHandle());
		createdEntry.Add(False, entry->GetHandle());

		Paint(SP_UPDATE);

		return Success();
	}

	return Error();
}

S::Int S::GUI::List::RemoveEntry(ListEntry *entry)
{
	if (entry == NIL) return Error();

	if (Remove(entry) == Success())
	{
		Int	 entryHandle = entry->GetHandle();

		if (createdEntry.Get(entryHandle)) DeleteObject(entry);

		elementOrder.Remove(entryHandle);
		createdEntry.Remove(entryHandle);

		Paint(SP_UPDATE);

		return Success();
	}

	return Error();
}

S::Int S::GUI::List::RemoveAllEntries()
{
	Int	 nonListEntry = 0;

	while (GetNOfObjects() - nonListEntry)
	{
		if (GetNthObject(nonListEntry)->GetObjectType() != ListEntry::classID) { nonListEntry++; continue; }

		Widget	*widget = GetNthObject(nonListEntry);

		Remove(widget);

		if (createdEntry.Get(widget->GetHandle())) DeleteObject(widget);
	}

	elementOrder.RemoveAll();
	createdEntry.RemoveAll();

	Paint(SP_PAINT);

	return Success();
}

S::Int S::GUI::List::SwitchEntries(Int entry1n, Int entry2n)
{
	if (entry1n == entry2n)			        return Success();
	if (entry1n >= Length() || entry2n >= Length()) return Error();
	if (entry1n <  0	|| entry2n <  0	      ) return Error();

	ListEntry	*entry1 = elementOrder.GetNth(Math::Min(entry1n, entry2n));
	ListEntry	*entry2 = elementOrder.GetNth(Math::Max(entry1n, entry2n));

	elementOrder.Remove(entry1->GetHandle());
	elementOrder.Remove(entry2->GetHandle());

	elementOrder.InsertAtPos(Math::Min(entry1n, entry2n), entry2, entry2->GetHandle());
	elementOrder.InsertAtPos(Math::Max(entry1n, entry2n), entry1, entry1->GetHandle());

	return Success();
}

S::Int S::GUI::List::Length() const
{
	return elementOrder.Length();
}

S::GUI::ListEntry *S::GUI::List::GetNthEntry(Int n) const
{
	return elementOrder.GetNth(n);
}

S::Int S::GUI::List::SelectEntry(ListEntry *entryToSelect)
{
	for (Int i = 0; i < elementOrder.Length(); i++)
	{
		ListEntry	*entry = GetNthEntry(i);

		if (entry == entryToSelect)
		{
			entry->Select();

			return Success();
		}
	}

	return Error();
}

S::GUI::ListEntry *S::GUI::List::GetSelectedEntry() const
{
	for (Int i = 0; i < elementOrder.Length(); i++)
	{
		ListEntry	*entry = elementOrder.GetNth(i);

		if (entry->IsSelected()) return entry;
	}

	return NIL;
}

S::Int S::GUI::List::SelectNthEntry(Int n)
{
	if (n >= Length()) return Error();

	GetNthEntry(n)->Select();

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

S::Int S::GUI::List::SelectEntry(const String &entryText)
{
	for (Int i = 0; i < elementOrder.Length(); i++)
	{
		ListEntry	*entry = elementOrder.GetNth(i);

		if (entry->GetText() == entryText)
		{
			entry->Select();

			return Success();
		}
	}

	return Error();
}
