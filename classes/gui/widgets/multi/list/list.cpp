 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/list/list.h>
#include <smooth/misc/i18n.h>

S::GUI::List::List()
{
	referenceList = NIL;
	addNil = False;
}

S::GUI::List::~List()
{
	referenceList = NIL;

	RemoveAll();
}

S::GUI::ListEntry *S::GUI::List::AddEntry(String name, Int id)
{
	if (referenceList != NIL) return NIL;

	ListEntry	*newEntry = new ListEntry(id);

	newEntry->SetText(name);

	if (Array<ListEntry *>::AddEntry(newEntry, id) == True)
	{
		return newEntry;
	}
	else
	{
		Object::DeleteObject(newEntry);

		return NIL;
	}
}

S::Int S::GUI::List::ModifyEntry(Int id, String name)
{
	if (referenceList != NIL) return Error;

	ListEntry	*entry = GetEntry(id);

	if (entry != NIL)
	{
		if (entry->GetText() == name) return Break;

		Bool	 prevClicked = entry->clicked;

		entry->SetText(name);

		entry->clicked = prevClicked;

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::GUI::List::RemoveEntry(Int id)
{
	if (referenceList != NIL) return Error;

	ListEntry	*entry = GetEntry(id);

	if (entry != NIL)
	{
		Object::DeleteObject(entry);

		Array<ListEntry *>::RemoveEntry(id);

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::GUI::List::RemoveAll()
{
	if (referenceList != NIL) return Error;

	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		Object::DeleteObject(GetNthEntry(i));
	}

	Array<ListEntry *>::RemoveAll();

	return Success;
}

S::GUI::ListEntry *S::GUI::List::GetSelectedEntry()
{
	if (GetNOfEntries() == 0) return NIL;

	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		ListEntry	*operat = GetNthEntry(i);

		if (operat->clicked) return operat;
	}

	return NIL;
}

S::Int S::GUI::List::SelectEntry(Int id)
{
	if (GetNOfEntries() == 0) return Error;

	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		ListEntry	*operat = GetNthEntry(i);

		if (operat->clicked)	operat->clicked = False;
		if (operat->id == id)	operat->clicked = True;
	}

	return Success;
}

S::Int S::GUI::List::SetReferenceList(List *nRefList)
{
	referenceList = nRefList;

	SynchronizeList();

	return Success;
}

S::Void S::GUI::List::CheckFlags()
{
}

S::Bool S::GUI::List::IsListSane()
{
	CheckFlags();

	if (referenceList == NIL) return True;
	if (GetNOfEntries() != (referenceList->GetNOfEntries() + (addNil ? 1 : 0))) return False;

	for (Int i = 0; i < referenceList->GetNOfEntries(); i++)
	{
		if (referenceList->GetNthEntry(i)->id != GetNthEntry(i + (addNil ? 1 : 0))->id)				return False;
		else if (referenceList->GetNthEntry(i)->GetText() != GetNthEntry(i + (addNil ? 1 : 0))->GetText())	return False;
	}

	return True;
}

S::Int S::GUI::List::SynchronizeList()
{
	if (referenceList == NIL) return Error;

	List	*rList = referenceList;

	referenceList = NIL;

	CheckFlags();
	RemoveAll();

	if (addNil) AddEntry("", 2147483647);

	for (Int i = 0; i < rList->GetNOfEntries(); i++)
	{
		AddEntry(rList->GetNthEntry(i)->GetText(), rList->GetNthEntry(i)->id);
	}

	referenceList = rList;

	return Success;
}
