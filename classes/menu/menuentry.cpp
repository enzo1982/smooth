 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/menu.h>
#include <smooth/definitions.h>
#include <smooth/stk.h>

S::Menu::Entry::Entry(Int newType, Int newID)
{
	type		= newType;
	id		= newID;

	bitmap		= NIL;
	graymap		= NIL;

	orientation	= OR_LEFT;

	checked		= False;
	clicked		= False;

	proc		= NIL;
	procParam	= NIL;

	bVar		= NIL;
	iVar		= NIL;

	popup		= NIL;
}

S::Menu::Entry::~Entry()
{
}

S::Int S::Menu::Entry::SetText(String newText)
{
	if (newText == NIL) type = (type | SM_TEXT) ^ SM_TEXT;

	text = newText;

	return Success;
}

S::Int S::Menu::Entry::SetTooltip(String newTooltip)
{
	tooltip = newTooltip;

	return Success;
}

S::Int S::Menu::Entry::SetStatusText(String newDescription)
{
	description = newDescription;

	return Success;
}

S::Int S::Menu::Entry::SetBitmap(HBITMAP newBitmap)
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

	return Success;
}

S::Int S::Menu::Entry::SetProc(ProcParam, Void *newProcParam)
{
	proc = (ProcType) newProc;
	procParam = newProcParam;

	return Success;
}

S::Int S::Menu::Entry::SetOrientation(Int newOrientation)
{
	if (newOrientation == OR_TOP)		newOrientation = OR_LEFT;
	else if (newOrientation == OR_BOTTOM)	newOrientation = OR_RIGHT;

	orientation = newOrientation;

	return Success;
}
