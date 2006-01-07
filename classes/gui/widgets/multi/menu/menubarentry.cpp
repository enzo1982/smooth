 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/menubarentry.h>
#include <smooth/gui/widgets/multi/menu/menubar.h>
#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/gui/widgets/hotspot/simplebutton.h>
#include <smooth/gui/window/toolwindow.h>
#include <smooth/graphics/color.h>
#include <smooth/graphics/surface.h>

S::GUI::MenubarEntry::MenubarEntry(const String &iText, const Bitmap &iBitmap, Menu *iPopup, Bool *ibVar, Int *iiVar, Int iiCode) : MenuEntry(iText, iBitmap, iPopup, ibVar, iiVar, iiCode)
{
	if (text == NIL && bitmap == NIL)	SetSize(Size(7, 15));
	else if (text != NIL && bitmap == NIL)	SetSize(Size(textSize.cx + 7, 16));
	else if (text == NIL && bitmap != NIL)	SetSize(bitmap.GetSize() + Size(4 + (popup != NIL ? 12 : 0), 4));

	hotspot = NIL;

	if (text != NIL || bitmap != NIL)
	{
		hotspot	= new HotspotSimpleButton(Point(), GetSize());

		hotspot->onLeftButtonDown.Connect(&MenubarEntry::OpenPopupMenu, this);
		hotspot->onLeftButtonClick.Connect(&onAction);

		RegisterObject(hotspot);
	}
}

S::GUI::MenubarEntry::~MenubarEntry()
{
	if (hotspot != NIL) DeleteObject(hotspot);
}

S::Int S::GUI::MenubarEntry::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface	= container->GetDrawSurface();

	EnterProtectedRegion();

	Point	 realPos	= GetRealPosition();
	Rect	 frame		= Rect(realPos, GetSize());
	Rect	 bmpRect	= Rect(realPos + Point(2, 2), bitmap.GetSize());

	switch (message)
	{
		default:
		case SP_PAINT:
			if (text == NIL && bitmap == NIL)
			{
				Point	 p1 = Point(realPos.x + 4 + (Setup::rightToLeft ? 2 : 0), realPos.y);
				Point	 p2 = Point(p1.x, p1.y + GetHeight());

				surface->Bar(p1, p2, OR_VERT);

				p1.x += 2;
				p2.x += 2;

				surface->Bar(p1, p2, OR_VERT);
			}
			else if (text != NIL && bitmap == NIL)
			{
				Rect	 textRect = Rect(realPos + Point(3, 1), GetSize() - Size(4, 2));

				surface->SetText(text, textRect, font);
			}
			else if (text == NIL && bitmap != NIL)
			{
				if (flags & MB_COLOR)	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
				else			surface->BlitFromBitmap(graymap, Rect(Point(0, 0), graymap.GetSize()), bmpRect);

				if (popup != NIL)
				{
					Point	 p1;
					Point	 p2;

					p1.x = realPos.x + GetWidth() - 10 + (Setup::rightToLeft ? 1 : 0);
					p2.x = p1.x + 7;
					p1.y = realPos.y + (GetHeight() - 3) / 2;
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

			if (bitmap != NIL)
			{
				surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);

				if (onAction.GetNOfConnectedSlots() > 0 && popup != NIL)
				{
					Point	 p1 = Point(realPos.x + GetWidth() - 13 + (Setup::rightToLeft ? 2 : 0), realPos.y + 1);
					Point	 p2 = Point(realPos.x + GetWidth() - 13 + (Setup::rightToLeft ? 2 : 0), realPos.y + GetHeight() - 2);

					surface->Bar(p1, p2, OR_VERT);
				}
			}

			break;
		case SP_MOUSEOUT:
			if (focussed && popup != NIL) break;

			if (bitmap != NIL)
			{
				if (flags & MB_COLOR)	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
				else			surface->BlitFromBitmap(graymap, Rect(Point(0, 0), graymap.GetSize()), bmpRect);

				if (onAction.GetNOfConnectedSlots() > 0 && popup != NIL)
				{
					Point	 p1 = Point(realPos.x + GetWidth() - 13 + (Setup::rightToLeft ? 1 : 0), realPos.y + 1);
					Point	 p2 = Point(realPos.x + GetWidth() - 13 + (Setup::rightToLeft ? 1 : 0), realPos.y + GetHeight() - 1);

					surface->Line(p1, p2, Setup::BackgroundColor);

					p1.x++;
					p2.x++;

					surface->Line(p1, p2, Setup::BackgroundColor);
				}
			}

			break;
	}

	LeaveProtectedRegion();

	return Success();
}

S::Int S::GUI::MenubarEntry::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())			return Error();
	if (!IsVisible() || !IsActive())	return Success();

	Window	*window	= container->GetContainerWindow();

	EnterProtectedRegion();

	switch (message)
	{
		case WM_KILLFOCUS:
			if (Window::GetWindow((HWND) wParam) != NIL)
			{
				if (Window::GetWindow((HWND) wParam)->GetObjectType() == ToolWindow::classID || Window::GetWindow((HWND) wParam) == window) break;
			}
		case WM_ACTIVATEAPP:
			ClosePopupMenu();

			break;
	}

	LeaveProtectedRegion();

	return Widget::Process(message, wParam, lParam);
}

S::Void S::GUI::MenubarEntry::OpenPopupMenu()
{
	if (popup == NIL) return;

	Window	*window		= container->GetContainerWindow();
	Rect	 popupFrame	= Rect(GetRealPosition() + Point(GetX() + GetWidth() - 11, GetY()), Size(11, GetHeight()));

	if (onAction.GetNOfConnectedSlots() == 0 || window->IsMouseOn(popupFrame))
	{
		PopupMenu::internalClosePopups.Emit();

		popup->SetPosition(GetRealPosition() + Point(orientation == OR_LEFT ? -1 : GetWidth() + 2 - popup->GetWidth(), GetHeight() + 1));

		RegisterObject(popup);
	}
}

S::Void S::GUI::MenubarEntry::ClosePopupMenu()
{
	if (popup == NIL) return;

	if (popup->GetContainer() == this)
	{
		UnregisterObject(popup);
	}
}
