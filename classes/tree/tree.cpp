 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/tree.h>
#include <smooth/definitions.h>
#include <smooth/array.h>
#include <smooth/i18n.h>
#include <smooth/metrics.h>
#include <smooth/graphics/font.h>

S::Tree::Tree()
{
	nOfEntries = 0;

	sizeset = False;
	entrysizesset = False;
}

S::Tree::~Tree()
{
	for (Int i = 0; i < nOfEntries; i++)
	{
		delete entries.GetNthEntry(i);
	}

	entries.RemoveAll();
}

S::Tree::Entry *S::Tree::AddEntry(String text, Tree *sub)
{
	Int	 id	= System::RequestGUID();
	Int	 type	= ST_ENTRY;

	if (sub != NIL) type = ST_SUB;

	Tree::Entry	*newEntry = new Tree::Entry(type, id);

	newEntry->text		= text;
	newEntry->sub		= sub;
	newEntry->sizeset	= False;
	newEntry->last		= True;

	if (nOfEntries != 0) entries.GetLastEntry()->last = False;

	if (entries.AddEntry(newEntry, id) == True)
	{
		nOfEntries++;

		sizeset = False;
		entrysizesset = False;

		return newEntry;
	}
	else
	{
		delete newEntry;

		return NIL;
	}
}

S::Int S::Tree::RemoveEntry(Tree::Entry *entry)
{
	if (entry == NIL) return Error;

	if (entries.GetEntry(entry->id) != NIL)
	{
		entries.RemoveEntry(entry->id);

		delete entry;

		nOfEntries--;

		return Success;
	}

	return Error;
}

S::Int S::Tree::RemoveEntry(Int id)
{
	return RemoveEntry(entries.GetEntry(id));
}

S::Void S::Tree::GetSize()
{
	if (!entrysizesset)
	{
		GetTreeEntriesSize();
		entrysizesset = True;
	}
	if (!sizeset)
	{
		subsize.cx = GetSizeX();
		subsize.cy = GetSizeY();
		sizeset = True;
	}
}

S::Int S::Tree::GetSizeX()
{
	Int	 mSize = 50;
	Int	 greatest = 0;

	if (nOfEntries == 0) return mSize;

	for (Int i = 0; i < nOfEntries; i++)
	{
		Tree::Entry	*entry = entries.GetNthEntry(i);

		if (entry->size > greatest)
		{
			mSize = 50 + entry->size;
			greatest = entry->size;
		}
	}

	return mSize;
}

S::Int S::Tree::GetSizeY()
{
	if (nOfEntries == 0) return 4;

	return 4 + 19 * nOfEntries;
}

S::Void S::Tree::GetTreeEntriesSize()
{
	if (nOfEntries == 0) return;

	for (Int i = 0; i < nOfEntries; i++)
	{
		Tree::Entry	*operat = entries.GetNthEntry(i);
		GUI::Font	 font(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, 0, FW_NORMAL);

		if (!operat->sizeset) operat->size = font.GetTextSizeX(operat->text);

		operat->sizeset = True;
	}
}

S::Int S::Tree::GetNOfEntries()
{
	return nOfEntries;
}
