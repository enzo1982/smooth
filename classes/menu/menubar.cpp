 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/definitions.h>
#include <smooth/menubar.h>
#include <smooth/menu.h>
#include <smooth/toolkit.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/popupmenu.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_MENUBAR = S::Object::RequestObjectID();

S::GUI::Menubar::Menubar()
{
	type				= OBJ_MENUBAR;
	objectProperties->orientation	= OR_TOP;
	objectProperties->size.cx	= METRIC_MENUBARHEIGHT;
	objectProperties->size.cy	= METRIC_MENUBARHEIGHT;
	popupHandle			= -1;
	subtype				= WO_SEPARATOR;
	style				= MB_GRAYSCALE;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

S::GUI::Menubar::~Menubar()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Menubar::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Window		*wnd = (Window *) myContainer->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Surface		*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Menu::Entry	*operat;
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

		if (wnd->icon != NIL && objectProperties->orientation == OR_TOP)
		{
			doublebar1.x = doublebar1.x + METRIC_TITLEBARHEIGHT - 2;
			doublebar2.x = doublebar1.x;
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

		if (wnd->icon != NIL && objectProperties->orientation == OR_TOP) menuentry.left = menuentry.left + METRIC_TITLEBARHEIGHT - 2;

		menuentry.right		= menuentry.left - METRIC_MBENTRYSPACING;
		menuentry.bottom	= menuentry.top + METRIC_IBICONSIZE;

		GetSize();

		for (i = 0; i < nOfEntries; i++)
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

				menuentry.bottom += 1;
				surface->SetText(operat->text, menuentry, objectProperties->font);
				menuentry.bottom -= 1;

				operat->rect.left	= menuentry.left - 3;
				operat->rect.top	= menuentry.top - 1;
				operat->rect.right	= menuentry.right + 3;
				operat->rect.bottom	= menuentry.top + METRIC_MBENTRYHEIGHT;

				prevtext = True;
			}
			else if (operat->type == SM_BITMAP && operat->orientation == OR_LEFT && operat->popup == NIL)
			{
				if (firstentry)	menuentry.left = menuentry.right + METRIC_MBENTRYSPACING;
				else		menuentry.left = menuentry.right + METRIC_IBENTRYSPACING;

				if (prevtext) menuentry.left += 2;

				menuentry.left--;
				menuentry.right = menuentry.left + METRIC_IBICONSIZE;
				menuentry.top++;
				menuentry.bottom++;

				if (style == MB_GRAYSCALE)	surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->graymap), GetBitmapSizeY(operat->graymap))), menuentry);
				else				surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->bitmap), GetBitmapSizeY(operat->bitmap))), menuentry);

				menuentry.left++;
				menuentry.top--;
				menuentry.bottom--;

				operat->rect.left	= menuentry.left - 3;
				operat->rect.top	= menuentry.top - 1;
				operat->rect.right	= menuentry.right + 1;
				operat->rect.bottom	= menuentry.bottom + 2;

				prevbitmap = True;
			}
			else if (operat->type == SM_BITMAP && operat->orientation == OR_LEFT && operat->popup != NIL)
			{
				if (firstentry)	menuentry.left = menuentry.right + METRIC_MBENTRYSPACING;
				else		menuentry.left = menuentry.right + METRIC_IBENTRYSPACING;

				if (prevtext) menuentry.left += 2;

				menuentry.left--;
				menuentry.right = menuentry.left + METRIC_IBICONSIZE;
				menuentry.top++;
				menuentry.bottom++;

				if (style == MB_GRAYSCALE)	surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->graymap), GetBitmapSizeY(operat->graymap))), menuentry);
				else				surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->bitmap), GetBitmapSizeY(operat->bitmap))), menuentry);

				menuentry.left++;
				menuentry.top--;
				menuentry.bottom--;
				menuentry.right = menuentry.left + METRIC_IBICONSIZE + METRIC_IBARROWSIZEX + 2;

				if (operat->onClick.GetNOfConnectedSlots() > 0) menuentry.right += 2;

				p1.x = menuentry.right - METRIC_IBARROWSIZEX - 1;
				p2.x = p1.x + METRIC_IBARROWSIZEX;
				p1.y = menuentry.top + METRIC_IBARROWOFFSETY + 1;
				p2.y = p1.y;

				for (int y = 0; y < METRIC_IBARROWSIZEY; y++)
				{
					p1.x++;
					p2.x--;
					p1.y++;
					p2.y++;
					surface->Line(p1, p2, Setup::TextColor);
				}

				operat->rect.left	= menuentry.left - 3;
				operat->rect.top	= menuentry.top - 1;
				operat->rect.right	= menuentry.right + 1;
				operat->rect.bottom	= menuentry.bottom + 2;

				prevbitmap = True;
			}

			if (operat->orientation == OR_LEFT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = False;
				if (operat->type != SM_BITMAP)		prevbitmap = False;
				if (operat->type != SM_TEXT)		prevtext = False;
			}

			if (operat->checked && !operat->clicked) surface->Frame(operat->rect, FRAME_UP);
		}

		firstentry = True;
		prevbitmap = False;
		prevtext = False;

		helpmenuentry.top	= menuentry.top;
		helpmenuentry.bottom	= menuentry.bottom;
		helpmenuentry.right	= wnd->GetObjectProperties()->size.cx + 1;
		helpmenuentry.left	= helpmenuentry.right;

		for (i = nOfEntries - 1; i >= 0; i--)
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

				helpmenuentry.bottom += 1;
				surface->SetText(operat->text, helpmenuentry, objectProperties->font);
				helpmenuentry.bottom -= 1;

				operat->rect.left	= helpmenuentry.left - 3;
				operat->rect.top	= helpmenuentry.top - 1;
				operat->rect.right	= helpmenuentry.right + 3;
				operat->rect.bottom	= helpmenuentry.top + METRIC_MBENTRYHEIGHT;
			}
			else if (operat->type == SM_BITMAP && operat->orientation == OR_RIGHT && operat->popup == NIL)
			{
				if (firstentry)	helpmenuentry.right = helpmenuentry.left - METRIC_MBENTRYSPACING + 2;
				else		helpmenuentry.right = helpmenuentry.left - METRIC_IBENTRYSPACING;

				if (prevbitmap) helpmenuentry.right += 1;

				helpmenuentry.left = helpmenuentry.right - METRIC_IBICONSIZE;
				helpmenuentry.top++;
				helpmenuentry.bottom++;

				if (style == MB_GRAYSCALE)	surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->graymap), GetBitmapSizeY(operat->graymap))), helpmenuentry);
				else				surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->bitmap), GetBitmapSizeY(operat->bitmap))), helpmenuentry);

				helpmenuentry.top--;
				helpmenuentry.bottom--;
				helpmenuentry.left++;

				operat->rect.left	= helpmenuentry.left - 3;
				operat->rect.top	= helpmenuentry.top - 1;
				operat->rect.right	= helpmenuentry.right + 1;
				operat->rect.bottom	= helpmenuentry.bottom + 2;

				prevtext = True;
			}
			else if (operat->type == SM_BITMAP && operat->orientation == OR_RIGHT && operat->popup != NIL)
			{
				if (firstentry)	helpmenuentry.right = helpmenuentry.left - METRIC_MBENTRYSPACING + 2;
				else		helpmenuentry.right = helpmenuentry.left - METRIC_IBENTRYSPACING;

				if (prevbitmap) helpmenuentry.right += 1;

				helpmenuentry.right -= (METRIC_IBARROWSIZEX + 3);
				helpmenuentry.left = helpmenuentry.right - METRIC_IBICONSIZE;
				helpmenuentry.top++;
				helpmenuentry.bottom++;

				if (style == MB_GRAYSCALE)	surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->graymap), GetBitmapSizeY(operat->graymap))), helpmenuentry);
				else				surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->bitmap), GetBitmapSizeY(operat->bitmap))), helpmenuentry);

				helpmenuentry.top--;
				helpmenuentry.bottom--;
				helpmenuentry.right += (METRIC_IBARROWSIZEX + 3);
				helpmenuentry.left = helpmenuentry.right - METRIC_IBICONSIZE - METRIC_IBARROWSIZEX - 2;

				p1.x = helpmenuentry.right - METRIC_IBARROWSIZEX - 1;
				p2.x = p1.x + METRIC_IBARROWSIZEX;
				p1.y = helpmenuentry.top + METRIC_IBARROWOFFSETY + 1;
				p2.y = p1.y;

				for (int y = 0; y < METRIC_IBARROWSIZEY; y++)
				{
					p1.x++;
					p2.x--;
					p1.y++;
					p2.y++;
					surface->Line(p1, p2, Setup::TextColor);
				}

				operat->rect.left	= helpmenuentry.left - 3;
				operat->rect.top	= helpmenuentry.top - 1;
				operat->rect.right	= helpmenuentry.right + 1;
				operat->rect.bottom	= helpmenuentry.bottom + 2;

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
		menuentry.right		= menuentry.left + METRIC_IBICONSIZE;
		menuentry.bottom	= menuentry.top - METRIC_MBENTRYSPACING;

		GetSize();

		for (int i = 0; i < nOfEntries; i++)
		{
			operat = entries.GetNthEntry(i);

			if (operat->type == SM_BITMAP && operat->popup == NIL)
			{
				if (firstentry)	menuentry.top = menuentry.bottom + METRIC_MBENTRYSPACING;
				else		menuentry.top = menuentry.bottom + METRIC_IBENTRYSPACING;

				if (prevtext) menuentry.top += 2;

				menuentry.bottom = menuentry.top + METRIC_IBICONSIZE + 1;

				menuentry.top++;

				if (style == MB_GRAYSCALE)	surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->graymap), GetBitmapSizeY(operat->graymap))), menuentry);
				else				surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->bitmap), GetBitmapSizeY(operat->bitmap))), menuentry);

				menuentry.top--;

				operat->rect.left	= menuentry.left - 3;
				operat->rect.top	= menuentry.top - 1;
				operat->rect.right	= menuentry.right + 1;
				operat->rect.bottom	= menuentry.bottom + 2;

				prevbitmap = True;
			}

			if (operat->orientation == OR_LEFT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = False;
				if (operat->type != SM_BITMAP)		prevbitmap = False;
				if (operat->type != SM_TEXT)		prevtext = False;
			}
		}

		for (int j = 0; j < nOfEntries; j++)
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
	Menu::Entry	*operat;
	Rect		 bmprect;
	Int		 retVal = Success;
	bool		 cont = False;
	Int		 i;
	PopupMenu	*popupMenu = NIL;

	String		 newStatus;
	Bool		 updateStatus = False;
	Bool		 setOldStatus = False;

	switch (message)
	{
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if ((operat->popup != NIL) && operat->clicked && (GetObject(popupHandle, OBJ_POPUP) != NIL)) if (((PopupMenu *) GetObject(popupHandle, OBJ_POPUP))->IsVisible() && !cont) continue;

				if (operat->checked && (operat->type != SM_SEPARATOR))
				{
					wnd->Process(SM_LOOSEFOCUS, handle, 0);

					if (!operat->clicked)
					{
						operat->clicked = True;
						surface->Frame(operat->rect, FRAME_DOWN);
					}
					else
					{
						operat->clicked = False;

						if (!wnd->IsMouseOn(operat->rect))	surface->Box(Rect(Point(operat->rect.left, operat->rect.top), Size(operat->rect.right - operat->rect.left + 1, operat->rect.bottom - operat->rect.top + 1)), Setup::BackgroundColor, OUTLINED);
						else					surface->Frame(operat->rect, FRAME_UP);
					}

					if (operat->clicked && operat->popup != NIL)
					{
						Rect	 popupFrame = Rect(Point(operat->rect.right - METRIC_IBARROWSIZEX - 4, operat->rect.top), Size(METRIC_IBARROWSIZEX + 4, operat->rect.bottom - operat->rect.top));

						if (operat->onClick.GetNOfConnectedSlots() == 0 || wnd->IsMouseOn(popupFrame))
						{
							popupMenu = new PopupMenu(operat->popup);

							popupHandle = popupMenu->handle;

							if (operat->orientation == OR_LEFT)
							{
								popupMenu->GetObjectProperties()->pos.x = operat->rect.left - 1;
							}
							else if (operat->orientation == OR_RIGHT)
							{
								popupMenu->realMenu->GetSize();
								popupMenu->GetObjectProperties()->pos.x = operat->rect.right + 1 - popupMenu->realMenu->popupsize.cx;
							}

							popupMenu->GetObjectProperties()->pos.y = operat->rect.bottom + 2;

							wnd->RegisterObject(popupMenu);

							cont = True;
						}
					}

					retVal = Break;
				}
			}

			break;
		case SM_LBUTTONUP:
			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if ((operat->popup != NIL) && operat->clicked && (GetObject(popupHandle, OBJ_POPUP) != NIL)) if (((PopupMenu *) GetObject(popupHandle, OBJ_POPUP))->IsVisible()) continue;

				if (operat->type == SM_TEXT && operat->clicked)
				{
					operat->clicked = False;
					surface->Frame(operat->rect, FRAME_UP);

					if (operat->popup == NIL && operat->bVar == NIL && operat->iVar == NIL)
					{
						operat->checked = False;

						if (operat->description != NIL) wnd->SetStatusText(backupStatusText);

						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::BackgroundColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;

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
					surface->Frame(operat->rect, FRAME_UP);

					if ((operat->popup == NIL || operat->onClick.GetNOfConnectedSlots() > 0) && operat->bVar == NIL && operat->iVar == NIL)
					{
						operat->checked = False;

						if (operat->description != NIL) wnd->SetStatusText(backupStatusText);

						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::BackgroundColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;

						bmprect = operat->rect;
						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + METRIC_IBICONSIZE;
						bmprect.bottom	= bmprect.top + METRIC_IBICONSIZE;

						if (style == MB_GRAYSCALE)	surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->graymap), GetBitmapSizeY(operat->graymap))), bmprect);
						else				surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->bitmap), GetBitmapSizeY(operat->bitmap))), bmprect);

						if (operat->onClick.GetNOfConnectedSlots() > 0 && operat->popup != NIL)
						{
							Point	 p1 = Point(operat->rect.right - METRIC_IBARROWSIZEX - 5, operat->rect.top + 1);
							Point	 p2 = Point(operat->rect.right - METRIC_IBARROWSIZEX - 5, operat->rect.bottom);

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
			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if ((operat->popup != NIL) && operat->clicked && (GetObject(popupHandle, OBJ_POPUP) != NIL)) if (((PopupMenu *) GetObject(popupHandle, OBJ_POPUP))->IsVisible()) continue;

				if (operat->type == SM_TEXT)
				{
					if (wnd->IsMouseOn(operat->rect) && !operat->checked)
					{
						operat->checked = True;

						if (operat->description != NIL)
						{
							newStatus = operat->description;

							updateStatus = True;
						}

						surface->Frame(operat->rect, FRAME_UP);
					}
					else if (!wnd->IsMouseOn(operat->rect) && operat->checked)
					{
						operat->checked = False;
						operat->clicked = False;

						if (operat->description != NIL) setOldStatus = True;

						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::BackgroundColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;
					}
				}
				else if (operat->type == SM_BITMAP)
				{
					if (wnd->IsMouseOn(operat->rect) && !operat->checked)
					{
						operat->checked = True;

						if (operat->description != NIL)
						{
							newStatus = operat->description;

							updateStatus = True;
						}

						bmprect = operat->rect;
						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + METRIC_IBICONSIZE;
						bmprect.bottom	= bmprect.top + METRIC_IBICONSIZE;

						surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->bitmap), GetBitmapSizeY(operat->bitmap))), bmprect);
						surface->Frame(operat->rect, FRAME_UP);

						if (operat->onClick.GetNOfConnectedSlots() > 0 && operat->popup != NIL)
						{
							Point	 p1 = Point(operat->rect.right - METRIC_IBARROWSIZEX - 5, operat->rect.top + 1);
							Point	 p2 = Point(operat->rect.right - METRIC_IBARROWSIZEX - 5, operat->rect.bottom - 1);

							surface->Bar(p1, p2, OR_VERT);
						}
					}
					else if (!wnd->IsMouseOn(operat->rect) && operat->checked)
					{
						operat->checked = False;
						operat->clicked = False;

						if (operat->description != NIL) setOldStatus = True;

						operat->rect.right++;
						operat->rect.bottom++;
						surface->Box(operat->rect, Setup::BackgroundColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;
						bmprect = operat->rect;
						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + METRIC_IBICONSIZE;
						bmprect.bottom	= bmprect.top + METRIC_IBICONSIZE;

						if (style == MB_GRAYSCALE)	surface->BlitFromBitmap(operat->graymap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->graymap), GetBitmapSizeY(operat->graymap))), bmprect);
						else				surface->BlitFromBitmap(operat->bitmap, Rect(Point(0, 0), Size(GetBitmapSizeX(operat->bitmap), GetBitmapSizeY(operat->bitmap))), bmprect);

						if (operat->onClick.GetNOfConnectedSlots() > 0 && operat->popup != NIL)
						{
							Point	 p1 = Point(operat->rect.right - METRIC_IBARROWSIZEX - 5, operat->rect.top + 1);
							Point	 p2 = Point(operat->rect.right - METRIC_IBARROWSIZEX - 5, operat->rect.bottom);

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

S::Int S::GUI::Menubar::SetStyle(Int newstyle)
{
	if (newstyle == MB_GRAYSCALE || newstyle == MB_COLOR)
	{
		style = newstyle;

		return Success;
	}
	else
	{
		return Error;
	}
}
