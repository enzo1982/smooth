 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/menu.h>
#include <smooth/definitions.h>
#include <smooth/array.h>
#include <smooth/toolkit.h>
#include <smooth/color.h>
#include <smooth/system.h>
#include <smooth/metrics.h>
#include <smooth/i18n.h>

S::Menu::Menu()
{
	nOfEntries = 0;

	sizeset = False;
	entrysizesset = False;
}

S::Menu::~Menu()
{
	Int	 maxCount = nOfEntries;

	for (Int i = 0; i < maxCount; i++)
	{
		RemoveEntry(entries.GetFirstEntry());
	}
}

S::MenuEntry *S::Menu::AddEntry(String text, GUI::Bitmap bitmap, Menu *popupMenu, Bool *bVar, Int *iVar, Int iCode, Int orientation)
{
	Int	 id	= System::RequestGUID();
	Int	 type	= SM_SEPARATOR;

	if (text != NIL)	type = type | SM_TEXT;
	if (bitmap != NIL)	type = type | SM_BITMAP;

	MenuEntry	*newEntry = new MenuEntry(type, id);

	newEntry->text		= text;
	newEntry->orientation	= orientation;

	if (bitmap != NIL)
	{
		newEntry->bitmap	= bitmap;
		newEntry->graymap	= bitmap;

		newEntry->bitmap.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);

		newEntry->graymap.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);
		newEntry->graymap.GrayscaleBitmap();
	}

	newEntry->popup		= popupMenu;
	newEntry->bVar		= bVar;
	newEntry->iVar		= iVar;
	newEntry->iCode		= iCode;
	newEntry->sizeset	= False;

	if (entries.AddEntry(newEntry, id) == True)
	{
		nOfEntries++;

		sizeset		= False;
		entrysizesset	= False;

		return newEntry;
	}
	else
	{
		Object::DeleteObject(newEntry);

		return NIL;
	}
}

S::Int S::Menu::RemoveEntry(MenuEntry *entry)
{
	if (entry == NIL) return Error;

	if (entries.GetEntry(entry->id) != NIL)
	{
		entries.RemoveEntry(entry->id);

		Object::DeleteObject(entry);

		nOfEntries--;

		return Success;
	}

	return Error;
}

S::Int S::Menu::RemoveEntry(Int id)
{
	return RemoveEntry(entries.GetEntry(id));
}

S::Int S::Menu::Clear()
{
	Int	 maxCount = nOfEntries;

	for (Int i = 0; i < maxCount; i++)
	{
		RemoveEntry(entries.GetFirstEntry());
	}

	return Success;
}

S::Void S::Menu::GetSize()
{
	if (!entrysizesset)
	{
		GetMenuEntriesSize();

		entrysizesset = True;
	}
	if (!sizeset)
	{
		popupsize.cx = GetSizeX();
		popupsize.cy = GetSizeY();

		sizeset = True;
	}
}

S::Int S::Menu::GetSizeX()
{
	Int	 mSize = 50;
	Int	 greatest = 0;

	if (nOfEntries == 0) return mSize;

	for (Int i = 0; i < nOfEntries; i++)
	{
		MenuEntry	*entry = entries.GetNthEntry(i);

		if (entry->size > greatest)
		{
			mSize		= 50 + entry->size;
			greatest	= entry->size;
		}
	}

	return mSize;
}

S::Int S::Menu::GetSizeY()
{
	Int	 mSize = 4;

	if (nOfEntries == 0) return mSize;

	for (Int i = 0; i < nOfEntries; i++)
	{
		MenuEntry	*entry = entries.GetNthEntry(i);

		if (entry->type == SM_SEPARATOR)	mSize = mSize + 5;
		else					mSize = mSize + METRIC_POPUPENTRYSIZE;
	}

	return mSize;
}

S::Void S::Menu::GetMenuEntriesSize()
{
	if (nOfEntries == 0) return;

	for (Int i = 0; i < nOfEntries; i++)
	{
		MenuEntry	*operat = entries.GetNthEntry(i);
		GUI::Font	 font(I18N_DEFAULTFONT, I18N_SMALLFONTSIZE, 0, FW_NORMAL);

		if (!operat->sizeset) operat->size = font.GetTextSizeX(operat->text);

		operat->sizeset = True;
	}
}

S::Int S::Menu::GetNOfEntries()
{
	return nOfEntries;
}
