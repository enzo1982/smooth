 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/gui/widgets/multi/menu/menubar.h>
#include <smooth/gui/widgets/multi/menu/menu.h>
#include <smooth/loop.h>
#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/toolwindow.h>

const S::Int	 S::GUI::Menubar::classID = S::Object::RequestClassID();

S::GUI::Menubar::Menubar()
{
	type		= classID;
	orientation	= OR_TOP;
	size		= Size(18, 18);
	subtype		= WO_SEPARATOR;
	bitmapSize	= 12;

	possibleContainers.AddEntry(Window::classID);

	onRegister.Connect(&Menubar::OnRegister, this);
}

S::GUI::Menubar::~Menubar()
{
	if (registered && container != NIL) container->UnregisterObject(this);
}

S::Void S::GUI::Menubar::OnRegister()
{
	for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
	{
		MenuEntry	*operat = (MenuEntry *) assocObjects.GetNthEntry(i);

		if (operat->type == SM_BITMAP && operat->bitmap.GetSize().cy > bitmapSize) SetBitmapSize(operat->bitmap.GetSize().cy);
	}
}

S::Int S::GUI::Menubar::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Window		*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface		*surface = container->GetDrawSurface();

	EnterProtectedRegion();

	MenuEntry	*operat;
	Rect		 menubar;
	Rect		 menuentry;
	Rect		 helpmenuentry;
	bool		 firstentry = True;
	bool		 prevbitmap = False;
	bool		 prevtext = False;
	int		 i;

	menubar.left	= pos.x;
	menubar.top	= pos.y;
	menubar.right	= menubar.left + size.cx - 1;
	menubar.bottom	= menubar.top + size.cy - 1;

	if (orientation == OR_BOTTOM)
	{
		menubar.top--;
		menubar.bottom--;
	}

	if (orientation == OR_TOP || orientation == OR_BOTTOM)
	{
		Point	 p1 = Point(menubar.left + 1 + (Setup::rightToLeft ? 2 : 0), menubar.top + 2);
		Point	 p2 = Point(p1.x, menubar.bottom);

		if (wnd->GetIcon() != NIL && orientation == OR_TOP)
		{
			p1.x += 17;
			p2.x += 17;
		}

		surface->Bar(p1, p2, OR_VERT);

		p1.x += 2;
		p2.x += 2;

		surface->Bar(p1, p2, OR_VERT);
	}

	if (orientation == OR_TOP || orientation == OR_BOTTOM)
	{
		menuentry.top		= menubar.top + 3;
		menuentry.left		= 12;

		if (wnd->GetIcon() != NIL && orientation == OR_TOP) menuentry.left += 17;

		menuentry.right		= menuentry.left - 9;
		menuentry.bottom	= menuentry.top + bitmapSize;

		for (i = 0; i < assocObjects.GetNOfEntries(); i++)
		{
			operat = (MenuEntry *) assocObjects.GetNthEntry(i);

			if (operat->GetOrientation() != OR_LEFT) continue;

			if (operat->type == SM_SEPARATOR)
			{
				menuentry.left = menuentry.right + 8;

				if (prevbitmap) menuentry.left -= 2;

				menuentry.right = menuentry.left;

				operat->size.cy	= menubar.bottom - menubar.top - 2;

				firstentry = True;
			}
			else if (operat->type == SM_TEXT)
			{
				menuentry.left = menuentry.right + 9;

				if (prevbitmap) menuentry.left -= 3;

				menuentry.right = menuentry.left + operat->textSize.cx;

				prevtext = True;
			}
			else if (operat->type == SM_BITMAP)
			{
				if (firstentry)	menuentry.left = menuentry.right + 9;
				else		menuentry.left = menuentry.right + 6;

				if (prevtext) menuentry.left += 2;

				if (operat->popup == NIL)
				{
					menuentry.right = menuentry.left - 1 + bitmapSize;
				}
				else
				{
					menuentry.right = menuentry.left + bitmapSize + 9;

					if (operat->onClick.GetNOfConnectedSlots() > 0) menuentry.right += 2;
				}

				prevbitmap = True;
			}

			operat->pos.x = menuentry.left - 3;
			operat->pos.y = menuentry.top - 1;

			if (operat->type != SM_SEPARATOR)	firstentry = False;
			if (operat->type != SM_BITMAP)		prevbitmap = False;
			if (operat->type != SM_TEXT)		prevtext = False;

			if (operat->checked && !operat->clicked) surface->Frame(Rect(operat->pos, operat->size), FRAME_UP);
		}

		firstentry = True;
		prevbitmap = False;
		prevtext = False;

		helpmenuentry.top	= menuentry.top;
		helpmenuentry.bottom	= menuentry.bottom;
		helpmenuentry.right	= wnd->size.cx + 1;
		helpmenuentry.left	= helpmenuentry.right;

		for (i = assocObjects.GetNOfEntries() - 1; i >= 0; i--)
		{
			operat = (MenuEntry *) assocObjects.GetNthEntry(i);

			if (operat->GetOrientation() != OR_RIGHT) continue;

			if (operat->type == SM_SEPARATOR)
			{
				helpmenuentry.right = helpmenuentry.left - 9;
				helpmenuentry.left = helpmenuentry.right;

				operat->size.cy	= menubar.bottom - menubar.top - 2;

				firstentry = True;
				prevbitmap = True;
			}
			else if (operat->type == SM_TEXT)
			{
				helpmenuentry.right = helpmenuentry.left - 9;

				if (prevbitmap) helpmenuentry.right += 1;
				if (prevtext) helpmenuentry.right += 1;

				helpmenuentry.left = helpmenuentry.right - operat->textSize.cx;
			}
			else if (operat->type == SM_BITMAP)
			{
				if (firstentry)	helpmenuentry.right = helpmenuentry.left - 7;
				else		helpmenuentry.right = helpmenuentry.left - 6;

				if (prevbitmap) helpmenuentry.right += 1;

				if (operat->popup == NIL)	helpmenuentry.left = helpmenuentry.right - bitmapSize + 1;
				else				helpmenuentry.left = helpmenuentry.right - bitmapSize - 9;

				prevtext = True;
			}

			operat->pos.x = helpmenuentry.left - 3;
			operat->pos.y = helpmenuentry.top - 1;

			if (operat->type != SM_SEPARATOR)	firstentry = False;
			if (operat->type != SM_BITMAP)		prevbitmap = False;
			if (operat->type != SM_TEXT)		prevtext = False;
		}
	}
	else if (orientation == OR_LEFT || orientation == OR_RIGHT)
	{
		menuentry.top		= menubar.top + 4;
		menuentry.left		= 6;
		menuentry.right		= menuentry.left + bitmapSize;
		menuentry.bottom	= menuentry.top - 9;

		for (int i = 0; i < assocObjects.GetNOfEntries(); i++)
		{
			operat = (MenuEntry *) assocObjects.GetNthEntry(i);

			if (operat->type == SM_BITMAP && operat->popup == NIL)
			{
				if (firstentry)	menuentry.top = menuentry.bottom + 9;
				else		menuentry.top = menuentry.bottom + 6;

				if (prevtext) menuentry.top += 2;

				menuentry.bottom = menuentry.top + bitmapSize + 1;

				prevbitmap = True;
			}

			operat->pos.x	= menuentry.left - 3;
			operat->pos.y	= menuentry.top - 1;

			if (operat->type != SM_SEPARATOR)	firstentry = False;
			if (operat->type != SM_BITMAP)		prevbitmap = False;
			if (operat->type != SM_TEXT)		prevtext = False;
		}
	}

	for (Int j = 0; j < assocObjects.GetNOfEntries(); j++)
	{
		MenuEntry	*object = (MenuEntry *) assocObjects.GetNthEntry(j);

		if (object == NIL) continue;

		object->Paint(SP_PAINT);
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::Menubar::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Failure;
	if (!active || !visible)	return Success;

	Int	 retVal = Success;

	EnterProtectedRegion();

	for (Int i = 0; i < assocObjects.GetNOfEntries(); i++)
	{
		MenuEntry	*object = (MenuEntry *) assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->Process(message, wParam, lParam) == Break) retVal = Break;
	}

	LeaveProtectedRegion();

	return retVal;
}

S::Int S::GUI::Menubar::SetBitmapSize(Int nSize)
{
	bitmapSize = nSize;

	size.cx = bitmapSize + 6;
	size.cy = bitmapSize + 6;

	return Success;
}
