 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/menuentry.h>
#include <smooth/definitions.h>
#include <smooth/stk.h>

S::MenuEntry::MenuEntry(Int newType, Int newID)
{
	type		= newType;
	id		= newID;

	bitmap		= NIL;
	graymap		= NIL;

	orientation	= OR_LEFT;

	checked		= False;
	clicked		= False;

	scKey		= 0;
	scFlags		= 0;

	bVar		= NIL;
	iVar		= NIL;

	popup		= NIL;

	onClick.SetParentObject(this);
}

S::MenuEntry::~MenuEntry()
{
}

S::Int S::MenuEntry::SetText(String newText)
{
	if (newText == NIL) type = (type | SM_TEXT) ^ SM_TEXT;

	text = newText;

	return Success;
}

S::Int S::MenuEntry::SetTooltip(String newTooltip)
{
	tooltip = newTooltip;

	return Success;
}

S::Int S::MenuEntry::SetStatusText(String newDescription)
{
	description = newDescription;

	return Success;
}

S::Int S::MenuEntry::SetShortcut(Int nKey, Int nFlags)
{
	scKey	= nKey;
	scFlags	= nFlags;

	return Success;
}

S::Int S::MenuEntry::SetBitmap(HBITMAP newBitmap)
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

S::Int S::MenuEntry::SetOrientation(Int newOrientation)
{
	if (newOrientation == OR_TOP)		newOrientation = OR_LEFT;
	else if (newOrientation == OR_BOTTOM)	newOrientation = OR_RIGHT;

	orientation = newOrientation;

	return Success;
}
