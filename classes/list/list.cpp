 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/list.h>
#include <smooth/definitions.h>
#include <smooth/array.h>
#include <smooth/toolkit.h>
#include <smooth/stk.h>
#include <smooth/i18n.h>
#include <smooth/metrics.h>

S::List::List()
{
	referenceList = NIL;
	addNil = False;
}

S::List::~List()
{
	referenceList = NIL;

	RemoveAll();
}

S::ListEntry *S::List::AddEntry(String name, Int id)
{
	if (referenceList != NIL) return NIL;

	ListEntry	*newEntry = new ListEntry(id);

	newEntry->name = name;

	if (Array<ListEntry *>::AddEntry(newEntry, id) == True)
	{
		return newEntry;
	}
	else
	{
		delete newEntry;

		return NIL;
	}
}

S::Int S::List::ModifyEntry(Int id, String name)
{
	if (referenceList != NIL) return Error;

	ListEntry	*entry = GetEntry(id);

	if (entry != NIL)
	{
		if (entry->name == name) return Break;

		entry->name = name;

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::List::RemoveEntry(Int id)
{
	if (referenceList != NIL) return Error;

	ListEntry	*entry = GetEntry(id);

	if (entry != NIL)
	{
		delete entry;

		Array<ListEntry *>::RemoveEntry(id);

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::List::RemoveAll()
{
	if (referenceList != NIL) return Error;

	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		delete GetNthEntry(i);
	}

	Array<ListEntry *>::RemoveAll();

	return Success;
}

S::ListEntry *S::List::GetSelectedEntry()
{
	if (GetNOfEntries() == 0) return NIL;

	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		ListEntry	*operat = GetNthEntry(i);

		if (operat->clicked) return operat;
	}

	return NIL;
}

S::Int S::List::SelectEntry(Int id)
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

S::Int S::List::SetReferenceList(List *nRefList)
{
	referenceList = nRefList;

	SynchronizeList();

	return Success;
}

S::Void S::List::CheckFlags()
{
}

S::Bool S::List::IsListSane()
{
	CheckFlags();

	if (referenceList == NIL) return True;
	if (GetNOfEntries() != (referenceList->GetNOfEntries() + (addNil ? 1 : 0))) return False;

	for (Int i = 0; i < referenceList->GetNOfEntries(); i++)
	{
		if (referenceList->GetNthEntry(i)->id != GetNthEntry(i + (addNil ? 1 : 0))->id)			return False;
		else if (referenceList->GetNthEntry(i)->name != GetNthEntry(i + (addNil ? 1 : 0))->name)	return False;
	}

	return True;
}

S::Int S::List::SynchronizeList()
{
	if (referenceList == NIL) return Error;

	List	*rList = referenceList;

	referenceList = NIL;

	CheckFlags();
	RemoveAll();

	if (addNil) AddEntry("", 2147483647);

	for (Int i = 0; i < rList->GetNOfEntries(); i++)
	{
		AddEntry(rList->GetNthEntry(i)->name, rList->GetNthEntry(i)->id);
	}

	referenceList = rList;

	return Success;
}
