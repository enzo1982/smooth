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
#include <smooth/menu/menubar.h>
#include <smooth/menu/menu.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/menu/popupmenu.h>
#include <smooth/objectproperties.h>
#include <smooth/graphics/surface.h>
#include <smooth/toolwindow.h>

const S::Int	 S::GUI::Menubar::classID = S::Object::RequestClassID();

S::GUI::Menubar::Menubar()
{
	type				= classID;
	objectProperties->orientation	= OR_TOP;
	objectProperties->size.cx	= METRIC_MENUBARHEIGHT;
	objectProperties->size.cy	= METRIC_MENUBARHEIGHT;
	subtype				= WO_SEPARATOR;
	bitmapSize			= METRIC_IBICONSIZE;

	possibleContainers.AddEntry(Window::classID);

	onRegister.Connect(&Menubar::OnRegister, this);
}

S::GUI::Menubar::~Menubar()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Void S::GUI::Menubar::OnRegister()
{
	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		MenuEntry	*operat = (MenuEntry *) assocObjects.GetNthEntry(i);

		if (operat->type == SM_BITMAP && operat->bitmap.GetSize().cy > bitmapSize) SetBitmapSize(operat->bitmap.GetSize().cy);
	}
}

S::Int S::GUI::Menubar::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Window		*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Surface		*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	MenuEntry	*operat;
	Rect		 menubar;
	Rect		 menuentry;
	Rect		 helpmenuentry;
	Point		 doublebar1;
	Point		 doublebar2;
	bool		 firstentry = True;
	bool		 prevbitmap = False;
	bool		 prevtext = False;
	int		 i;

	menubar.left	= objectProperties->pos.x;
	menubar.top	= objectProperties->pos.y;
	menubar.right	= menubar.left + objectProperties->size.cx - 1;
	menubar.bottom	= menubar.top + objectProperties->size.cy - 1;

	if (objectProperties->orientation == OR_BOTTOM)
	{
		menubar.top--;
		menubar.bottom--;
	}

	if (objectProperties->orientation == OR_TOP || objectProperties->orientation == OR_BOTTOM)
	{
		doublebar1.x	= menubar.left + 1;
		doublebar1.y	= menubar.top + 2;
		doublebar2.y	= menubar.bottom;
		doublebar2.x	= doublebar1.x;

		if (wnd->GetIcon() != NIL && objectProperties->orientation == OR_TOP)
		{
			doublebar1.x = doublebar1.x + METRIC_TITLEBARHEIGHT - 2;
			doublebar2.x = doublebar1.x;
		}

		if (Setup::rightToLeft)
		{
			doublebar1.x += 2;
			doublebar2.x += 2;
		}

		surface->Bar(doublebar1, doublebar2, OR_VERT);

		doublebar1.x = doublebar1.x + 2;
		doublebar2.x = doublebar2.x + 2;

		surface->Bar(doublebar1, doublebar2, OR_VERT);
	}

	if (objectProperties->orientation == OR_TOP || objectProperties->orientation == OR_BOTTOM)
	{
		menuentry.top		= menubar.top + METRIC_MBTEXTOFFSETY - 1;
		menuentry.left		= METRIC_MBTEXTOFFSETX;

		if (wnd->GetIcon() != NIL && objectProperties->orientation == OR_TOP) menuentry.left = menuentry.left + METRIC_TITLEBARHEIGHT - 2;

		menuentry.right		= menuentry.left - METRIC_MBENTRYSPACING;
		menuentry.bottom	= menuentry.top + bitmapSize;

		GetSize();

		for (i = 0; i < GetNOfEntries(); i++)
		{
			operat = (MenuEntry *) assocObjects.GetNthEntry(i);

			if (operat->type == SM_SEPARATOR && operat->GetObjectProperties()->orientation == OR_LEFT)
			{
				menuentry.left = menuentry.right + METRIC_MBENTRYSPACING - 1;

				if (prevbitmap) menuentry.left -= 2;

				menuentry.right = menuentry.left;
				doublebar1.x = menuentry.left + 1;
				doublebar1.y = menubar.top + 2;
				doublebar2.x = doublebar1.x;
				doublebar2.y = menubar.bottom;

				if (Setup::rightToLeft)
				{
					doublebar1.x += 2;
					doublebar2.x += 2;
				}

				surface->Bar(doublebar1, doublebar2, OR_VERT);

				doublebar1.x = doublebar1.x + 2;
				doublebar2.x = doublebar2.x + 2;

				surface->Bar(doublebar1, doublebar2, OR_VERT);

				firstentry = True;
			}
			else if (operat->type == SM_TEXT && operat->GetObjectProperties()->orientation == OR_LEFT)
			{
				menuentry.left = menuentry.right + METRIC_MBENTRYSPACING;

				if (prevbitmap) menuentry.left -= 3;

				menuentry.right = menuentry.left + operat->GetObjectProperties()->textSize.cx;

				operat->GetObjectProperties()->pos.x	= menuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= menuentry.top - 1;

				prevtext = True;
			}
			else if (operat->type == SM_BITMAP && operat->GetObjectProperties()->orientation == OR_LEFT)
			{
				if (firstentry)	menuentry.left = menuentry.right + METRIC_MBENTRYSPACING;
				else		menuentry.left = menuentry.right + METRIC_IBENTRYSPACING;

				if (prevtext) menuentry.left += 2;

				if (operat->popup == NIL)
				{
					menuentry.right = menuentry.left - 1 + bitmapSize;
				}
				else
				{
					menuentry.right = menuentry.left + bitmapSize + METRIC_IBARROWSIZEX + 2;

					if (operat->onClick.GetNOfConnectedSlots() > 0) menuentry.right += 2;
				}

				operat->GetObjectProperties()->pos.x	= menuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= menuentry.top - 1;

				prevbitmap = True;
			}

			if (operat->GetObjectProperties()->orientation == OR_LEFT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = False;
				if (operat->type != SM_BITMAP)		prevbitmap = False;
				if (operat->type != SM_TEXT)		prevtext = False;
			}

			if (operat->GetObjectProperties()->checked && !operat->GetObjectProperties()->clicked) surface->Frame(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size), FRAME_UP);
		}

		firstentry = True;
		prevbitmap = False;
		prevtext = False;

		helpmenuentry.top	= menuentry.top;
		helpmenuentry.bottom	= menuentry.bottom;
		helpmenuentry.right	= wnd->GetObjectProperties()->size.cx + 1;
		helpmenuentry.left	= helpmenuentry.right;

		for (i = GetNOfEntries() - 1; i >= 0; i--)
		{
			operat = (MenuEntry *) assocObjects.GetNthEntry(i);

			if (operat->type == SM_SEPARATOR && operat->GetObjectProperties()->orientation == OR_RIGHT)
			{
				helpmenuentry.right = helpmenuentry.left - METRIC_MBENTRYSPACING;
				helpmenuentry.left = helpmenuentry.right;

				doublebar1.x = helpmenuentry.left + 1;
				doublebar1.y = menubar.top + 2;
				doublebar2.x = doublebar1.x;
				doublebar2.y = menubar.bottom;

				if (Setup::rightToLeft)
				{
					doublebar1.x += 2;
					doublebar2.x += 2;
				}

				surface->Bar(doublebar1, doublebar2, OR_VERT);

				doublebar1.x = doublebar1.x + 2;
				doublebar2.x = doublebar2.x + 2;

				surface->Bar(doublebar1, doublebar2, OR_VERT);

				firstentry = True;
				prevbitmap = True;
			}
			else if (operat->type == SM_TEXT  && operat->GetObjectProperties()->orientation == OR_RIGHT)
			{
				helpmenuentry.right = helpmenuentry.left - METRIC_MBENTRYSPACING;

				if (prevbitmap) helpmenuentry.right += 1;
				if (prevtext) helpmenuentry.right += 1;

				helpmenuentry.left = helpmenuentry.right - operat->GetObjectProperties()->textSize.cx;

				operat->GetObjectProperties()->pos.x	= helpmenuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= helpmenuentry.top - 1;
			}
			else if (operat->type == SM_BITMAP && operat->GetObjectProperties()->orientation == OR_RIGHT)
			{
				if (firstentry)	helpmenuentry.right = helpmenuentry.left - METRIC_MBENTRYSPACING + 2;
				else		helpmenuentry.right = helpmenuentry.left - METRIC_IBENTRYSPACING;

				if (prevbitmap) helpmenuentry.right += 1;

				if (operat->popup == NIL)
				{
					helpmenuentry.left = helpmenuentry.right - bitmapSize + 1;
				}
				else
				{
					helpmenuentry.left = helpmenuentry.right - bitmapSize - METRIC_IBARROWSIZEX - 2;
				}

				operat->GetObjectProperties()->pos.x	= helpmenuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= helpmenuentry.top - 1;

				prevtext = True;
			}

			if (operat->GetObjectProperties()->orientation == OR_RIGHT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = False;
				if (operat->type != SM_BITMAP)		prevbitmap = False;
				if (operat->type != SM_TEXT)		prevtext = False;
			}
		}
	}
	else if (objectProperties->orientation == OR_LEFT || objectProperties->orientation == OR_RIGHT)
	{
		menuentry.top		= menubar.top + METRIC_MBTEXTOFFSETX - 8;
		menuentry.left		= METRIC_MBTEXTOFFSETX - 6;
		menuentry.right		= menuentry.left + bitmapSize;
		menuentry.bottom	= menuentry.top - METRIC_MBENTRYSPACING;

		GetSize();

		for (int i = 0; i < GetNOfEntries(); i++)
		{
			operat = (MenuEntry *) assocObjects.GetNthEntry(i);

			if (operat->type == SM_BITMAP && operat->popup == NIL)
			{
				if (firstentry)	menuentry.top = menuentry.bottom + METRIC_MBENTRYSPACING;
				else		menuentry.top = menuentry.bottom + METRIC_IBENTRYSPACING;

				if (prevtext) menuentry.top += 2;

				menuentry.bottom = menuentry.top + bitmapSize + 1;

				operat->GetObjectProperties()->pos.x	= menuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= menuentry.top - 1;

				prevbitmap = True;
			}

			if (operat->GetObjectProperties()->orientation == OR_LEFT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = False;
				if (operat->type != SM_BITMAP)		prevbitmap = False;
				if (operat->type != SM_TEXT)		prevtext = False;
			}
		}

		for (int j = 0; j < GetNOfEntries(); j++)
		{
			operat = (MenuEntry *) assocObjects.GetNthEntry(j);

			if (operat->GetObjectProperties()->orientation == OR_RIGHT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = False;
				if (operat->type != SM_BITMAP)		prevbitmap = False;
				if (operat->type != SM_TEXT)		prevtext = False;
			}
		}
	}

	for (Int j = 0; j < GetNOfEntries(); j++)
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
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	EnterProtectedRegion();

	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		MenuEntry	*object = (MenuEntry *) assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		object->Process(message, wParam, lParam);
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::Menubar::SetBitmapSize(Int nSize)
{
	bitmapSize = nSize;

	objectProperties->size.cx = bitmapSize + 6;
	objectProperties->size.cy = bitmapSize + 6;

	return Success;
}

S::GUI::Surface *S::GUI::Menubar::GetDrawSurface()
{
	if (!registered) return nullSurface;

	return myContainer->GetDrawSurface();
}
