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

const S::Int	 S::GUI::List::classID = S::Object::RequestClassID();

S::GUI::List::List()
{
	type		= classID;
	containerType	= classID;

	internalOnSelectEntry.SetParentObject(this);
}

S::GUI::List::~List()
{
	Clear();
}

S::GUI::ListEntry *S::GUI::List::AddEntry(String text)
{
	ListEntry	*newEntry = new ListEntry(text);

	if (RegisterObject(newEntry) == Success)
	{
		Paint(SP_UPDATE);

		newEntry->Show();

		return newEntry;
	}
	else
	{
		DeleteObject(newEntry);

		return NIL;
	}
}

S::Int S::GUI::List::RemoveEntry(ListEntry *entry)
{
	if (entry == NIL) return Failure;

	if (UnregisterObject(entry) == Success)
	{
		DeleteObject(entry);

		Paint(SP_PAINT);

		return Success;
	}

	return Failure;
}

S::Int S::GUI::List::Clear()
{
	Int	 nOfEntries = assocObjects.GetNOfEntries();

	for (Int i = 0; i < nOfEntries; i++)
	{
		Widget	*widget = assocObjects.GetFirstEntry();

		UnregisterObject(widget);
		DeleteObject(widget);
	}

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::List::GetNOfEntries()
{
	return assocObjects.GetNOfEntries();
}

S::GUI::ListEntry *S::GUI::List::GetNthEntry(Int n)
{
	return (ListEntry *) assocObjects.GetNthEntry(n);
}

S::Int S::GUI::List::SelectEntry(ListEntry *entry)
{
	Int	 retVal = Failure;

	for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
	{
		ListEntry	*widget = (ListEntry *) assocObjects.GetNthEntry(i);

		if (widget == entry)
		{
			widget->clicked = True;
			widget->Paint(SP_PAINT);

			retVal = Success;
		}
		else if (widget->clicked)
		{
			widget->clicked = False;
			widget->Paint(SP_PAINT);
		}
	}

	return retVal;
}

S::GUI::ListEntry *S::GUI::List::GetSelectedEntry()
{
	for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
	{
		ListEntry	*widget = (ListEntry *) assocObjects.GetNthEntry(i);

		if (widget->clicked) return widget;
	}

	return NIL;
}

S::Int S::GUI::List::SelectNthEntry(Int n)
{
	if (n >= assocObjects.GetNOfEntries()) return Failure;

	for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
	{
		ListEntry	*widget = (ListEntry *) assocObjects.GetNthEntry(i);

		if (i == n)
		{
			widget->clicked = True;
			widget->Paint(SP_PAINT);
		}
		else if (widget->clicked)
		{
			widget->clicked = False;
			widget->Paint(SP_PAINT);
		}
	}

	return Success;
}

S::Int S::GUI::List::GetSelectedEntryNumber()
{
	for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
	{
		if (((ListEntry *) assocObjects.GetNthEntry(i))->clicked) return i;
	}

	return -1;
}

S::Int S::GUI::List::SelectEntry(String entryText)
{
	Int	 rVal = Failure;

	for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
	{
		ListEntry	*widget = (ListEntry *) assocObjects.GetNthEntry(i);

		if (widget->GetText() == entryText)
		{
			widget->clicked = True;
			widget->Paint(SP_PAINT);

			rVal = Success;
		}
		else if (widget->clicked)
		{
			widget->clicked = False;
			widget->Paint(SP_PAINT);
		}
	}

	return rVal;
}
