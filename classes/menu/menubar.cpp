 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_MENUBAR_
#define __OBJSMOOTH_MENUBAR_

#include <smooth/definitions.h>
#include <smooth/menubar.h>
#include <smooth/menu.h>
#include <smooth/toolkit.h>
#include <smooth/loop.h>
#include <smooth/binary.h>
#include <smooth/metrics.h>
#include <smooth/popupmenu.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_MENUBAR = SMOOTH::RequestObjectID();

SMOOTHMenubar::SMOOTHMenubar()
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

SMOOTHMenubar::~SMOOTHMenubar()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHMenubar::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	EnterProtectedRegion();

	SMOOTHMenu::Entry	*operat;
	HDC			 dc = GetContext(wnd);
	SMOOTHRect		 menubar;
	SMOOTHRect		 menuentry;
	SMOOTHRect		 helpmenuentry;
	SMOOTHPoint		 doublebar1;
	SMOOTHPoint		 doublebar2;
	SMOOTHPoint		 p1;
	SMOOTHPoint		 p2;
	bool			 firstentry = SMOOTH::True;
	bool			 prevbitmap = SMOOTH::False;
	bool			 prevtext = SMOOTH::False;
	int			 i;

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

		VBar(dc, doublebar1, doublebar2);

		doublebar1.x = doublebar1.x + 2;
		doublebar2.x = doublebar2.x + 2;

		VBar(dc, doublebar1, doublebar2);
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

				VBar(dc, doublebar1, doublebar2);

				doublebar1.x = doublebar1.x + 2;
				doublebar2.x = doublebar2.x + 2;

				VBar(dc, doublebar1, doublebar2);

				firstentry = SMOOTH::True;
			}
			else if (operat->type == SM_TEXT && operat->orientation == OR_LEFT)
			{
				menuentry.left = menuentry.right + METRIC_MBENTRYSPACING;

				if (prevbitmap) menuentry.left -= 3;

				menuentry.right = menuentry.left + operat->size;

				menuentry.bottom += 1;
				::SetText(dc, operat->text, menuentry, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
				menuentry.bottom -= 1;

				operat->rect.left	= menuentry.left - 3;
				operat->rect.top	= menuentry.top - 1;
				operat->rect.right	= menuentry.right + 3;
				operat->rect.bottom	= menuentry.top + METRIC_MBENTRYHEIGHT;

				prevtext = SMOOTH::True;
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

				if (style == MB_GRAYSCALE)	PaintBitmap(dc, menuentry, operat->graymap);
				else				PaintBitmap(dc, menuentry, operat->bitmap);

				menuentry.left++;
				menuentry.top--;
				menuentry.bottom--;

				operat->rect.left	= menuentry.left - 3;
				operat->rect.top	= menuentry.top - 1;
				operat->rect.right	= menuentry.right + 1;
				operat->rect.bottom	= menuentry.bottom + 2;

				prevbitmap = SMOOTH::True;
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

				if (style == MB_GRAYSCALE)	PaintBitmap(dc, menuentry, operat->graymap);
				else				PaintBitmap(dc, menuentry, operat->bitmap);

				menuentry.left++;
				menuentry.top--;
				menuentry.bottom--;
				menuentry.right = menuentry.left + METRIC_IBICONSIZE + METRIC_IBARROWSIZEX + 2;

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
					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
				}

				operat->rect.left	= menuentry.left - 3;
				operat->rect.top	= menuentry.top - 1;
				operat->rect.right	= menuentry.right + 1;
				operat->rect.bottom	= menuentry.bottom + 2;

				prevbitmap = SMOOTH::True;
			}

			if (operat->orientation == OR_LEFT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = SMOOTH::False;
				if (operat->type != SM_BITMAP)		prevbitmap = SMOOTH::False;
				if (operat->type != SM_TEXT)		prevtext = SMOOTH::False;
			}
		}

		firstentry = SMOOTH::True;
		prevbitmap = SMOOTH::False;
		prevtext = SMOOTH::False;

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

				VBar(dc, doublebar1, doublebar2);

				doublebar1.x = doublebar1.x + 2;
				doublebar2.x = doublebar2.x + 2;

				VBar(dc, doublebar1, doublebar2);

				firstentry = SMOOTH::True;
				prevbitmap = SMOOTH::True;
			}
			else if (operat->type == SM_TEXT  && operat->orientation == OR_RIGHT)
			{
				helpmenuentry.right = helpmenuentry.left - METRIC_MBENTRYSPACING;

				if (prevbitmap) helpmenuentry.right += 1;
				if (prevtext) helpmenuentry.right += 1;

				helpmenuentry.left = helpmenuentry.right - operat->size;

				helpmenuentry.bottom += 1;
				::SetText(dc, operat->text, helpmenuentry, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
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

				if (style == MB_GRAYSCALE)	PaintBitmap(dc, helpmenuentry, operat->graymap);
				else				PaintBitmap(dc, helpmenuentry, operat->bitmap);

				helpmenuentry.top--;
				helpmenuentry.bottom--;
				helpmenuentry.left++;

				operat->rect.left	= helpmenuentry.left - 3;
				operat->rect.top	= helpmenuentry.top - 1;
				operat->rect.right	= helpmenuentry.right + 1;
				operat->rect.bottom	= helpmenuentry.bottom + 2;

				prevtext = SMOOTH::True;
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

				if (style == MB_GRAYSCALE)	PaintBitmap(dc, helpmenuentry, operat->graymap);
				else				PaintBitmap(dc, helpmenuentry, operat->bitmap);

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
					Line(dc, p1, p2, SMOOTH::Setup::TextColor, PS_SOLID, 1);
				}

				operat->rect.left	= helpmenuentry.left - 3;
				operat->rect.top	= helpmenuentry.top - 1;
				operat->rect.right	= helpmenuentry.right + 1;
				operat->rect.bottom	= helpmenuentry.bottom + 2;

				prevtext = SMOOTH::True;
			}

			if (operat->orientation == OR_RIGHT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = SMOOTH::False;
				if (operat->type != SM_BITMAP)		prevbitmap = SMOOTH::False;
				if (operat->type != SM_TEXT)		prevtext = SMOOTH::False;
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

				if (style == MB_GRAYSCALE)	PaintBitmap(dc, menuentry, operat->graymap);
				else				PaintBitmap(dc, menuentry, operat->bitmap);

				menuentry.top--;

				operat->rect.left	= menuentry.left - 3;
				operat->rect.top	= menuentry.top - 1;
				operat->rect.right	= menuentry.right + 1;
				operat->rect.bottom	= menuentry.bottom + 2;

				prevbitmap = SMOOTH::True;
			}

			if (operat->orientation == OR_LEFT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = SMOOTH::False;
				if (operat->type != SM_BITMAP)		prevbitmap = SMOOTH::False;
				if (operat->type != SM_TEXT)		prevtext = SMOOTH::False;
			}
		}

		for (int j = 0; j < nOfEntries; j++)
		{
			operat = entries.GetNthEntry(j);

			if (operat->orientation == OR_RIGHT)
			{
				if (operat->type != SM_SEPARATOR)	firstentry = SMOOTH::False;
				if (operat->type != SM_BITMAP)		prevbitmap = SMOOTH::False;
				if (operat->type != SM_TEXT)		prevtext = SMOOTH::False;
			}
		}
	}

	FreeContext(wnd, dc);

	LeaveProtectedRegion();

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHMenubar::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	EnterProtectedRegion();

	SMOOTHMenu::Entry	*operat;
	SMOOTHRect		 bmprect;
	HDC			 dc;
	SMOOTHInt		 retVal = SMOOTH::Success;
	bool			 cont = SMOOTH::False;
	SMOOTHInt		 i;
	SMOOTHPopupMenu		*popupMenu = NIL;

	SMOOTHString		 newStatus;
	SMOOTHBool		 updateStatus = SMOOTH::False;
	SMOOTHBool		 setOldStatus = SMOOTH::False;

	switch (message)
	{
		case SM_LBUTTONDOWN:
			dc = GetContext(wnd);

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if ((operat->popup != NIL) && operat->clicked && (SMOOTH::GetObject(popupHandle, OBJ_POPUP) != NIL)) if (SMOOTH::GetObject(popupHandle, OBJ_POPUP)->IsVisible() && !cont) continue;

				if (operat->checked && (operat->type != SM_SEPARATOR))
				{
					wnd->Process(SM_LOOSEFOCUS, handle, 0);

					if (!operat->clicked)
					{
						operat->clicked = SMOOTH::True;
						Frame(dc, operat->rect, FRAME_DOWN);
					}
					else
					{
						operat->clicked = SMOOTH::False;
						Frame(dc, operat->rect, FRAME_UP);
					}

					if (operat->clicked && (operat->popup != NIL && operat->orientation == OR_LEFT))
					{
						popupMenu = new SMOOTHPopupMenu();

						popupHandle = popupMenu->handle;

						popupMenu->MenuToPopup(operat->popup);
						popupMenu->GetObjectProperties()->pos.x = operat->rect.left - 1;
						popupMenu->GetObjectProperties()->pos.y = operat->rect.bottom + 2;

						wnd->RegisterObject(popupMenu);

						cont = SMOOTH::True;
					}
					else if (operat->clicked && (operat->popup != NIL && operat->orientation == OR_RIGHT))
					{
						popupMenu = new SMOOTHPopupMenu();

						popupHandle = popupMenu->handle;

						popupMenu->MenuToPopup(operat->popup);
						((SMOOTHMenu *) popupMenu)->GetSize();
						popupMenu->GetObjectProperties()->pos.x = operat->rect.right + 1 - ((SMOOTHMenu *) popupMenu)->popupsize.cx;
						popupMenu->GetObjectProperties()->pos.y = operat->rect.bottom + 2;

						wnd->RegisterObject(popupMenu);

						cont = SMOOTH::True;
					}

					retVal = SMOOTH::Break;
				}
			}

			FreeContext(wnd, dc);

			break;
		case SM_LBUTTONUP:
			dc = GetContext(wnd);

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if ((operat->popup != NIL) && operat->clicked && (SMOOTH::GetObject(popupHandle, OBJ_POPUP) != NIL)) if (SMOOTH::GetObject(popupHandle, OBJ_POPUP)->IsVisible()) continue;

				if (operat->type == SM_TEXT && operat->clicked)
				{
					operat->clicked = SMOOTH::False;
					Frame(dc, operat->rect, FRAME_UP);

					if (operat->proc != NIL && operat->bVar == NIL && operat->iVar == NIL)
					{
						operat->checked = SMOOTH::False;

						if (operat->description != NIL) wnd->SetStatusText(backupStatusText);

						operat->rect.right++;
						operat->rect.bottom++;
						Box(dc, operat->rect, SMOOTH::Setup::BackgroundColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;

						SMOOTHProcCall(operat->proc, operat->procParam);

						Process(SM_MOUSEMOVE, 0, 0);

						retVal = SMOOTH::Break;

						break;
					}

					retVal = SMOOTH::Break;
				}
				else if (operat->type == SM_BITMAP && operat->clicked)
				{
					operat->clicked = SMOOTH::False;
					Frame(dc, operat->rect, FRAME_UP);

					if (operat->proc != NIL && operat->bVar == NIL && operat->iVar == NIL)
					{
						operat->checked = SMOOTH::False;

						if (operat->description != NIL) wnd->SetStatusText(backupStatusText);

						operat->rect.right++;
						operat->rect.bottom++;
						Box(dc, operat->rect, SMOOTH::Setup::BackgroundColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;

						bmprect = operat->rect;
						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + METRIC_IBICONSIZE;
						bmprect.bottom	= bmprect.top + METRIC_IBICONSIZE;

						if (style == MB_GRAYSCALE)	PaintBitmap(dc, bmprect, operat->graymap);
						else				PaintBitmap(dc, bmprect, operat->bitmap);

						SMOOTHProcCall(operat->proc, operat->procParam);

						Process(SM_MOUSEMOVE, 0, 0);

						retVal = SMOOTH::Break;

						break;
					}

					retVal = SMOOTH::Break;
				}
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSELEAVE:
			dc = GetContext(wnd);

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if ((operat->popup != NIL) && operat->clicked && (SMOOTH::GetObject(popupHandle, OBJ_POPUP) != NIL)) if (SMOOTH::GetObject(popupHandle, OBJ_POPUP)->IsVisible()) continue;

				if (operat->type == SM_TEXT)
				{
					if (!IsMouseOn(wnd->hwnd, operat->rect, WINDOW) && operat->checked)
					{
						operat->checked = SMOOTH::False;
						operat->clicked = SMOOTH::False;

						if (operat->description != NIL) setOldStatus = SMOOTH::True;

						operat->rect.right++;
						operat->rect.bottom++;
						Box(dc, operat->rect, SMOOTH::Setup::BackgroundColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;
					}
				}
				else if (operat->type == SM_BITMAP)
				{
					if (!IsMouseOn(wnd->hwnd, operat->rect, WINDOW) && operat->checked)
					{
						operat->checked = SMOOTH::False;
						operat->clicked = SMOOTH::False;

						if (operat->description != NIL) setOldStatus = SMOOTH::True;

						operat->rect.right++;
						operat->rect.bottom++;
						Box(dc, operat->rect, SMOOTH::Setup::BackgroundColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;
						bmprect = operat->rect;
						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + METRIC_IBICONSIZE;
						bmprect.bottom	= bmprect.top + METRIC_IBICONSIZE;

						if (style == MB_GRAYSCALE)	PaintBitmap(dc, bmprect, operat->graymap);
						else				PaintBitmap(dc, bmprect, operat->bitmap);
					}
				}
			}

			FreeContext(wnd, dc);

			break;
		case SM_MOUSEMOVE:
			dc = GetContext(wnd);

			for (i = 0; i < nOfEntries; i++)
			{
				operat = entries.GetNthEntry(i);

				if ((operat->popup != NIL) && operat->clicked && (SMOOTH::GetObject(popupHandle, OBJ_POPUP) != NIL)) if (SMOOTH::GetObject(popupHandle, OBJ_POPUP)->IsVisible()) continue;

				if (operat->type == SM_TEXT)
				{
					if (IsMouseOn(wnd->hwnd, operat->rect, WINDOW) && !operat->checked)
					{
						operat->checked = SMOOTH::True;

						if (operat->description != NIL)
						{
							newStatus = operat->description;

							updateStatus = SMOOTH::True;
						}

						Frame(dc, operat->rect, FRAME_UP);
					}
					else if (!IsMouseOn(wnd->hwnd, operat->rect, WINDOW) && operat->checked)
					{
						operat->checked = SMOOTH::False;
						operat->clicked = SMOOTH::False;

						if (operat->description != NIL) setOldStatus = SMOOTH::True;

						operat->rect.right++;
						operat->rect.bottom++;
						Box(dc, operat->rect, SMOOTH::Setup::BackgroundColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;
					}
				}
				else if (operat->type == SM_BITMAP)
				{
					if (IsMouseOn(wnd->hwnd, operat->rect, WINDOW) && !operat->checked)
					{
						operat->checked = SMOOTH::True;

						if (operat->description != NIL)
						{
							newStatus = operat->description;

							updateStatus = SMOOTH::True;
						}

						bmprect = operat->rect;
						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + METRIC_IBICONSIZE;
						bmprect.bottom	= bmprect.top + METRIC_IBICONSIZE;
						PaintBitmap(dc, bmprect, operat->bitmap);
						Frame(dc, operat->rect, FRAME_UP);
					}
					else if (!IsMouseOn(wnd->hwnd, operat->rect, WINDOW) && operat->checked)
					{
						operat->checked = SMOOTH::False;
						operat->clicked = SMOOTH::False;

						if (operat->description != NIL) setOldStatus = SMOOTH::True;

						operat->rect.right++;
						operat->rect.bottom++;
						Box(dc, operat->rect, SMOOTH::Setup::BackgroundColor, OUTLINED);
						operat->rect.right--;
						operat->rect.bottom--;
						bmprect = operat->rect;
						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + METRIC_IBICONSIZE;
						bmprect.bottom	= bmprect.top + METRIC_IBICONSIZE;

						if (style == MB_GRAYSCALE)	PaintBitmap(dc, bmprect, operat->graymap);
						else				PaintBitmap(dc, bmprect, operat->bitmap);
					}
				}
			}

			FreeContext(wnd, dc);

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

SMOOTHInt SMOOTHMenubar::SetStyle(SMOOTHInt newstyle)
{
	if (newstyle == MB_GRAYSCALE || newstyle == MB_COLOR)
	{
		style = newstyle;

		return SMOOTH::Success;
	}
	else
	{
		return SMOOTH::Error;
	}
}

#endif
