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
	popupHandle			= -1;
	subtype				= WO_SEPARATOR;
	bitmapSize			= METRIC_IBICONSIZE;

	possibleContainers.AddEntry(Window::classID);
}

S::GUI::Menubar::~Menubar()
{
	if (registered && myContainer != NIL)
	{
		if (popupHandle != NIL) myContainer->UnregisterObject(GetObject(popupHandle, PopupMenu::classID));

		myContainer->UnregisterObject(this);
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
	Point		 p1;
	Point		 p2;
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
			operat = entries.GetNthEntry(i);

			if (operat->type == SM_SEPARATOR && operat->orientation == OR_LEFT)
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
			else if (operat->type == SM_TEXT && operat->orientation == OR_LEFT)
			{
				menuentry.left = menuentry.right + METRIC_MBENTRYSPACING;

				if (prevbitmap) menuentry.left -= 3;

				menuentry.right = menuentry.left + operat->size;

				operat->GetObjectProperties()->pos.x	= menuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= menuentry.top - 1;
				operat->GetObjectProperties()->size.cx	= menuentry.right + 3 - operat->GetObjectProperties()->pos.x;
				operat->GetObjectProperties()->size.cy	= menuentry.top + METRIC_MBENTRYHEIGHT - operat->GetObjectProperties()->pos.y;

				prevtext = True;
			}
			else if (operat->type == SM_BITMAP && operat->orientation == OR_LEFT && operat->popup == NIL)
			{
				if (firstentry)	menuentry.left = menuentry.right + METRIC_MBENTRYSPACING;
				else		menuentry.left = menuentry.right + METRIC_IBENTRYSPACING;

				if (prevtext) menuentry.left += 2;

				menuentry.left--;
				menuentry.right = menuentry.left + bitmapSize;
				menuentry.top++;
				menuentry.bottom++;

				if (flags & MB_COLOR)	surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), operat->bitmap.GetSize()), menuentry);
				else			surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), operat->graymap.GetSize()), menuentry);

				menuentry.left++;
				menuentry.top--;
				menuentry.bottom--;

				operat->GetObjectProperties()->pos.x	= menuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= menuentry.top - 1;
				operat->GetObjectProperties()->size.cx	= menuentry.right + 1 - operat->GetObjectProperties()->pos.x;
				operat->GetObjectProperties()->size.cy	= menuentry.bottom + 2 - operat->GetObjectProperties()->pos.y;

				prevbitmap = True;
			}
			else if (operat->type == SM_BITMAP && operat->orientation == OR_LEFT && operat->popup != NIL)
			{
				if (firstentry)	menuentry.left = menuentry.right + METRIC_MBENTRYSPACING;
				else		menuentry.left = menuentry.right + METRIC_IBENTRYSPACING;

				if (prevtext) menuentry.left += 2;

				menuentry.left--;
				menuentry.right = menuentry.left + bitmapSize;
				menuentry.top++;
				menuentry.bottom++;

				if (flags & MB_COLOR)	surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), operat->bitmap.GetSize()), menuentry);
				else			surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), operat->graymap.GetSize()), menuentry);

				menuentry.left++;
				menuentry.top--;
				menuentry.bottom--;
				menuentry.right = menuentry.left + bitmapSize + METRIC_IBARROWSIZEX + 2;

				if (operat->onClick.GetNOfConnectedSlots() > 0) menuentry.right += 2;

				p1.x = menuentry.right - METRIC_IBARROWSIZEX - 1 + (Setup::rightToLeft ? 1 : 0);
				p2.x = p1.x + METRIC_IBARROWSIZEX;
				p1.y = menuentry.top + (menuentry.bottom - menuentry.top - METRIC_IBARROWSIZEY) / 2 + 1;
				p2.y = p1.y;

				for (int y = 0; y < METRIC_IBARROWSIZEY; y++)
				{
					p1.x++;
					p2.x--;
					p1.y++;
					p2.y++;
					surface->Line(p1, p2, Setup::TextColor);
				}

				operat->GetObjectProperties()->pos.x	= menuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= menuentry.top - 1;
				operat->GetObjectProperties()->size.cx	= menuentry.right + 1 - operat->GetObjectProperties()->pos.x;
				operat->GetObjectProperties()->size.cy	= menuentry.bottom + 2 - operat->GetObjectProperties()->pos.y;

				prevbitmap = True;
			}

			if (operat->orientation == OR_LEFT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = False;
				if (operat->type != SM_BITMAP)		prevbitmap = False;
				if (operat->type != SM_TEXT)		prevtext = False;
			}

			if (operat->checked && !operat->clicked) surface->Frame(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size), FRAME_UP);
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
			operat = entries.GetNthEntry(i);

			if (operat->type == SM_SEPARATOR && operat->orientation == OR_RIGHT)
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
			else if (operat->type == SM_TEXT  && operat->orientation == OR_RIGHT)
			{
				helpmenuentry.right = helpmenuentry.left - METRIC_MBENTRYSPACING;

				if (prevbitmap) helpmenuentry.right += 1;
				if (prevtext) helpmenuentry.right += 1;

				helpmenuentry.left = helpmenuentry.right - operat->size;

				operat->GetObjectProperties()->pos.x	= helpmenuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= helpmenuentry.top - 1;
				operat->GetObjectProperties()->size.cx	= helpmenuentry.right + 3 - operat->GetObjectProperties()->pos.x;
				operat->GetObjectProperties()->size.cy	= helpmenuentry.top + METRIC_MBENTRYHEIGHT - operat->GetObjectProperties()->pos.y;
			}
			else if (operat->type == SM_BITMAP && operat->orientation == OR_RIGHT && operat->popup == NIL)
			{
				if (firstentry)	helpmenuentry.right = helpmenuentry.left - METRIC_MBENTRYSPACING + 2;
				else		helpmenuentry.right = helpmenuentry.left - METRIC_IBENTRYSPACING;

				if (prevbitmap) helpmenuentry.right += 1;

				helpmenuentry.left = helpmenuentry.right - bitmapSize;
				helpmenuentry.top++;
				helpmenuentry.bottom++;

				if (flags & MB_COLOR)	surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), operat->bitmap.GetSize()), helpmenuentry);
				else			surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), operat->graymap.GetSize()), helpmenuentry);

				helpmenuentry.top--;
				helpmenuentry.bottom--;
				helpmenuentry.left++;

				operat->GetObjectProperties()->pos.x	= helpmenuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= helpmenuentry.top - 1;
				operat->GetObjectProperties()->size.cx	= helpmenuentry.right + 1 - operat->GetObjectProperties()->pos.x;
				operat->GetObjectProperties()->size.cy	= helpmenuentry.bottom + 2 - operat->GetObjectProperties()->pos.y;

				prevtext = True;
			}
			else if (operat->type == SM_BITMAP && operat->orientation == OR_RIGHT && operat->popup != NIL)
			{
				if (firstentry)	helpmenuentry.right = helpmenuentry.left - METRIC_MBENTRYSPACING + 2;
				else		helpmenuentry.right = helpmenuentry.left - METRIC_IBENTRYSPACING;

				if (prevbitmap) helpmenuentry.right += 1;

				helpmenuentry.right -= (METRIC_IBARROWSIZEX + 3);
				helpmenuentry.left = helpmenuentry.right - bitmapSize;
				helpmenuentry.top++;
				helpmenuentry.bottom++;

				if (flags & MB_COLOR)	surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), operat->bitmap.GetSize()), helpmenuentry);
				else			surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), operat->graymap.GetSize()), helpmenuentry);

				helpmenuentry.top--;
				helpmenuentry.bottom--;
				helpmenuentry.right += (METRIC_IBARROWSIZEX + 3);
				helpmenuentry.left = helpmenuentry.right - bitmapSize - METRIC_IBARROWSIZEX - 2;

				p1.x = helpmenuentry.right - METRIC_IBARROWSIZEX - 1 + (Setup::rightToLeft ? 1 : 0);
				p2.x = p1.x + METRIC_IBARROWSIZEX;
				p1.y = helpmenuentry.top + (helpmenuentry.bottom - helpmenuentry.top - METRIC_IBARROWSIZEY) / 2 + 1;
				p2.y = p1.y;

				for (int y = 0; y < METRIC_IBARROWSIZEY; y++)
				{
					p1.x++;
					p2.x--;
					p1.y++;
					p2.y++;
					surface->Line(p1, p2, Setup::TextColor);
				}

				operat->GetObjectProperties()->pos.x	= helpmenuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= helpmenuentry.top - 1;
				operat->GetObjectProperties()->size.cx	= helpmenuentry.right + 1 - operat->GetObjectProperties()->pos.x;
				operat->GetObjectProperties()->size.cy	= helpmenuentry.bottom + 2 - operat->GetObjectProperties()->pos.y;

				prevtext = True;
			}

			if (operat->orientation == OR_RIGHT)
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
			operat = entries.GetNthEntry(i);

			if (operat->type == SM_BITMAP && operat->popup == NIL)
			{
				if (firstentry)	menuentry.top = menuentry.bottom + METRIC_MBENTRYSPACING;
				else		menuentry.top = menuentry.bottom + METRIC_IBENTRYSPACING;

				if (prevtext) menuentry.top += 2;

				menuentry.bottom = menuentry.top + bitmapSize + 1;

				menuentry.top++;

				if (flags & MB_COLOR)	surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), operat->bitmap.GetSize()), menuentry);
				else			surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), operat->graymap.GetSize()), menuentry);

				menuentry.top--;

				operat->GetObjectProperties()->pos.x	= menuentry.left - 3;
				operat->GetObjectProperties()->pos.y	= menuentry.top - 1;
				operat->GetObjectProperties()->size.cx	= menuentry.right + 1 - operat->GetObjectProperties()->pos.x;
				operat->GetObjectProperties()->size.cy	= menuentry.bottom + 2 - operat->GetObjectProperties()->pos.y;

				prevbitmap = True;
			}

			if (operat->orientation == OR_LEFT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = False;
				if (operat->type != SM_BITMAP)		prevbitmap = False;
				if (operat->type != SM_TEXT)		prevtext = False;
			}
		}

		for (int j = 0; j < GetNOfEntries(); j++)
		{
			operat = entries.GetNthEntry(j);

			if (operat->orientation == OR_RIGHT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = False;
				if (operat->type != SM_BITMAP)		prevbitmap = False;
				if (operat->type != SM_TEXT)		prevtext = False;
			}
		}
	}

	for (Int j = 0; j < GetNOfEntries(); j++)
	{
		MenuEntry	*object = entries.GetNthEntry(j);

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

	Window		*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	EnterProtectedRegion();

	Surface		*surface = myContainer->GetDrawSurface();
	MenuEntry	*operat;
	Rect		 bmprect;
	Int		 retVal = Success;
	Int		 i;

	String		 newStatus;
	Bool		 updateStatus = False;
	Bool		 setOldStatus = False;

	switch (message)
	{
		case WM_KILLFOCUS:
			for (i = 0; i < GetNOfEntries(); i++)
			{
				MenuEntry	*operat = entries.GetNthEntry(i);

				if ((operat->popup != NIL) && operat->clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL))
				{
					Bool	 destroyPopup = True;

					if (Window::GetWindow((HWND) wParam) != NIL) if (Window::GetWindow((HWND) wParam)->handle >= ((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->handle) destroyPopup = False;

					if (destroyPopup)
					{
						((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->Hide();

						wnd->UnregisterObject(GetObject(popupHandle, PopupMenu::classID));
						DeleteObject(GetObject(popupHandle, PopupMenu::classID));

						popupHandle = -1;

						break;
					}
				}
			}

			break;
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			for (i = 0; i < GetNOfEntries(); i++)
			{
				MenuEntry	*operat = entries.GetNthEntry(i);

				if ((operat->popup != NIL) && operat->clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL))
				{
					((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->Hide();

					DeleteObject(GetObject(popupHandle, PopupMenu::classID));

					popupHandle = -1;

					break;
				}
			}

			for (i = 0; i < GetNOfEntries(); i++)
			{
				operat = entries.GetNthEntry(i);

				if (operat->checked && (operat->type != SM_SEPARATOR))
				{
					wnd->Process(SM_LOOSEFOCUS, handle, 0);

					if (!operat->clicked)
					{
						operat->clicked = True;
						surface->Frame(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size), FRAME_DOWN);
					}
					else
					{
						operat->clicked = False;

						if (!wnd->IsMouseOn(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size)))	surface->Box(Rect(operat->GetObjectProperties()->pos, Size(operat->GetObjectProperties()->size.cx + 1, operat->GetObjectProperties()->size.cy + 1)), Setup::BackgroundColor, OUTLINED);
						else													surface->Frame(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size), FRAME_UP);
					}

					if (operat->clicked && operat->popup != NIL)
					{
						Rect	 popupFrame = Rect(Point(operat->GetObjectProperties()->pos.x + operat->GetObjectProperties()->size.cx - METRIC_IBARROWSIZEX - 4, operat->GetObjectProperties()->pos.y), Size(METRIC_IBARROWSIZEX + 4, operat->GetObjectProperties()->size.cy));

						if (operat->onClick.GetNOfConnectedSlots() == 0 || wnd->IsMouseOn(popupFrame))
						{
							PopupMenu *popupMenu = new PopupMenu(operat->popup);

							popupHandle = popupMenu->handle;

							if (operat->orientation == OR_LEFT)
							{
								popupMenu->GetObjectProperties()->pos.x = operat->GetObjectProperties()->pos.x - 1;
							}
							else if (operat->orientation == OR_RIGHT)
							{
								popupMenu->realMenu->GetSize();
								popupMenu->GetObjectProperties()->pos.x = operat->GetObjectProperties()->pos.x + operat->GetObjectProperties()->size.cx + 1 - popupMenu->realMenu->popupsize.cx;
							}

							popupMenu->GetObjectProperties()->pos.y = operat->GetObjectProperties()->pos.y + operat->GetObjectProperties()->size.cy + 2;
							popupMenu->onClick.Connect(&Menubar::PopupProc, this);

							wnd->RegisterObject(popupMenu);
						}
					}

					retVal = Break;
				}
			}

			break;
		case SM_LBUTTONUP:
			for (i = 0; i < GetNOfEntries(); i++)
			{
				operat = entries.GetNthEntry(i);

				if ((operat->popup != NIL) && operat->clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL)) if (((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->IsVisible()) continue;

				if (operat->type == SM_TEXT && operat->clicked)
				{
					operat->clicked = False;
					surface->Frame(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size), FRAME_UP);

					if (operat->popup == NIL && operat->bVar == NIL && operat->iVar == NIL)
					{
						operat->checked = False;

						if (operat->description != NIL) wnd->SetStatusText(backupStatusText);

						surface->Box(Rect(operat->GetObjectProperties()->pos, Size(operat->GetObjectProperties()->size.cx + 1, operat->GetObjectProperties()->size.cy + 1)), Setup::BackgroundColor, OUTLINED);

						operat->onClick.Emit();

						Process(SM_MOUSEMOVE, 0, 0);

						retVal = Break;

						break;
					}

					retVal = Break;
				}
				else if (operat->type == SM_BITMAP && operat->clicked)
				{
					operat->clicked = False;
					surface->Frame(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size), FRAME_UP);

					if ((operat->popup == NIL || operat->onClick.GetNOfConnectedSlots() > 0) && operat->bVar == NIL && operat->iVar == NIL)
					{
						operat->checked = False;

						if (operat->description != NIL) wnd->SetStatusText(backupStatusText);

						surface->Box(Rect(operat->GetObjectProperties()->pos, Size(operat->GetObjectProperties()->size.cx + 1, operat->GetObjectProperties()->size.cy + 1)), Setup::BackgroundColor, OUTLINED);

						bmprect = Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size);
						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + bitmapSize;
						bmprect.bottom	= bmprect.top + bitmapSize;

						if (flags & MB_COLOR)	surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), operat->bitmap.GetSize()), bmprect);
						else			surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), operat->graymap.GetSize()), bmprect);

						if (operat->onClick.GetNOfConnectedSlots() > 0 && operat->popup != NIL)
						{
							Point	 p1 = Point(operat->GetObjectProperties()->pos.x + operat->GetObjectProperties()->size.cx - METRIC_IBARROWSIZEX - 5, operat->GetObjectProperties()->pos.y + 1);
							Point	 p2 = Point(operat->GetObjectProperties()->pos.x + operat->GetObjectProperties()->size.cx - METRIC_IBARROWSIZEX - 5, operat->GetObjectProperties()->pos.y + operat->GetObjectProperties()->size.cy);

							surface->Line(p1, p2, Setup::BackgroundColor);

							p1.x++;
							p2.x++;

							surface->Line(p1, p2, Setup::BackgroundColor);
						}

						operat->onClick.Emit();

						Process(SM_MOUSEMOVE, 0, 0);

						retVal = Break;

						break;
					}

					retVal = Break;
				}
			}

			break;
		case SM_MOUSEMOVE:
			for (i = 0; i < GetNOfEntries(); i++)
			{
				operat = entries.GetNthEntry(i);

				if ((operat->popup != NIL) && operat->clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL)) if (((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->IsVisible()) continue;

				if (operat->type == SM_TEXT)
				{
					if (wnd->IsMouseOn(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size)) && !operat->checked)
					{
						operat->checked = True;

						if (operat->description != NIL)
						{
							newStatus = operat->description;

							updateStatus = True;
						}

						surface->Frame(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size), FRAME_UP);
					}
					else if (!wnd->IsMouseOn(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size)) && operat->checked)
					{
						operat->checked = False;
						operat->clicked = False;

						if (operat->description != NIL) setOldStatus = True;

						surface->Box(Rect(operat->GetObjectProperties()->pos, Size(operat->GetObjectProperties()->size.cx + 1, operat->GetObjectProperties()->size.cy + 1)), Setup::BackgroundColor, OUTLINED);
					}
				}
				else if (operat->type == SM_BITMAP)
				{
					if (wnd->IsMouseOn(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size)) && !operat->checked)
					{
						operat->checked = True;

						if (operat->description != NIL)
						{
							newStatus = operat->description;

							updateStatus = True;
						}

						bmprect = Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size);
						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + bitmapSize;
						bmprect.bottom	= bmprect.top + bitmapSize;

						surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), operat->bitmap.GetSize()), bmprect);
						surface->Frame(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size), FRAME_UP);

						if (operat->onClick.GetNOfConnectedSlots() > 0 && operat->popup != NIL)
						{
							Point	 p1 = Point(operat->GetObjectProperties()->pos.x + operat->GetObjectProperties()->size.cx - METRIC_IBARROWSIZEX - 5 + (Setup::rightToLeft ? 2 : 0), operat->GetObjectProperties()->pos.y + 1);
							Point	 p2 = Point(operat->GetObjectProperties()->pos.x + operat->GetObjectProperties()->size.cx - METRIC_IBARROWSIZEX - 5 + (Setup::rightToLeft ? 2 : 0), operat->GetObjectProperties()->pos.y + operat->GetObjectProperties()->size.cy - 1);

							surface->Bar(p1, p2, OR_VERT);
						}
					}
					else if (!wnd->IsMouseOn(Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size)) && operat->checked)
					{
						operat->checked = False;
						operat->clicked = False;

						if (operat->description != NIL) setOldStatus = True;

						surface->Box(Rect(operat->GetObjectProperties()->pos, Size(operat->GetObjectProperties()->size.cx + 1, operat->GetObjectProperties()->size.cy + 1)), Setup::BackgroundColor, OUTLINED);

						bmprect = Rect(operat->GetObjectProperties()->pos, operat->GetObjectProperties()->size);
						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + bitmapSize;
						bmprect.bottom	= bmprect.top + bitmapSize;

						if (flags & MB_COLOR)	surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), operat->bitmap.GetSize()), bmprect);
						else			surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), operat->graymap.GetSize()), bmprect);

						if (operat->onClick.GetNOfConnectedSlots() > 0 && operat->popup != NIL)
						{
							Point	 p1 = Point(operat->GetObjectProperties()->pos.x + operat->GetObjectProperties()->size.cx - METRIC_IBARROWSIZEX - 5 + (Setup::rightToLeft ? 1 : 0), operat->GetObjectProperties()->pos.y + 1);
							Point	 p2 = Point(operat->GetObjectProperties()->pos.x + operat->GetObjectProperties()->size.cx - METRIC_IBARROWSIZEX - 5 + (Setup::rightToLeft ? 1 : 0), operat->GetObjectProperties()->pos.y + operat->GetObjectProperties()->size.cy);

							surface->Line(p1, p2, Setup::BackgroundColor);

							p1.x++;
							p2.x++;

							surface->Line(p1, p2, Setup::BackgroundColor);
						}
					}
				}
			}

			break;
	}

	if (setOldStatus && !updateStatus) wnd->SetStatusText(backupStatusText);

	if (updateStatus)
	{
		if (!setOldStatus) backupStatusText = wnd->GetStatusText();

		wnd->SetStatusText(newStatus);
	}

	LeaveProtectedRegion();

	return retVal;
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

S::Void S::GUI::Menubar::PopupProc()
{
	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		MenuEntry	*operat = entries.GetNthEntry(i);

		if ((operat->popup != NIL) && operat->clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL))
		{
			((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->Hide();

			myContainer->UnregisterObject(GetObject(popupHandle, PopupMenu::classID));
			DeleteObject(GetObject(popupHandle, PopupMenu::classID));

			popupHandle = -1;

			break;
		}
	}
}
