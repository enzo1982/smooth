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

	onGetFocus.Connect(&MenuEntry::OpenPopupMenu, this);
	onLoseFocus.Connect(&MenuEntry::ClosePopupMenu, this);
	onClickInFocus.Connect(&MenuEntry::ClosePopupMenu, this);
}

S::GUI::MenuEntry::~MenuEntry()
{
	if (shortcut != NIL) DeleteObject(shortcut);

	if (IsRegistered() && container != NIL)
	{
		if (popupHandle != NIL) container->UnregisterObject((Widget *) GetObject(popupHandle, PopupMenu::classID));

		container->UnregisterObject(this);
	}
}

S::Int S::GUI::MenuEntry::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface	= container->GetDrawSurface();

	EnterProtectedRegion();

	Point	 realPos	= GetRealPosition();
	Rect	 frame		= Rect(GetRealPosition(), size);
	Rect	 bmpRect	= Rect(GetRealPosition() + Point(2, 2), bitmap.GetSize());

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
				Rect	 textRect = Rect(realPos + Point(3, 1), size - Size(4, 2));

				surface->SetText(text, textRect, font);
			}
			else if (type == SM_BITMAP)
			{
				if (flags & MB_COLOR)	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
				else			surface->BlitFromBitmap(graymap, Rect(Point(0, 0), graymap.GetSize()), bmpRect);

				if (popup != NIL)
				{
					Point	 p1;
					Point	 p2;

					p1.x = realPos.x + size.cx - 10 + (Setup::rightToLeft ? 1 : 0);
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
		case SP_MOUSEIN:
		case SP_MOUSEUP:
			if (focussed && popup != NIL) break;

			surface->Frame(frame, FRAME_UP);

			if (type == SM_BITMAP)
			{
				surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);

				if (onClick.GetNOfConnectedSlots() > 0 && popup != NIL)
				{
					Point	 p1 = Point(realPos.x + size.cx - 13 + (Setup::rightToLeft ? 2 : 0), realPos.y + 1);
					Point	 p2 = Point(realPos.x + size.cx - 13 + (Setup::rightToLeft ? 2 : 0), realPos.y + size.cy - 2);

					surface->Bar(p1, p2, OR_VERT);
				}
			}

			break;
		case SP_MOUSEDOWN:
			surface->Frame(Rect(pos, size), FRAME_DOWN);

			break;
		case SP_MOUSEOUT:
			if (focussed && popup != NIL) break;

			surface->Box(frame, Setup::BackgroundColor, OUTLINED);

			if (type == SM_BITMAP)
			{
				if (flags & MB_COLOR)	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
				else			surface->BlitFromBitmap(graymap, Rect(Point(0, 0), graymap.GetSize()), bmpRect);

				if (onClick.GetNOfConnectedSlots() > 0 && popup != NIL)
				{
					Point	 p1 = Point(realPos.x + size.cx - 13 + (Setup::rightToLeft ? 1 : 0), realPos.y + 1);
					Point	 p2 = Point(realPos.x + size.cx - 13 + (Setup::rightToLeft ? 1 : 0), realPos.y + size.cy - 1);

					surface->Line(p1, p2, Setup::BackgroundColor);

					p1.x++;
					p2.x++;

					surface->Line(p1, p2, Setup::BackgroundColor);
				}
			}

			break;
	}

	LeaveProtectedRegion();

	return Success;
}

S::Int S::GUI::MenuEntry::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())		return Failure;
	if (!active || !IsVisible())	return Success;

	if (type == SM_SEPARATOR)	return Success;

	Window	*window = container->GetContainerWindow();

	if (window == NIL) return Success;

	EnterProtectedRegion();

	Int	 retVal = Success;

	if (type == SM_BITMAP && onClick.GetNOfConnectedSlots() > 0 && popup != NIL && (message == SM_LBUTTONDOWN || message == SM_LBUTTONDBLCLK))
	{
			if (mouseOver)
			{
				leftButtonDown = True;

				DeactivateTooltip();

				Paint(SP_MOUSEDOWN);

				if (focussed)
				{
					leftButtonDown = False;

					onClickInFocus.Emit();
				}
				else if (popup != NIL)
				{
					Rect	 popupFrame = Rect(Point(pos.x + size.cx - 11, pos.y), Size(11, size.cy));

					if (onClick.GetNOfConnectedSlots() == 0 || window->IsMouseOn(popupFrame))
					{
						focussed = True;
						leftButtonDown = False;

						OpenPopupMenu();
					}
				}

				onLeftButtonDown.Emit(Point(window->MouseX(), window->MouseY()));

				if (message == SM_LBUTTONDBLCLK) onLeftButtonDoubleClick.Emit(Point(window->MouseX(), window->MouseY()));
			}
			else
			{
				if (focussed)
				{
					focussed = False;

					onLoseFocus.Emit();
				}
			}
	}
	else
	{
		retVal = Widget::Process(message, wParam, lParam);
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
		size.cx = textSize.cx + 7;
		size.cy = textSize.cy + 4;
	}
	else if (type == SM_BITMAP)
	{
		size.cx = bitmap.GetSize().cx + 4 + (popup != NIL ? 10 + (onClick.GetNOfConnectedSlots() > 0 ? 2 : 0) : 0);
		size.cy = bitmap.GetSize().cy + 4;
	}

	return Success;
}

S::Void S::GUI::MenuEntry::OpenPopupMenu()
{
	if (popup == NIL) return;

	Window	*window		= container->GetContainerWindow();

	if (window == NIL) return;

	Rect	 popupFrame	= Rect(Point(pos.x + size.cx - 11, pos.y), Size(11, size.cy));

	if (onClick.GetNOfConnectedSlots() == 0 || window->IsMouseOn(popupFrame))
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

		popupMenu->pos.y = pos.y + size.cy + 1;
		popupMenu->onClick.Connect(&MenuEntry::ClosePopupMenu, this);

		window->RegisterObject(popupMenu);
	}
}

S::Void S::GUI::MenuEntry::ClosePopupMenu()
{
	if (popup == NIL) return;

	Window	*window = container->GetContainerWindow();

	if (window == NIL) return;

	if (GetObject(popupHandle, PopupMenu::classID) != NIL)
	{
		((Widget *) GetObject(popupHandle, PopupMenu::classID))->Hide();

		container->UnregisterObject((Widget *) GetObject(popupHandle, PopupMenu::classID));

		DeleteObject(GetObject(popupHandle, PopupMenu::classID));

		popupHandle = -1;

		focussed = False;

		if (window->IsMouseOn(Rect(GetRealPosition(), size)))	Paint(SP_MOUSEIN);
		else							Paint(SP_MOUSEOUT);
	}
}
