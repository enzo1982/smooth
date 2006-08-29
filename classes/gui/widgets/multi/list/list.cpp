 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

	if (RegisterObject(newEntry) == Success())
	{
		elementOrder.AddEntry(newEntry->GetHandle(), newEntry->GetHandle());
		createdEntry.AddEntry(True, newEntry->GetHandle());

		Paint(SP_UPDATE);

		return newEntry;
	}

	DeleteObject(newEntry);

	return NIL;
}

S::Int S::GUI::List::AddEntry(ListEntry *entry)
{
	entry->Hide();

	if (RegisterObject(entry) == Success())
	{
		elementOrder.AddEntry(entry->GetHandle(), entry->GetHandle());
		createdEntry.AddEntry(False, entry->GetHandle());

		Paint(SP_UPDATE);

		return Success();
	}

	return Error();
}

S::Int S::GUI::List::RemoveEntry(ListEntry *entry)
{
	if (entry == NIL) return Error();

	if (UnregisterObject(entry) == Success())
	{
		Int	 entryHandle = entry->GetHandle();

		if (createdEntry.GetEntry(entryHandle)) DeleteObject(entry);

		elementOrder.RemoveEntry(entryHandle);
		createdEntry.RemoveEntry(entryHandle);

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

		UnregisterObject(widget);

		if (createdEntry.GetEntry(widget->GetHandle())) DeleteObject(widget);
	}

	elementOrder.RemoveAll();
	createdEntry.RemoveAll();

	Paint(SP_PAINT);

	return Success();
}

S::Int S::GUI::List::SwitchEntries(Int entry1n, Int entry2n)
{
	if (entry1n == entry2n)					      return Success();
	if (entry1n >= GetNOfEntries() || entry2n >= GetNOfEntries()) return Error();
	if (entry1n <  0	       || entry2n <  0		    ) return Error();

	Int	 entry1 = elementOrder.GetNthEntry(Math::Min(entry1n, entry2n));
	Int	 entry2 = elementOrder.GetNthEntry(Math::Max(entry1n, entry2n));

	elementOrder.RemoveEntry(entry1);
	elementOrder.RemoveEntry(entry2);

	elementOrder.InsertEntryAtPos(Math::Min(entry1n, entry2n), entry2, entry2);
	elementOrder.InsertEntryAtPos(Math::Max(entry1n, entry2n), entry1, entry1);

	return Success();
}

S::Int S::GUI::List::GetNOfEntries() const
{
	return elementOrder.GetNOfEntries();
}

S::GUI::ListEntry *S::GUI::List::GetNthEntry(Int n) const
{
	Int	 handle = elementOrder.GetNthEntry(n);

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetHandle() == handle) return (ListEntry *) GetNthObject(i);
	}

	return NIL;
}

S::Int S::GUI::List::SelectEntry(ListEntry *entry)
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

		ListEntry	*widget = (ListEntry *) GetNthObject(i);

		if (widget == entry)
		{
			widget->Select();

			return Success();
		}
	}

	return Error();
}

S::GUI::ListEntry *S::GUI::List::GetSelectedEntry() const
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

		ListEntry	*widget = (ListEntry *) GetNthObject(i);

		if (widget->IsSelected()) return widget;
	}

	return NIL;
}

S::Int S::GUI::List::SelectNthEntry(Int n)
{
	if (n >= GetNOfEntries()) return Error();

	GetNthEntry(n)->Select();

	return Success();
}

S::Int S::GUI::List::GetSelectedEntryNumber() const
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

		if (((ListEntry *) GetNthObject(i))->IsSelected())
		{
			for (Int j = 0; j < elementOrder.GetNOfEntries(); j++)
			{
				if (GetNthObject(i)->GetHandle() == elementOrder.GetNthEntry(j)) return j;
			}
		}
	}

	return -1;
}

S::Int S::GUI::List::SelectEntry(const String &entryText)
{
	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

		ListEntry	*entry = (ListEntry *) GetNthObject(i);

		if (entry->GetText() == entryText)
		{
			entry->Select();

			return Success();
		}
	}

	return Error();
}
