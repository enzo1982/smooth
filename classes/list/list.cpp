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
	entrysizesset = False;
}

S::List::~List()
{
	Cleanup();
}

S::ListEntry *S::List::AddEntry(Int id, String name)
{
	ListEntry	*newEntry = new ListEntry(id);

	newEntry->name = name;

	if (Array<ListEntry *>::AddEntry(newEntry, id) == True)
	{
		entrysizesset = False;

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
	ListEntry	*entry = GetEntry(id);

	if (entry != NIL)
	{
		entry->name	= name;
		entrysizesset	= False;

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::List::RemoveEntry(Int id)
{
	if (GetEntry(id) != NIL)
	{
		delete GetEntry(id);

		RemoveEntry(id);

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Void S::List::Cleanup()
{
	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		delete GetNthEntry(i);
	}

	RemoveAll();
}

S::Void S::List::GetSize()
{
	if (!entrysizesset)
	{
		GetListEntriesSize();
		entrysizesset = True;
	}
}

S::Void S::List::GetListEntriesSize()
{
	if (GetNOfEntries() == 0) return;

	HDC	 hdc = GetContext(0);

	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		ListEntry	*operat = GetNthEntry(i);

		if (!operat->sizeset) operat->size = GetTextSizeX(operat->name, I18N_DEFAULTFONT, -MulDiv(I18N_SMALLFONTSIZE, GetDeviceCaps(hdc, LOGPIXELSY), 72), FW_NORMAL);

		operat->sizeset = True;
	}

	FreeContext(0, hdc);
}

S::ListEntry *S::List::GetSelectedEntry()
{
	if (GetNOfEntries() == 0) return NIL;

	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		ListEntry	*operat = GetNthEntry(i);

		if (operat->clk) return operat;
	}

	return NIL;
}

S::Int S::List::SelectEntry(Int id)
{
	if (GetNOfEntries() == 0) return Error;

	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		ListEntry	*operat = GetNthEntry(i);

		if (operat->clk)	operat->clk = False;
		if (operat->id == id)	operat->clk = True;
	}

	return Success;
}
