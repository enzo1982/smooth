 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_LIST_
#define __OBJSMOOTH_LIST_

#include <smooth/list.h>
#include <smooth/definitions.h>
#include <smooth/array.h>
#include <smooth/toolkit.h>
#include <smooth/stk.h>
#include <smooth/i18n.h>
#include <smooth/metrics.h>

SMOOTHList::SMOOTHList()
{
	nOfEntries = 0;

	entrysizesset = SMOOTH::False;
}

SMOOTHList::~SMOOTHList()
{
	for (SMOOTHInt i = 0; i < nOfEntries; i++)
	{
		delete entries.GetNthEntry(i);
	}

	entries.DeleteAll();
}

SMOOTHInt SMOOTHList::AddListEntry(SMOOTHInt code, SMOOTHString text, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	SMOOTHList::Entry	*newentry = new SMOOTHList::Entry(code);

	newentry->text		= text;
	newentry->proc		= (SMOOTHProcType) newProc;
	newentry->procParam	= procParam;

	if (entries.AddEntry(newentry, code) == SMOOTH::True)
	{
		nOfEntries++;

		entrysizesset = SMOOTH::False;

		return SMOOTH::Success;
	}
	else
	{
		delete newentry;

		return SMOOTH::Error;
	}
}

SMOOTHInt SMOOTHList::ModifyListEntry(SMOOTHInt code, SMOOTHString text, SMOOTHProcParam, SMOOTHVoid *procParam)
{
	SMOOTHList::Entry	*entry = entries.GetEntry(code);

	if (entry != NIL)
	{
		entry->text		= text;
		entry->proc		= (SMOOTHProcType) newProc;
		entry->procParam	= procParam;

		entrysizesset = SMOOTH::False;

		return SMOOTH::Success;
	}
	else
	{
		return SMOOTH::Error;
	}
}

SMOOTHInt SMOOTHList::RemoveListEntry(SMOOTHInt code)
{
	if (entries.GetEntry(code) != NIL)
	{
		delete entries.GetEntry(code);

		entries.DeleteEntry(code);

		nOfEntries--;

		return SMOOTH::Success;
	}
	else
	{
		return SMOOTH::Error;
	}
}

SMOOTHVoid SMOOTHList::CleanupList()
{
	SMOOTHList::Entry	*operat;

	for (SMOOTHInt i = 0; i < nOfEntries; i++)
	{
		operat = entries.GetNthEntry(i);

		delete operat;
	}

	entries.DeleteAll();

	nOfEntries = 0;
}

SMOOTHVoid SMOOTHList::GetSize()
{
	if (!entrysizesset)
	{
		GetListEntriesSize();
		entrysizesset = SMOOTH::True;
	}
}

SMOOTHVoid SMOOTHList::GetListEntriesSize()
{
	if (nOfEntries == 0) return;

	SMOOTHList::Entry	*operat;
	HDC			 hdc = GetContext(0);

	for (SMOOTHInt i = 0; i < nOfEntries; i++)
	{
		operat = entries.GetNthEntry(i);

#ifdef __WIN32__
		if (!operat->sizeset) operat->size = GetTextSizeX(hdc, operat->text, I18N_DEFAULTFONT, -MulDiv(I18N_SMALLFONTSIZE, GetDeviceCaps(hdc, LOGPIXELSY), 72), FW_NORMAL);
#endif

		operat->sizeset = SMOOTH::True;
	}

	FreeContext(0, hdc);
}

SMOOTHInt SMOOTHList::GetSelectedEntry()
{
	if (nOfEntries == 0) return -1;

	SMOOTHList::Entry	*operat;

	for (SMOOTHInt i = 0; i < nOfEntries; i++)
	{
		operat = entries.GetNthEntry(i);

		if (operat->clk) return operat->code;
	}

	return -1;
}

SMOOTHInt SMOOTHList::SelectListEntry(SMOOTHInt code)
{
	if (nOfEntries == 0) return SMOOTH::Error;

	SMOOTHList::Entry	*operat;

	for (SMOOTHInt i = 0; i < nOfEntries; i++)
	{
		operat = entries.GetNthEntry(i);

		if (operat->clk) operat->clk = SMOOTH::False;

		if (operat->code == code) operat->clk = SMOOTH::True;
	}

	return SMOOTH::Success;
}

SMOOTHString SMOOTHList::GetSelectedEntryName()
{
	SMOOTHList::Entry	*operat;

	for (SMOOTHInt i = 0; i < nOfEntries; i++)
	{
		operat = entries.GetNthEntry(i);

		if (operat->clk) return operat->text;
	}

	return NIL;
}

SMOOTHInt SMOOTHList::GetNOfEntries()
{
	return nOfEntries;
}

#endif
