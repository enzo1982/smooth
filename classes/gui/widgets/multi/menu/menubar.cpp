 /* The smooth Class Library
  * Copyright (C) 1998-2008 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/menubar.h>
#include <smooth/gui/widgets/multi/menu/menubarentry.h>
#include <smooth/gui/window/window.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Menubar::classID = S::Object::RequestClassID();

S::GUI::Menubar::Menubar()
{
	type		= classID;
	orientation	= OR_TOP;
	subtype		= WO_SEPARATOR;

	SetSize(Size(18, 18));
}

S::GUI::Menubar::~Menubar()
{
}

S::GUI::MenuEntry *S::GUI::Menubar::AddEntry(const String &text, const Bitmap &bitmap, PopupMenu *popupMenu, Bool *bVar, Int *iVar, Int iCode)
{
	MenuEntry	*newEntry = new MenubarEntry(text, bitmap, popupMenu, bVar, iVar, iCode);

	Add(newEntry);

	if (GetHeight() < newEntry->GetHeight() + 2) SetHeight(newEntry->GetHeight() + 2);
	if (GetWidth() < newEntry->GetWidth() + 3) SetWidth(newEntry->GetWidth() + 3);

	return newEntry;
}

S::Int S::GUI::Menubar::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Window	*window	 = container->GetContainerWindow();
	Surface	*surface = container->GetDrawSurface();
	Rect	 menubar = Rect(GetRealPosition(), GetSize() - Size(1, 1));

	if (orientation == OR_TOP || orientation == OR_BOTTOM)
	{
		Int	 nextXPosLeft	= 7 + (window->GetIcon() != NIL ? 17 : 0);
		Int	 nextXPosRight	= GetSize().cx - 1;
		Int	 highestEntry	= 0;

		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			MenuEntry	*entry = (MenuEntry *) GetNthObject(i);

			if (entry->GetOrientation() == OR_TOP) entry->SetOrientation(OR_LEFT);

			if (entry->GetOrientation() == OR_LEFT)
			{
				entry->SetPosition(Point(nextXPosLeft, 2));

				nextXPosLeft += entry->GetWidth() + 2;

				if (highestEntry < entry->GetHeight()) highestEntry = entry->GetHeight();
			}
		}

		for (Int j = GetNOfObjects() - 1; j >= 0; j--)
		{
			MenuEntry	*entry = (MenuEntry *) GetNthObject(j);

			if (entry->GetOrientation() == OR_BOTTOM) entry->SetOrientation(OR_RIGHT);

			if (entry->GetOrientation() == OR_RIGHT)
			{
				entry->SetPosition(Point(nextXPosRight - entry->GetWidth(), 2));

				nextXPosRight -= entry->GetWidth() + 2;

				if (highestEntry < entry->GetHeight()) highestEntry = entry->GetHeight();
			}
		}

		SetHeight(highestEntry + 2);

		for (Int k = 0; k < GetNOfObjects(); k++)
		{
			MenuEntry	*entry = (MenuEntry *) GetNthObject(k);

			if (entry->GetText() == NIL && entry->GetBitmap() == NIL) entry->SetHeight(GetHeight() - 3);
		}
	}
	else if (orientation == OR_LEFT || orientation == OR_RIGHT)
	{
		Int	 nextYPos	= 1;
		Int	 widestEntry	= 0;

		for (Int i = 0; i < GetNOfObjects(); i++)
		{
			MenuEntry	*entry = (MenuEntry *) GetNthObject(i);

			if (entry->GetOrientation() == OR_LEFT) entry->SetOrientation(OR_TOP);

			if (entry->GetOrientation() == OR_TOP)
			{
				entry->SetPosition(Point(1, nextYPos));

				nextYPos += entry->GetHeight() + 2;

				if (widestEntry < entry->GetWidth()) widestEntry = entry->GetWidth();
			}
		}

		for (Int j = 0; j < GetNOfObjects(); j++)
		{
			MenuEntry	*entry = (MenuEntry *) GetNthObject(j);

			if (entry->GetOrientation() == OR_RIGHT) entry->SetOrientation(OR_BOTTOM);

			if (entry->GetOrientation() == OR_BOTTOM)
			{
				entry->SetPosition(Point(1, nextYPos));

				nextYPos += entry->GetHeight() + 2;

				if (widestEntry < entry->GetWidth()) widestEntry = entry->GetWidth();
			}
		}

		SetWidth(widestEntry + 3);

		for (Int k = 0; k < GetNOfObjects(); k++)
		{
			MenuEntry	*entry = (MenuEntry *) GetNthObject(k);

			if (entry->GetText() == NIL && entry->GetBitmap() == NIL) entry->SetWidth(GetWidth() - 3);
		}
	}

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			if (orientation == OR_TOP || orientation == OR_BOTTOM)
			{
				if (orientation == OR_BOTTOM) { menubar.top--; menubar.bottom--; }

				Point	 p1 = Point(menubar.left + 1, menubar.top + 2);
				Point	 p2 = Point(p1.x, menubar.bottom);

				if (window->GetIcon() != NIL && orientation == OR_TOP)
				{
					p1.x += 17;
					p2.x += 17;
				}

				surface->Bar(p1, p2, OR_VERT);

				p1.x += 2;
				p2.x += 2;

				surface->Bar(p1, p2, OR_VERT);
			}

			break;
	}

	return Widget::Paint(message);
}
