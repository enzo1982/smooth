 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_MENU_
#define __OBJSMOOTH_MENU_

#include <smooth/menu.h>
#include <smooth/definitions.h>
#include <smooth/array.h>
#include <smooth/toolkit.h>
#include <smooth/stk.h>
#include <smooth/metrics.h>
#include <smooth/i18n.h>

SMOOTHMenu::SMOOTHMenu()
{
	nOfEntries = 0;

	sizeset = SMOOTH::False;
	entrysizesset = SMOOTH::False;
}

SMOOTHMenu::~SMOOTHMenu()
{
	SMOOTHInt	 maxCount = nOfEntries;

	for (SMOOTHInt i = 0; i < maxCount; i++)
	{
		RemoveEntry(entries.GetFirstEntry());
	}
}

SMOOTHMenu::Entry *SMOOTHMenu::AddEntry(SMOOTHString text, HBITMAP bitmap, SMOOTHProcParam, SMOOTHVoid *procParam, SMOOTHMenu *popupMenu, SMOOTHBool *bVar, SMOOTHInt *iVar, SMOOTHInt iCode, SMOOTHInt orientation)
{
	SMOOTHInt	 id	= SMOOTH::RequestGUID();
	SMOOTHInt	 type	= SM_SEPARATOR;

	if (text != NIL)	type = type | SM_TEXT;
	if (bitmap != NIL)	type = type | SM_BITMAP;

	SMOOTHMenu::Entry	*newEntry = new SMOOTHMenu::Entry(type, id);

	newEntry->text		= text;
	newEntry->orientation	= orientation;
	newEntry->proc		= (SMOOTHProcType) newProc;
	newEntry->procParam	= procParam;
	newEntry->bitmap	= DetectTransparentRegions(bitmap);
	newEntry->graymap	= DetectTransparentRegions(GrayscaleBitmap(bitmap));
	newEntry->popup		= popupMenu;
	newEntry->bVar		= bVar;
	newEntry->iVar		= iVar;
	newEntry->iCode		= iCode;
	newEntry->sizeset	= SMOOTH::False;

	if (entries.AddEntry(newEntry, id) == SMOOTH::True)
	{
		nOfEntries++;

		sizeset		= SMOOTH::False;
		entrysizesset	= SMOOTH::False;

		return newEntry;
	}
	else
	{
		delete newEntry;

		return NIL;
	}
}

SMOOTHInt SMOOTHMenu::RemoveEntry(SMOOTHMenu::Entry *entry)
{
	if (entry == NIL) return SMOOTH::Error;

	if (entries.GetEntry(entry->id) != NIL)
	{
		entries.DeleteEntry(entry->id);

		delete entry;

		nOfEntries--;

		return SMOOTH::Success;
	}

	return SMOOTH::Error;
}

SMOOTHInt SMOOTHMenu::RemoveEntry(SMOOTHInt id)
{
	return RemoveEntry(entries.GetEntry(id));
}

SMOOTHInt SMOOTHMenu::Clear()
{
	SMOOTHInt	 maxCount = nOfEntries;

	for (SMOOTHInt i = 0; i < maxCount; i++)
	{
		RemoveEntry(entries.GetFirstEntry());
	}

	return SMOOTH::Success;
}

SMOOTHVoid SMOOTHMenu::GetSize()
{
	if (!entrysizesset)
	{
		GetMenuEntriesSize();

		entrysizesset = SMOOTH::True;
	}
	if (!sizeset)
	{
		popupsize.cx = GetSizeX();
		popupsize.cy = GetSizeY();

		sizeset = SMOOTH::True;
	}
}

SMOOTHInt SMOOTHMenu::GetSizeX()
{
	SMOOTHMenu::Entry	*operat;
	HDC			 hdc;
	SMOOTHInt		 msize = 50;
	SMOOTHInt		 greatest = 0;
	SMOOTHInt		 textsize;

	if (nOfEntries == 0) return msize;
	else
	{
		hdc = GetContext(0);

		for (SMOOTHInt i = 0; i < nOfEntries; i++)
		{
			operat = entries.GetNthEntry(i);

			textsize = operat->size;

			if (textsize > greatest)
			{
				msize = 50 + textsize;
				greatest = textsize;
			}
		}
	}

	FreeContext(0, hdc);

	return msize;
}

SMOOTHInt SMOOTHMenu::GetSizeY()
{
	SMOOTHMenu::Entry	*operat;
	HDC			 hdc;
	SMOOTHInt		 msize = 4;

	if (nOfEntries == 0) return msize;
	else
	{
		hdc = GetContext(0);

		for (SMOOTHInt i = 0; i < nOfEntries; i++)
		{
			operat = entries.GetNthEntry(i);

			if (operat->type == SM_SEPARATOR)	msize = msize + 5;
			else					msize = msize + METRIC_POPUPENTRYSIZE;
		}
	}

	FreeContext(0, hdc);

	return msize;
}

SMOOTHVoid SMOOTHMenu::GetMenuEntriesSize()
{
	SMOOTHMenu::Entry	*operat;
	HDC			 hdc;

	if (nOfEntries == 0) return;
	else
	{
		hdc = GetContext(0);

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
}

SMOOTHInt SMOOTHMenu::GetNOfEntries()
{
	return nOfEntries;
}

#endif
