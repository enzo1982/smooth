 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/system/system.h>

const S::Short	 S::GUI::MenubarEntry::classID = S::Object::RequestClassID();

S::GUI::MenubarEntry::MenubarEntry(const String &iText, const Bitmap &iBitmap, PopupMenu *iPopup, Bool *ibVar, Int *iiVar, Int iiCode) : MenuEntry(iText, iBitmap, iPopup, ibVar, iiVar, iiCode)
{
	type		= classID;

	if	(text == NIL && bitmap == NIL) SetSize(Size(4, 15));
	else if (text != NIL && bitmap == NIL) SetSize(Size(unscaledTextSize.cx + 7, 16));
	else if (text == NIL && bitmap != NIL) SetSize(bitmap.GetSize() + Size(4 + (popup != NIL ? 12 : 0), 4));

	hotspot		= NIL;
	actionHotspot	= NIL;

	if (text != NIL || bitmap != NIL)
	{
		hotspot	= new HotspotSimpleButton(Point(), GetSize());
		hotspot->onMouseOver.Connect(&MenubarEntry::OnMouseOver, this);
		hotspot->onLeftButtonDown.Connect(&MenubarEntry::OpenPopupMenu, this);

		actionHotspot = new Hotspot(Point(), GetSize() - Size((text == NIL && bitmap != NIL && popup != NIL ? 12 : 0), 0));
		actionHotspot->onLeftButtonClick.Connect(&onAction);

		if (text == NIL && bitmap != NIL && popup != NIL) actionHotspot->onLeftButtonDown.Connect(&MenubarEntry::ClosePopupMenu, this);

		Add(hotspot);
		Add(actionHotspot);
	}

	popupMenuClosed	= 0;

	onChangeSize.Connect(&MenubarEntry::OnChangeSize, this);
}

S::GUI::MenubarEntry::~MenubarEntry()
{
	if (hotspot != NIL) DeleteObject(hotspot);
	if (actionHotspot != NIL) DeleteObject(actionHotspot);
}

S::Int S::GUI::MenubarEntry::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	Surface	*surface  = GetDrawSurface();
	Point	 realPos  = GetRealPosition();
	Size	 realSize = GetRealSize();
	Rect	 bmpRect  = Rect(realPos + Point(2, 2) * surface->GetSurfaceDPI() / 96.0, (GetSize() - Size(4 + (popup != NIL ? 12 : 0), 4)) * surface->GetSurfaceDPI() / 96.0);

	switch (message)
	{
		case SP_PAINT:
			surface->StartPaint(Rect(realPos, realSize));

			if (text == NIL && bitmap == NIL)
			{
				if (orientation == OR_LEFT || orientation == OR_RIGHT)
				{
					SetWidth(4);

					Point	 p1 = Point(realPos.x, realPos.y);
					Point	 p2 = Point(p1.x, p1.y + realSize.cy);

					surface->Bar(p1, p2, OR_VERT);

					p1.x += 2;
					p2.x += 2;

					surface->Bar(p1, p2, OR_VERT);
				}
				else if (orientation == OR_TOP || orientation == OR_BOTTOM)
				{
					SetHeight(4);

					Point	 p1 = Point(realPos.x, realPos.y);
					Point	 p2 = Point(p1.x + realSize.cx, p1.y);

					surface->Bar(p1, p2, OR_HORZ);

					p1.y += 2;
					p2.y += 2;

					surface->Bar(p1, p2, OR_HORZ);
				}
			}
			else if (text != NIL && bitmap == NIL)
			{
				Rect	 textRect = Rect(realPos + Point(1, Math::Ceil(Float(realSize.cy - scaledTextSize.cy) / 2) - 1) + Point(2, 0) * surface->GetSurfaceDPI() / 96.0, realSize - Size(2, 2) - Size(2, 0) * surface->GetSurfaceDPI() / 96.0);
				Font	 nFont	  = font;

				if (!IsActive()) nFont.SetColor(Setup::InactiveTextColor);

				surface->Box(textRect, GetBackgroundColor(), Rect::Filled);
				surface->SetText(text, textRect, nFont);
			}
			else if (text == NIL && bitmap != NIL)
			{
				if (flags & MB_COLOR)	surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
				else			surface->BlitFromBitmap(graymap, Rect(Point(0, 0), graymap.GetSize()), bmpRect);

				if (popup != NIL)
				{
					Int	 height	   = Math::Round(3 * surface->GetSurfaceDPI() / 96.0);

					Point	 lineStart = realPos + Point(realSize.cx - height * 2 - 2 * surface->GetSurfaceDPI() / 96.0 - (IsRightToLeft() ? 0 : 1), (realSize.cy - height) / 2 + 1);
					Point	 lineEnd   = lineStart + Point(height * 2 - 1, 0);

					for (Int i = 0; i < height; i++)
					{
						surface->Line(lineStart, lineEnd, Setup::TextColor);

						lineStart += Point(1, 1);
						lineEnd	  += Point(-1, 1);
					}
				}
			}

			surface->EndPaint();

			break;
		case SP_MOUSEIN:
		case SP_MOUSEUP:
			if (focussed && popup != NIL) if (popup->GetContainer() == this) break;

			if (bitmap != NIL)
			{
				surface->StartPaint(Rect(realPos, realSize));
				surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);

				if (onAction.GetNOfConnectedSlots() > 0 && popup != NIL)
				{
					Point	 p1 = Point(realPos.x + realSize.cx - 11 * surface->GetSurfaceDPI() / 96.0 - 2, realPos.y + 1);
					Point	 p2 = Point(realPos.x + realSize.cx - 11 * surface->GetSurfaceDPI() / 96.0 - 2, realPos.y + realSize.cy - 2);

					surface->Bar(p1, p2, OR_VERT);
				}

				surface->EndPaint();
			}

			break;
		case SP_MOUSEOUT:
			if (focussed && popup != NIL) if (popup->GetContainer() == this) break;

			if (bitmap != NIL)
			{
				surface->StartPaint(Rect(realPos, realSize));

				if (flags & MB_COLOR) surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
				else		      surface->BlitFromBitmap(graymap, Rect(Point(0, 0), graymap.GetSize()), bmpRect);

				if (onAction.GetNOfConnectedSlots() > 0 && popup != NIL)
				{
					Point	 p1 = Point(realPos.x + realSize.cx - 11 * surface->GetSurfaceDPI() / 96.0 - 2 + (IsRightToLeft() ? 1 : 0), realPos.y + 1);
					Point	 p2 = Point(realPos.x + realSize.cx - 11 * surface->GetSurfaceDPI() / 96.0 - 2 + (IsRightToLeft() ? 1 : 0), realPos.y + realSize.cy - 1);

					surface->Line(p1, p2, Setup::BackgroundColor);

					p1.x++;
					p2.x++;

					surface->Line(p1, p2, Setup::BackgroundColor);
				}

				surface->EndPaint();
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

S::Void S::GUI::MenubarEntry::OnChangeSize(const Size &nSize)
{
	if (hotspot == NIL || actionHotspot == NIL) return;

	hotspot->SetSize(nSize);
	actionHotspot->SetSize(nSize - Size((text == NIL && bitmap != NIL && popup != NIL ? 12 : 0), 0));
}

S::Void S::GUI::MenubarEntry::OpenPopupMenu()
{
	if (popup == NIL) return;

	if (S::System::System::Clock() - popupMenuClosed < 100)
	{
		hotspot->Paint(SP_MOUSEUP);

		return;
	}

	Window	*window	    = container->GetContainerWindow();
	Surface	*surface    = GetDrawSurface();
	Size	 realSize   = GetRealSize();
	Rect	 popupFrame = Rect(GetRealPosition() + Point(realSize.cx - 11 * surface->GetSurfaceDPI() / 96.0, 0), Size(11 * surface->GetSurfaceDPI() / 96.0, realSize.cy));

	if (onAction.GetNOfConnectedSlots() == 0 || window->IsMouseOn(popupFrame))
	{
		hotspot->Deactivate();

		popup->CalculateSize();

		popup->SetPosition(GetRealPosition() + Point(orientation == OR_LEFT ? -1 : realSize.cx + 2 - popup->GetWidth() * surface->GetSurfaceDPI() / 96.0, realSize.cy + 1));
		popup->internalRequestClose.Connect(&MenubarEntry::ClosePopupMenu, this);

		Add(popup);

		container->SetFlags(container->GetFlags() | MB_POPUPOPEN);
	}
}

S::Void S::GUI::MenubarEntry::ClosePopupMenu()
{
	if (popup == NIL || popup->GetContainer() != this) return;

	if (IsRegistered()) container->SetFlags(container->GetFlags() & ~MB_POPUPOPEN);

	Remove(popup);

	popup->internalRequestClose.Disconnect(&MenubarEntry::ClosePopupMenu, this);

	if (IsRegistered())
	{
		Window	*window	 = container->GetContainerWindow();
		Surface	*surface = GetDrawSurface();
		Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

		surface->StartPaint(frame);

		if (!window->IsMouseOn(frame)) Paint(SP_MOUSEOUT);

		hotspot->Activate();

		surface->EndPaint();
	}

	popupMenuClosed = S::System::System::Clock();
}

S::Bool S::GUI::MenubarEntry::IsTypeCompatible(Short compType) const
{
	if (compType == MenuEntry::classID) return True;
	else				    return MenuEntry::IsTypeCompatible(compType);
}
