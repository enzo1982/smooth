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
	nOfEntries = 0;

	entrysizesset = False;
}

S::List::~List()
{
	for (Int i = 0; i < nOfEntries; i++)
	{
		delete entries.GetNthEntry(i);
	}

	entries.DeleteAll();
}

S::List::Entry *S::List::AddListEntry(Int code, String text)
{
	List::Entry	*newEntry = new List::Entry(code);

	newEntry->text = text;

	if (entries.AddEntry(newEntry, code) == True)
	{
		nOfEntries++;

		entrysizesset = False;

		return newEntry;
	}
	else
	{
		delete newEntry;

		return NIL;
	}
}

S::Int S::List::ModifyListEntry(Int code, String text)
{
	List::Entry	*entry = entries.GetEntry(code);

	if (entry != NIL)
	{
		entry->text = text;

		entrysizesset = False;

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Int S::List::RemoveListEntry(Int code)
{
	if (entries.GetEntry(code) != NIL)
	{
		delete entries.GetEntry(code);

		entries.DeleteEntry(code);

		nOfEntries--;

		return Success;
	}
	else
	{
		return Error;
	}
}

S::Void S::List::CleanupList()
{
	List::Entry	*operat;

	for (Int i = 0; i < nOfEntries; i++)
	{
		operat = entries.GetNthEntry(i);

		delete operat;
	}

	entries.DeleteAll();

	nOfEntries = 0;
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
	if (nOfEntries == 0) return;

	List::Entry	*operat;
	HDC		 hdc = GetContext(0);

	for (Int i = 0; i < nOfEntries; i++)
	{
		operat = entries.GetNthEntry(i);

#ifdef __WIN32__
		if (!operat->sizeset) operat->size = GetTextSizeX(operat->text, I18N_DEFAULTFONT, -MulDiv(I18N_SMALLFONTSIZE, GetDeviceCaps(hdc, LOGPIXELSY), 72), FW_NORMAL);
#endif

		operat->sizeset = True;
	}

	FreeContext(0, hdc);
}

S::Int S::List::GetSelectedEntry()
{
	if (nOfEntries == 0) return -1;

	List::Entry	*operat;

	for (Int i = 0; i < nOfEntries; i++)
	{
		operat = entries.GetNthEntry(i);

		if (operat->clk) return operat->code;
	}

	return -1;
}

S::Int S::List::SelectListEntry(Int code)
{
	if (nOfEntries == 0) return Error;

	List::Entry	*operat;

	for (Int i = 0; i < nOfEntries; i++)
	{
		operat = entries.GetNthEntry(i);

		if (operat->clk) operat->clk = False;

		if (operat->code == code) operat->clk = True;
	}

	return Success;
}

S::String S::List::GetSelectedEntryName()
{
	List::Entry	*operat;

	for (Int i = 0; i < nOfEntries; i++)
	{
		operat = entries.GetNthEntry(i);

		if (operat->clk) return operat->text;
	}

	return NIL;
}

S::Int S::List::GetNOfEntries()
{
	return nOfEntries;
}

S::String S::List::GetNthEntryName(Int n)
{
	List::Entry	*operat = entries.GetNthEntry(n);

	if (operat != NIL)	return operat->text;
	else			return NIL;
}
