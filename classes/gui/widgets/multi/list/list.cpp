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

const S::Int	 S::GUI::List::classID = S::Object::RequestClassID();

S::GUI::List::List() : Widget(Point(), Size())
{
	type	= classID;

	onSelectEntry.SetParentObject(this);
	onMarkEntry.SetParentObject(this);
}

S::GUI::List::~List()
{
	Clear();
}

S::GUI::ListEntry *S::GUI::List::AddEntry(const String &text)
{
	ListEntry	*newEntry = new ListEntry(text);

	newEntry->Hide();

	if (RegisterObject(newEntry) != Success())
	{
		DeleteObject(newEntry);

		return NIL;
	}

	Paint(SP_UPDATE);

	return newEntry;
}

S::Int S::GUI::List::RemoveEntry(ListEntry *entry)
{
	if (entry == NIL) return Error();

	if (UnregisterObject(entry) == Success())
	{
		DeleteObject(entry);

		Paint(SP_UPDATE);

		return Success();
	}

	return Error();
}

S::Int S::GUI::List::Clear()
{
	Int	 nonListEntry = 0;

	while (GetNOfObjects() - nonListEntry)
	{
		if (GetNthObject(nonListEntry)->GetObjectType() != ListEntry::classID) { nonListEntry++; continue; }

		Widget	*widget = GetNthObject(nonListEntry);

		UnregisterObject(widget);
		DeleteObject(widget);
	}

	Paint(SP_PAINT);

	return Success();
}

S::Int S::GUI::List::GetNOfEntries()
{
	Int	n = 0;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

		n++;
	}

	return n;
}

S::GUI::ListEntry *S::GUI::List::GetNthEntry(Int n)
{
	Int	m = 0;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

		if (m == n) return (ListEntry *) GetNthObject(i);
		else	    m++;
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

S::GUI::ListEntry *S::GUI::List::GetSelectedEntry()
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
	if (n >= GetNOfObjects()) return Error();

	GetNthEntry(n)->Select();

	return Success();
}

S::Int S::GUI::List::GetSelectedEntryNumber()
{
	Int	n = 0;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

		if (((ListEntry *) GetNthObject(i))->IsSelected()) return n;

		n++;
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
