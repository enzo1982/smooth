 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/menu/menuentry.h>
#include <smooth/menu/menubar.h>
#include <smooth/menu/popupmenu.h>
#include <smooth/definitions.h>
#include <smooth/color.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/surface.h>
#include <smooth/objectproperties.h>

S::GUI::MenuEntry::MenuEntry(Int newType, Int newID)
{
	type		= newType;
	id		= newID;

	orientation	= OR_LEFT;

	checked		= False;
	clicked		= False;

	scKey		= 0;
	scFlags		= 0;

	shortcut	= NIL;

	bVar		= NIL;
	iVar		= NIL;

	popup		= NIL;

	onClick.SetParentObject(this);

	possibleContainers.AddEntry(Menu::classID);
}

S::GUI::MenuEntry::~MenuEntry()
{
	if (shortcut != NIL) DeleteObject(shortcut);

	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::MenuEntry::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 textRect;
	Point	 realPos = GetRealPosition();

	switch (message)
	{
		default:
		case SP_PAINT:
			if (type == SM_TEXT)
			{
				textRect.left	= realPos.x + 3;
				textRect.top	= realPos.y + 1;
				textRect.right	= realPos.x + objectProperties->size.cx - 1;
				textRect.bottom	= realPos.y + objectProperties->size.cy - 1;

				surface->SetText(text, textRect, objectProperties->font);
			}

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::MenuEntry::SetText(String newText)
{
	if (newText == NIL) type = (type | SM_TEXT) ^ SM_TEXT;

	text = newText;

	return Success;
}

S::Int S::GUI::MenuEntry::SetTooltip(String newTooltip)
{
	tooltip = newTooltip;

	return Success;
}

S::Int S::GUI::MenuEntry::SetStatusText(String newDescription)
{
	description = newDescription;

	return Success;
}

S::Int S::GUI::MenuEntry::SetShortcut(Int nKey, Int nFlags)
{
	scKey	= nKey;
	scFlags	= nFlags;

	shortcut = new Shortcut(scKey, scFlags);
	shortcut->onKeyDown.Connect(&onClick);

	return Success;
}

S::Int S::GUI::MenuEntry::SetBitmap(const Bitmap &newBitmap)
{
	bitmap	= newBitmap;
	graymap	= newBitmap;

	bitmap.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);

	graymap.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);
	graymap.GrayscaleBitmap();

	return Success;
}

S::Int S::GUI::MenuEntry::SetOrientation(Int newOrientation)
{
	if (newOrientation == OR_TOP)		newOrientation = OR_LEFT;
	else if (newOrientation == OR_BOTTOM)	newOrientation = OR_RIGHT;

	orientation = newOrientation;

	return Success;
}
