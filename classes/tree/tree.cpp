 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_TREE_
#define __OBJSMOOTH_TREE_

#include <smooth/tree.h>
#include <smooth/definitions.h>
#include <smooth/array.h>
#include <smooth/toolkit.h>
#include <smooth/stk.h>
#include <smooth/i18n.h>
#include <smooth/metrics.h>

SMOOTHTree::SMOOTHTree()
{
	nOfEntries = 0;

	sizeset = SMOOTH::False;
	entrysizesset = SMOOTH::False;
	counter = -1;
}

SMOOTHTree::~SMOOTHTree()
{
	for (SMOOTHInt i = 0; i < nOfEntries; i++)
	{
		delete entries.GetNthEntry(i);
	}

	entries.DeleteAll();
}

SMOOTHInt SMOOTHTree::AddEntry(SMOOTHString name, SMOOTHProcParam, SMOOTHVoid *pparam)
{
	counter++;

	AddTreeEntry(ST_ENTRY, counter, name, newProc, pparam, NIL);

	return counter;
}

SMOOTHInt SMOOTHTree::AddEntry(SMOOTHString name, SMOOTHTree *sub)
{
	counter++;

	AddTreeEntry(ST_SUB, counter, name, NULLPROC, sub);

	return counter;
}

SMOOTHInt SMOOTHTree::RemoveEntry(SMOOTHInt number)
{
	RemoveTreeEntry(number);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHTree::AddTreeEntry(SMOOTHInt type, SMOOTHInt code, SMOOTHString text, SMOOTHProcParam, SMOOTHVoid *pparam, SMOOTHTree *sub)
{
	SMOOTHTree::Entry	*newentry = new SMOOTHTree::Entry(type, code);

	newentry->text		= text;
	newentry->proc		= (SMOOTHProcType) newProc;
	newentry->procParam	= pparam;
	newentry->sub		= sub;
	newentry->sizeset	= SMOOTH::False;
	newentry->last		= SMOOTH::True;

	if (nOfEntries != 0) entries.GetLastEntry()->last = SMOOTH::False;

	if (entries.AddEntry(newentry, code) == SMOOTH::True)
	{
		nOfEntries++;

		sizeset = SMOOTH::False;
		entrysizesset = SMOOTH::False;

		return SMOOTH::Success;
	}
	else
	{
		delete newentry;

		return SMOOTH::Error;
	}
}

SMOOTHInt SMOOTHTree::RemoveTreeEntry(SMOOTHInt code)
{
	if (entries.GetEntry(code) != NIL)
	{
		delete entries.GetEntry(code);

		entries.DeleteEntry(code);

		nOfEntries--;

		if (nOfEntries != 0) entries.GetLastEntry()->last = SMOOTH::True;

		return SMOOTH::Success;
	}
	else
	{
		return SMOOTH::Error;
	}
}

SMOOTHVoid SMOOTHTree::GetSize()
{
	if (!entrysizesset)
	{
		GetTreeEntriesSize();
		entrysizesset = SMOOTH::True;
	}
	if (!sizeset)
	{
		subsize.cx = GetSizeX();
		subsize.cy = GetSizeY();
		sizeset = SMOOTH::True;
	}
}

SMOOTHInt SMOOTHTree::GetSizeX()
{
	SMOOTHTree::Entry	*operat;
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

SMOOTHInt SMOOTHTree::GetSizeY()
{
	SMOOTHTree::Entry	*operat;
	HDC			 hdc;
	SMOOTHInt		 msize = 4;

	if (nOfEntries == 0) return msize;
	else
	{
		hdc = GetContext(0);

		for (SMOOTHInt i = 0; i < nOfEntries; i++)
		{
			operat = entries.GetNthEntry(i);

			msize = msize + 19;
		}
	}

	FreeContext(0, hdc);

	return msize;
}

SMOOTHVoid SMOOTHTree::GetTreeEntriesSize()
{
	SMOOTHTree::Entry	*operat;
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

SMOOTHInt SMOOTHTree::GetNOfEntries()
{
	return nOfEntries;
}

#endif
