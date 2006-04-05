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
#include <smooth/gui/window/window.h>
#include <smooth/graphics/color.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/binary.h>

const S::Int	 S::GUI::MenubarEntry::classID = S::Object::RequestClassID();

S::GUI::MenubarEntry::MenubarEntry(const String &iText, const Bitmap &iBitmap, PopupMenu *iPopup, Bool *ibVar, Int *iiVar, Int iiCode) : MenuEntry(iText, iBitmap, iPopup, ibVar, iiVar, iiCode)
{
	type		= classID;

	if (text == NIL && bitmap == NIL)	SetSize(Size(4, 15));
	else if (text != NIL && bitmap == NIL)	SetSize(Size(textSize.cx + 7, 16));
	else if (text == NIL && bitmap != NIL)	SetSize(bitmap.GetSize() + Size(4 + (popup != NIL ? 12 : 0), 4));

	hotspot = NIL;
	actionHotspot = NIL;

	if (text != NIL || bitmap != NIL)
	{
		hotspot	= new HotspotSimpleButton(Point(), GetSize());
		hotspot->onMouseOver.Connect(&MenubarEntry::OnMouseOver, this);
		hotspot->onLeftButtonDown.Connect(&MenubarEntry::OpenPopupMenu, this);

		actionHotspot = new Hotspot(Point(), GetSize() - Size((text == NIL && bitmap != NIL && popup != NIL ? 12 : 0), 0));
		actionHotspot->onLeftButtonClick.Connect(&onAction);

		RegisterObject(hotspot);
		RegisterObject(actionHotspot);
	}
}

S::GUI::MenubarEntry::~MenubarEntry()
{
	if (hotspot != NIL) DeleteObject(hotspot);
	if (actionHotspot != NIL) DeleteObject(actionHotspot);
}

S::Int S::GUI::MenubarEntry::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Surface	*surface = container->GetDrawSurface();
	Point	 realPos = GetRealPosition();
	Rect	 frame	 = Rect(realPos, GetSize());
	Rect	 bmpRect = Rect(realPos + Point(2, 2), bitmap.GetSize());

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			if (text == NIL && bitmap == NIL)
			{
				if (orientation == OR_LEFT || orientation == OR_RIGHT)
				{
					SetWidth(4);

					Point	 p1 = Point(realPos.x, realPos.y);
					Point	 p2 = Point(p1.x, p1.y + GetHeight());

					surface->Bar(p1, p2, OR_VERT);

					p1.x += 2;
					p2.x += 2;

					surface->Bar(p1, p2, OR_VERT);
				}
				else if (orientation == OR_TOP || orientation == OR_BOTTOM)
				{
					SetHeight(4);

					Point	 p1 = Point(realPos.x, realPos.y);
					Point	 p2 = Point(p1.x + GetWidth(), p1.y);

					surface->Bar(p1, p2, OR_HORZ);

					p1.y += 2;
					p2.y += 2;

					surface->Bar(p1, p2, OR_HORZ);
				}
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
			if (focussed && popup != NIL) if (popup->GetContainer() == this) break;

			if (bitmap != NIL)
			{
				surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);

				if (onAction.GetNOfConnectedSlots() > 0 && popup != NIL)
				{
					Point	 p1 = Point(realPos.x + GetWidth() - 13, realPos.y + 1);
					Point	 p2 = Point(realPos.x + GetWidth() - 13, realPos.y + GetHeight() - 2);

					surface->Bar(p1, p2, OR_VERT);
				}
			}

			break;
		case SP_MOUSEOUT:
			if (focussed && popup != NIL) if (popup->GetContainer() == this) break;

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

	return Success();
}

S::Void S::GUI::MenubarEntry::OnMouseOver()
{
	if (Binary::IsFlagSet(container->GetFlags(), MB_POPUPOPEN) && text != NIL && bitmap == NIL)
	{
		hotspot->Paint(SP_MOUSEDOWN);

		OpenPopupMenu();
	}
}

S::Void S::GUI::MenubarEntry::OpenPopupMenu()
{
	if (popup == NIL) return;

	Window	*window		= container->GetContainerWindow();
	Rect	 popupFrame	= Rect(GetRealPosition() + Point(GetWidth() - 11, 0), Size(11, GetHeight()));

	if (onAction.GetNOfConnectedSlots() == 0 || window->IsMouseOn(popupFrame))
	{
		hotspot->Deactivate();

		popup->SetPosition(GetRealPosition() + Point(orientation == OR_LEFT ? -1 : GetWidth() + 2 - popup->GetWidth(), GetHeight() + 1));
		popup->internalRequestClose.Connect(&MenubarEntry::ClosePopupMenu, this);

		RegisterObject(popup);

		container->SetFlags(container->GetFlags() | MB_POPUPOPEN);
	}
}

S::Void S::GUI::MenubarEntry::ClosePopupMenu()
{
	if (popup == NIL) return;

	if (popup->GetContainer() == this)
	{
		container->SetFlags(container->GetFlags() & !MB_POPUPOPEN);

		UnregisterObject(popup);

		popup->internalRequestClose.Disconnect(&MenubarEntry::ClosePopupMenu, this);

		Window	*window	= container->GetContainerWindow();
		Rect	 frame	= Rect(GetRealPosition(), GetSize());

		if (!window->IsMouseOn(frame)) Paint(SP_MOUSEOUT);

		hotspot->Activate();
	}
}

S::Bool S::GUI::MenubarEntry::IsTypeCompatible(Int compType) const
{
	if (compType == Object::classID || compType == Widget::classID || compType == MenuEntry::classID)	return True;
	else													return False;
}
