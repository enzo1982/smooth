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
#include <smooth/window/window.h>

S::GUI::MenuEntry::MenuEntry(String iText, Bitmap iBitmap, Menu *iPopup, Bool *ibVar, Int *iiVar, Int iiCode, Int iOrientation)
{
	type	= SM_SEPARATOR;

	if (iText != NIL)	type = type | SM_TEXT;
	if (iBitmap != NIL)	type = type | SM_BITMAP;

	popup		= iPopup;
	bVar		= ibVar;
	iVar		= iiVar;
	iCode		= iiCode;

	if (iBitmap != NIL) SetBitmap(iBitmap);

	SetText(iText);
	SetOrientation(iOrientation);

	scKey		= 0;
	scFlags		= 0;

	shortcut	= NIL;

	popupHandle	= -1;

	onClick.SetParentObject(this);

	possibleContainers.AddEntry(Menu::classID);
}

S::GUI::MenuEntry::~MenuEntry()
{
	if (shortcut != NIL) DeleteObject(shortcut);

	if (registered && myContainer != NIL)
	{
		if (popupHandle != NIL) myContainer->UnregisterObject(GetObject(popupHandle, PopupMenu::classID));

		myContainer->UnregisterObject(this);
	}
}

S::Int S::GUI::MenuEntry::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Point	 realPos = GetRealPosition();

	CalcSize();

	switch (message)
	{
		default:
		case SP_PAINT:
			if (type == SM_SEPARATOR)
			{
				Point	 p1 = Point(realPos.x + 4 + (Setup::rightToLeft ? 2 : 0), realPos.y);
				Point	 p2 = Point(p1.x, p1.y + objectProperties->size.cy);

				surface->Bar(p1, p2, OR_VERT);

				p1.x += 2;
				p2.x += 2;

				surface->Bar(p1, p2, OR_VERT);
			}
			else if (type == SM_TEXT)
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

					p1.x = realPos.x + objectProperties->size.cx - 9 + (Setup::rightToLeft ? 1 : 0);
					p2.x = p1.x + 7;
					p1.y = realPos.y + (objectProperties->size.cy - 3) / 2;
					p2.y = p1.y;

					for (Int y = 0; y < 3; y++)
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

S::Int S::GUI::MenuEntry::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;

	if (type == SM_SEPARATOR)	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	EnterProtectedRegion();

	Surface	*surface = myContainer->GetDrawSurface();
	Int	 retVal = Success;

	switch (message)
	{
		case SM_MOUSEMOVE:
			if ((popup != NIL) && objectProperties->clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL)) if (((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->IsVisible()) break;

			if (wnd->IsMouseOn(Rect(objectProperties->pos, objectProperties->size)) && !objectProperties->checked)
			{
				objectProperties->checked = True;

				if (description != NIL) wnd->SetStatusText(description);

				surface->Frame(Rect(objectProperties->pos, objectProperties->size), FRAME_UP);

				if (type == SM_BITMAP)
				{
					Rect	 bmprect = Rect(objectProperties->pos, objectProperties->size);

					bmprect.left	+= 2;
					bmprect.top	+= 2;
					bmprect.right	= bmprect.left + bitmap.GetSize().cx;
					bmprect.bottom	= bmprect.top + bitmap.GetSize().cy;

					surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmprect);

					if (onClick.GetNOfConnectedSlots() > 0 && popup != NIL)
					{
						Point	 p1 = Point(objectProperties->pos.x + objectProperties->size.cx - 12 + (Setup::rightToLeft ? 2 : 0), objectProperties->pos.y + 1);
						Point	 p2 = Point(objectProperties->pos.x + objectProperties->size.cx - 12 + (Setup::rightToLeft ? 2 : 0), objectProperties->pos.y + objectProperties->size.cy - 1);

						surface->Bar(p1, p2, OR_VERT);
					}
				}
			}
			else if (!wnd->IsMouseOn(Rect(objectProperties->pos, objectProperties->size)) && objectProperties->checked)
			{
				objectProperties->checked = False;
				objectProperties->clicked = False;

				if (description != NIL && wnd->GetStatusText() == description) wnd->RestoreDefaultStatusText();

				surface->Box(Rect(objectProperties->pos, Size(objectProperties->size.cx + 1, objectProperties->size.cy + 1)), Setup::BackgroundColor, OUTLINED);

				if (type == SM_BITMAP)
				{
					Rect	 bmprect = Rect(objectProperties->pos, objectProperties->size);

					bmprect.left	+= 2;
					bmprect.top	+= 2;
					bmprect.right	= bmprect.left + bitmap.GetSize().cx;
					bmprect.bottom	= bmprect.top + bitmap.GetSize().cy;

					if (flags & MB_COLOR)	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmprect);
					else			surface->BlitFromBitmap(graymap, Rect(Point(0, 0), graymap.GetSize()), bmprect);

					if (onClick.GetNOfConnectedSlots() > 0 && popup != NIL)
					{
						Point	 p1 = Point(objectProperties->pos.x + objectProperties->size.cx - 12 + (Setup::rightToLeft ? 1 : 0), objectProperties->pos.y + 1);
						Point	 p2 = Point(objectProperties->pos.x + objectProperties->size.cx - 12 + (Setup::rightToLeft ? 1 : 0), objectProperties->pos.y + objectProperties->size.cy);

						surface->Line(p1, p2, Setup::BackgroundColor);

						p1.x++;
						p2.x++;

						surface->Line(p1, p2, Setup::BackgroundColor);
					}
				}
			}

			break;
		case SM_LBUTTONDOWN:
		case SM_LBUTTONDBLCLK:
			if ((popup != NIL) && objectProperties->clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL))
			{
				((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->Hide();

				DeleteObject(GetObject(popupHandle, PopupMenu::classID));

				popupHandle = -1;

				break;
			}

			if (objectProperties->checked)
			{
				wnd->Process(SM_LOOSEFOCUS, handle, 0);

				if (!objectProperties->clicked)
				{
					objectProperties->clicked = True;

					surface->Frame(Rect(objectProperties->pos, objectProperties->size), FRAME_DOWN);
				}
				else
				{
					objectProperties->clicked = False;

					if (!wnd->IsMouseOn(Rect(objectProperties->pos, objectProperties->size)))	surface->Box(Rect(objectProperties->pos, Size(objectProperties->size.cx + 1, objectProperties->size.cy + 1)), Setup::BackgroundColor, OUTLINED);
					else										surface->Frame(Rect(objectProperties->pos, objectProperties->size), FRAME_UP);
				}

				if (objectProperties->clicked && popup != NIL)
				{
					Rect	 popupFrame = Rect(Point(objectProperties->pos.x + objectProperties->size.cx - 11, objectProperties->pos.y), Size(11, objectProperties->size.cy));

					if (onClick.GetNOfConnectedSlots() == 0 || wnd->IsMouseOn(popupFrame))
					{
						PopupMenu *popupMenu = new PopupMenu(popup);

						popupHandle = popupMenu->handle;

						if (objectProperties->orientation == OR_LEFT)
						{
							popupMenu->GetObjectProperties()->pos.x = objectProperties->pos.x - 1;
						}
						else if (objectProperties->orientation == OR_RIGHT)
						{
							popupMenu->GetSize();
							popupMenu->GetObjectProperties()->pos.x = objectProperties->pos.x + objectProperties->size.cx + 1 - popupMenu->popupsize.cx;
						}

						popupMenu->GetObjectProperties()->pos.y = objectProperties->pos.y + objectProperties->size.cy + 2;
						popupMenu->onClick.Connect(&MenuEntry::PopupProc, this);

						wnd->RegisterObject(popupMenu);
					}
				}

				retVal = Break;
			}

			break;
		case SM_LBUTTONUP:
			if ((popup != NIL) && objectProperties->clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL)) if (((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->IsVisible()) break;

			if (objectProperties->clicked)
			{
				objectProperties->clicked = False;

				surface->Frame(Rect(objectProperties->pos, objectProperties->size), FRAME_UP);

				if ((popup == NIL || onClick.GetNOfConnectedSlots() > 0) && bVar == NIL && iVar == NIL)
				{
					objectProperties->checked = False;

					if (description != NIL && wnd->GetStatusText() == description) wnd->RestoreDefaultStatusText();

					surface->Box(Rect(objectProperties->pos, Size(objectProperties->size.cx + 1, objectProperties->size.cy + 1)), Setup::BackgroundColor, OUTLINED);

					if (type == SM_BITMAP)
					{
						Rect	 bmprect = Rect(objectProperties->pos, objectProperties->size);

						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + bitmap.GetSize().cx;
						bmprect.bottom	= bmprect.top + bitmap.GetSize().cy;

						if (flags & MB_COLOR)	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmprect);
						else			surface->BlitFromBitmap(graymap, Rect(Point(0, 0), graymap.GetSize()), bmprect);

						if (onClick.GetNOfConnectedSlots() > 0 && popup != NIL)
						{
							Point	 p1 = Point(objectProperties->pos.x + objectProperties->size.cx - 12, objectProperties->pos.y + 1);
							Point	 p2 = Point(objectProperties->pos.x + objectProperties->size.cx - 12, objectProperties->pos.y + objectProperties->size.cy);

							surface->Line(p1, p2, Setup::BackgroundColor);

							p1.x++;
							p2.x++;

							surface->Line(p1, p2, Setup::BackgroundColor);
						}
					}

					onClick.Emit();

					Process(SM_MOUSEMOVE, 0, 0);
				}

				retVal = Break;
			}

			break;
#ifdef __WIN32__
		case WM_KILLFOCUS:
			if ((popup != NIL) && objectProperties->clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL))
			{
				Bool	 destroyPopup = True;

				if (Window::GetWindow((HWND) wParam) != NIL) if (Window::GetWindow((HWND) wParam)->handle >= ((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->handle) destroyPopup = False;

				if (destroyPopup)
				{
					((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->Hide();

					wnd->UnregisterObject(GetObject(popupHandle, PopupMenu::classID));

					DeleteObject(GetObject(popupHandle, PopupMenu::classID));

					popupHandle = -1;
				}
			}

			break;
#endif
	}

	LeaveProtectedRegion();

	return retVal;
}

S::Int S::GUI::MenuEntry::SetText(const String &newText)
{
	Widget::SetText(newText);

	CalcSize();

	if (objectProperties->text == NIL) type = (type | SM_TEXT) ^ SM_TEXT;

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

	CalcSize();

	return Success;
}

S::Int S::GUI::MenuEntry::CalcSize()
{
	if (type == SM_TEXT)
	{
		objectProperties->size.cx = objectProperties->textSize.cx + 6;
		objectProperties->size.cy = objectProperties->textSize.cy + 3;
	}
	else if (type == SM_BITMAP)
	{
		objectProperties->size.cx = bitmap.GetSize().cx + 3 + (popup != NIL ? 10 + (onClick.GetNOfConnectedSlots() > 0 ? 2 : 0) : 0);
		objectProperties->size.cy = bitmap.GetSize().cy + 3;
	}

	return Success;
}

S::Void S::GUI::MenuEntry::PopupProc()
{
	if ((popup != NIL) && objectProperties->clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL))
	{
		((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->Hide();

		myContainer->UnregisterObject(GetObject(popupHandle, PopupMenu::classID));

		DeleteObject(GetObject(popupHandle, PopupMenu::classID));

		popupHandle = -1;
	}
}
