 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_MENUENTRY_
#define __OBJSMOOTH_MENUENTRY_

#include <smooth/menu.h>
#include <smooth/definitions.h>
#include <smooth/stk.h>

SMOOTHMenu::Entry::Entry(SMOOTHInt newType, SMOOTHInt newID)
{
	type		= newType;
	id		= newID;

	bitmap		= NIL;
	graymap		= NIL;

	orientation	= OR_LEFT;

	checked		= SMOOTH::False;
	clicked		= SMOOTH::False;

	proc		= NIL;
	procParam	= NIL;

	bVar		= NIL;
	iVar		= NIL;

	popup		= NIL;
}

SMOOTHMenu::Entry::~Entry()
{
}

SMOOTHInt SMOOTHMenu::Entry::SetText(SMOOTHString newText)
{
	if (newText == NIL) type = (type | SM_TEXT) ^ SM_TEXT;

	text = newText;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHMenu::Entry::SetTooltip(SMOOTHString newTooltip)
{
	tooltip = newTooltip;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHMenu::Entry::SetStatusText(SMOOTHString newDescription)
{
	description = newDescription;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHMenu::Entry::SetBitmap(HBITMAP newBitmap)
{
	if (newBitmap == NIL)
	{
		type = (type | SM_BITMAP) ^ SM_BITMAP;

		bitmap = NIL;
		graymap = NIL;
	}
	else
	{
		bitmap = DetectTransparentRegions(newBitmap);
		graymap = DetectTransparentRegions(GrayscaleBitmap(newBitmap));
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHMenu::Entry::SetProc(SMOOTHProcParam, SMOOTHVoid *newProcParam)
{
	proc = (SMOOTHProcType) newProc;
	procParam = newProcParam;

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHMenu::Entry::SetOrientation(SMOOTHInt newOrientation)
{
	if (newOrientation == OR_TOP)		newOrientation = OR_LEFT;
	else if (newOrientation == OR_BOTTOM)	newOrientation = OR_RIGHT;

	orientation = newOrientation;

	return SMOOTH::Success;
}

#endif
