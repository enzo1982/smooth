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
#include <smooth/metrics.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/surface.h>
#include <smooth/objectproperties.h>

S::GUI::MenuEntry::MenuEntry(Int newType)
{
	type		= newType;

	scKey		= 0;
	scFlags		= 0;

	shortcut	= NIL;

	bVar		= NIL;
	iVar		= NIL;

	popup		= NIL;

	objectProperties->orientation = OR_LEFT;

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

	Point	 realPos = GetRealPosition();

	switch (message)
	{
		default:
		case SP_PAINT:
			if (type == SM_TEXT)
			{
				Rect	 textRect;

				textRect.left	= realPos.x + 3;
				textRect.top	= realPos.y + 1;
				textRect.right	= realPos.x + objectProperties->size.cx - 1;
				textRect.bottom	= realPos.y + objectProperties->size.cy - 1;

				surface->SetText(objectProperties->text, textRect, objectProperties->font);
			}
			else if (type == SM_BITMAP)
			{
				Rect	 rect;

				rect.left	= realPos.x + 2;
				rect.top	= realPos.y + 2;
				rect.right	= realPos.x + bitmap.GetSize().cx + 2;
				rect.bottom	= realPos.y + bitmap.GetSize().cy + 2;

				if (flags & MB_COLOR)	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), rect);
				else			surface->BlitFromBitmap(graymap, Rect(Point(0, 0), graymap.GetSize()), rect);

				if (popup != NIL)
				{
					Point	 p1;
					Point	 p2;

					p1.x = realPos.x + objectProperties->size.cx - METRIC_IBARROWSIZEX - 2 + (Setup::rightToLeft ? 1 : 0);
					p2.x = p1.x + METRIC_IBARROWSIZEX;
					p1.y = realPos.y + (objectProperties->size.cy - METRIC_IBARROWSIZEY) / 2;
					p2.y = p1.y;

					for (Int y = 0; y < METRIC_IBARROWSIZEY; y++)
					{
						p1.x++;
						p2.x--;
						p1.y++;
						p2.y++;

						surface->Line(p1, p2, Setup::TextColor);
					}
				}
			}

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::MenuEntry::SetText(String newText)
{
	if (newText == NIL) type = (type | SM_TEXT) ^ SM_TEXT;

	return Widget::SetText(newText);
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
