 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/menuentry.h>
#include <smooth/gui/widgets/multi/menu/menubar.h>
#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/definitions.h>
#include <smooth/color.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/graphics/surface.h>
#include <smooth/gui/window/window.h>

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

	possibleContainers.AddEntry(Menu::classID);
}

S::GUI::MenuEntry::~MenuEntry()
{
	if (shortcut != NIL) DeleteObject(shortcut);

	if (registered && container != NIL)
	{
		if (popupHandle != NIL) container->UnregisterObject((Widget *) GetObject(popupHandle, PopupMenu::classID));

		container->UnregisterObject(this);
	}
}

S::Int S::GUI::MenuEntry::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface	*surface = container->GetDrawSurface();

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
				Point	 p2 = Point(p1.x, p1.y + size.cy);

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
				textRect.right	= realPos.x + size.cx - 1;
				textRect.bottom	= realPos.y + size.cy - 1;

				surface->SetText(text, textRect, font);
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

					p1.x = realPos.x + size.cx - 9 + (Setup::rightToLeft ? 1 : 0);
					p2.x = p1.x + 7;
					p1.y = realPos.y + (size.cy - 3) / 2;
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
	if (!registered)		return Failure;
	if (!active || !visible)	return Success;

	if (type == SM_SEPARATOR)	return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	EnterProtectedRegion();

	Surface	*surface = container->GetDrawSurface();
	Int	 retVal = Success;

	switch (message)
	{
		case SM_MOUSEMOVE:
			if ((popup != NIL) && clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL)) if (((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->IsVisible()) break;

			if (wnd->IsMouseOn(Rect(pos, size)) && !checked)
			{
				checked = True;

				if (description != NIL) wnd->SetStatusText(description);

				surface->Frame(Rect(pos, size), FRAME_UP);

				if (type == SM_BITMAP)
				{
					Rect	 bmprect = Rect(pos, size);

					bmprect.left	+= 2;
					bmprect.top	+= 2;
					bmprect.right	= bmprect.left + bitmap.GetSize().cx;
					bmprect.bottom	= bmprect.top + bitmap.GetSize().cy;

					surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmprect);

					if (onClick.GetNOfConnectedSlots() > 0 && popup != NIL)
					{
						Point	 p1 = Point(pos.x + size.cx - 12 + (Setup::rightToLeft ? 2 : 0), pos.y + 1);
						Point	 p2 = Point(pos.x + size.cx - 12 + (Setup::rightToLeft ? 2 : 0), pos.y + size.cy - 1);

						surface->Bar(p1, p2, OR_VERT);
					}
				}
			}
			else if (!wnd->IsMouseOn(Rect(pos, size)) && checked)
			{
				checked = False;
				clicked = False;

				if (description != NIL && wnd->GetStatusText() == description) wnd->RestoreDefaultStatusText();

				surface->Box(Rect(pos, Size(size.cx + 1, size.cy + 1)), Setup::BackgroundColor, OUTLINED);

				if (type == SM_BITMAP)
				{
					Rect	 bmprect = Rect(pos, size);

					bmprect.left	+= 2;
					bmprect.top	+= 2;
					bmprect.right	= bmprect.left + bitmap.GetSize().cx;
					bmprect.bottom	= bmprect.top + bitmap.GetSize().cy;

					if (flags & MB_COLOR)	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmprect);
					else			surface->BlitFromBitmap(graymap, Rect(Point(0, 0), graymap.GetSize()), bmprect);

					if (onClick.GetNOfConnectedSlots() > 0 && popup != NIL)
					{
						Point	 p1 = Point(pos.x + size.cx - 12 + (Setup::rightToLeft ? 1 : 0), pos.y + 1);
						Point	 p2 = Point(pos.x + size.cx - 12 + (Setup::rightToLeft ? 1 : 0), pos.y + size.cy);

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
			if ((popup != NIL) && clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL))
			{
				((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->Hide();

				DeleteObject(GetObject(popupHandle, PopupMenu::classID));

				popupHandle = -1;

				break;
			}

			if (checked)
			{
				wnd->Process(SM_LOOSEFOCUS, GetHandle(), 0);

				if (!clicked)
				{
					clicked = True;

					surface->Frame(Rect(pos, size), FRAME_DOWN);
				}
				else
				{
					clicked = False;

					if (!wnd->IsMouseOn(Rect(pos, size)))	surface->Box(Rect(pos, Size(size.cx + 1, size.cy + 1)), Setup::BackgroundColor, OUTLINED);
					else					surface->Frame(Rect(pos, size), FRAME_UP);
				}

				if (clicked && popup != NIL)
				{
					Rect	 popupFrame = Rect(Point(pos.x + size.cx - 11, pos.y), Size(11, size.cy));

					if (onClick.GetNOfConnectedSlots() == 0 || wnd->IsMouseOn(popupFrame))
					{
						PopupMenu *popupMenu = new PopupMenu(popup);

						popupHandle = popupMenu->GetHandle();

						if (orientation == OR_LEFT)
						{
							popupMenu->pos.x = pos.x - 1;
						}
						else if (orientation == OR_RIGHT)
						{
							popupMenu->GetSize();
							popupMenu->pos.x = pos.x + size.cx + 1 - popupMenu->popupsize.cx;
						}

						popupMenu->pos.y = pos.y + size.cy + 2;
						popupMenu->onClick.Connect(&MenuEntry::PopupProc, this);

						wnd->RegisterObject(popupMenu);
					}
				}

				retVal = Break;
			}

			break;
		case SM_LBUTTONUP:
			if ((popup != NIL) && clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL)) if (((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->IsVisible()) break;

			if (clicked)
			{
				clicked = False;

				surface->Frame(Rect(pos, size), FRAME_UP);

				if ((popup == NIL || onClick.GetNOfConnectedSlots() > 0) && bVar == NIL && iVar == NIL)
				{
					checked = False;

					if (description != NIL && wnd->GetStatusText() == description) wnd->RestoreDefaultStatusText();

					surface->Box(Rect(pos, Size(size.cx + 1, size.cy + 1)), Setup::BackgroundColor, OUTLINED);

					if (type == SM_BITMAP)
					{
						Rect	 bmprect = Rect(pos, size);

						bmprect.left	+= 2;
						bmprect.top	+= 2;
						bmprect.right	= bmprect.left + bitmap.GetSize().cx;
						bmprect.bottom	= bmprect.top + bitmap.GetSize().cy;

						if (flags & MB_COLOR)	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmprect);
						else			surface->BlitFromBitmap(graymap, Rect(Point(0, 0), graymap.GetSize()), bmprect);

						if (onClick.GetNOfConnectedSlots() > 0 && popup != NIL)
						{
							Point	 p1 = Point(pos.x + size.cx - 12, pos.y + 1);
							Point	 p2 = Point(pos.x + size.cx - 12, pos.y + size.cy);

							surface->Line(p1, p2, Setup::BackgroundColor);

							p1.x++;
							p2.x++;

							surface->Line(p1, p2, Setup::BackgroundColor);
						}
					}

					onClick.Emit(wnd->MouseX(), wnd->MouseY());

					Process(SM_MOUSEMOVE, 0, 0);
				}

				retVal = Break;
			}

			break;
#ifdef __WIN32__
		case WM_KILLFOCUS:
			if ((popup != NIL) && clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL))
			{
				Bool	 destroyPopup = True;

				if (Window::GetWindow((HWND) wParam) != NIL) if (Window::GetWindow((HWND) wParam)->GetHandle() >= ((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->GetHandle()) destroyPopup = False;

				if (destroyPopup)
				{
					((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->Hide();

					wnd->UnregisterObject((Widget *) GetObject(popupHandle, PopupMenu::classID));

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

	if (text == NIL) type = (type | SM_TEXT) ^ SM_TEXT;

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
	shortcut->onKeyDown.Connect(&MenuEntry::ShortcutProc, this);

	return Success;
}

S::Void S::GUI::MenuEntry::ShortcutProc()
{
	onClick.Emit(0, 0);
}

S::Int S::GUI::MenuEntry::SetBitmap(const Bitmap &newBitmap)
{
	bitmap	= newBitmap;
	graymap	= newBitmap;

	bitmap.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);

	graymap.GrayscaleBitmap();
	graymap.ReplaceColor(CombineColor(192, 192, 192), Setup::BackgroundColor);

	CalcSize();

	return Success;
}

S::Int S::GUI::MenuEntry::CalcSize()
{
	if (type == SM_TEXT)
	{
		size.cx = textSize.cx + 6;
		size.cy = textSize.cy + 3;
	}
	else if (type == SM_BITMAP)
	{
		size.cx = bitmap.GetSize().cx + 3 + (popup != NIL ? 10 + (onClick.GetNOfConnectedSlots() > 0 ? 2 : 0) : 0);
		size.cy = bitmap.GetSize().cy + 3;
	}

	return Success;
}

S::Void S::GUI::MenuEntry::PopupProc()
{
	if ((popup != NIL) && clicked && (GetObject(popupHandle, PopupMenu::classID) != NIL))
	{
		((PopupMenu *) GetObject(popupHandle, PopupMenu::classID))->Hide();

		container->UnregisterObject((Widget *) GetObject(popupHandle, PopupMenu::classID));

		DeleteObject(GetObject(popupHandle, PopupMenu::classID));

		popupHandle = -1;
	}
}
