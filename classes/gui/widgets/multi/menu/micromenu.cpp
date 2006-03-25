 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/multi/menu/micromenu.h>
#include <smooth/gui/widgets/multi/menu/popupmenu.h>
#include <smooth/gui/widgets/hotspot/simplebutton.h>
#include <smooth/gui/window/window.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/math.h>

const S::Int	 S::GUI::MicroMenu::classID = S::Object::RequestClassID();

S::GUI::MicroMenu::MicroMenu(const Point &iPos, const Size &iSize, Int sType)
{
	type	= classID;
	subtype	= sType;

	SetMetrics(iPos, iSize);

	if (GetWidth() == 0) SetWidth(subtype == OR_VERT ? 13 : 7);
	if (GetHeight() == 0) SetHeight(subtype == OR_VERT ? 7 : 13);

	onOpenPopupMenu.SetParentObject(this);
	onClosePopupMenu.SetParentObject(this);

	hotspot	= new HotspotSimpleButton(Point(), GetSize());

	hotspot->onLeftButtonDown.Connect(&MicroMenu::OpenPopupMenu, this);

	RegisterObject(hotspot);

	popup = new PopupMenu();
}

S::GUI::MicroMenu::~MicroMenu()
{
	DeleteObject(popup);

	DeleteObject(hotspot);
}

S::GUI::MenuEntry *S::GUI::MicroMenu::AddEntry(const String &text, const Bitmap &bitmap, PopupMenu *popupMenu, Bool *bVar, Int *iVar, Int iCode)
{
	return popup->AddEntry(text, bitmap, popupMenu, bVar, iVar, iCode);
}

S::Int S::GUI::MicroMenu::RemoveEntry(MenuEntry *entry)
{
	return popup->RemoveEntry(entry);
}

S::Int S::GUI::MicroMenu::RemoveAllEntries()
{
	return popup->RemoveAllEntries();
}

S::Int S::GUI::MicroMenu::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	EnterProtectedRegion();

	Surface	*surface = container->GetDrawSurface();
	Rect	 frame	 = Rect(GetRealPosition(), GetSize());

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			if (GetBackgroundColor() >= 0) surface->Box(frame, GetBackgroundColor(), FILLED);

			surface->Frame(frame, FRAME_UP);

			if (subtype == OR_HORZ)
			{
				for (Int i = 0; i < 3; i++)
				{
					Point	 lineStart	= Point(frame.left + GetWidth() / 2 - 1 + i, frame.top + GetHeight() / 2 - 2 + i);
					Point	 lineEnd	= lineStart + Point(0, 5 - 2 * i);

					surface->Line(lineStart, lineEnd, Color(0, 0, 0));
				}
			}
			else if (subtype == OR_VERT)
			{
				for (Int i = 0; i < 3; i++)
				{
					Point	 lineStart	= Point(frame.left + GetWidth() / 2 - 2 + (Setup::rightToLeft ? 1 : 0) + i, frame.top + GetHeight() / 2 - 1 + i);
					Point	 lineEnd	= lineStart + Point(5 - 2 * i, 0);

					surface->Line(lineStart, lineEnd, Color(0, 0, 0));
				}
			}

			break;
		case SP_MOUSEOUT:
			if (hotspot->IsActive()) surface->Frame(frame, FRAME_UP);

			break;
	}

	LeaveProtectedRegion();

	return Success();
}

S::Void S::GUI::MicroMenu::OpenPopupMenu()
{
	hotspot->Deactivate();

	Widget	*window		= container->GetContainerWindow();
	Point	 realPos	= GetRealPosition();
	Point	 popupPos	= realPos + Point(subtype == OR_HORZ ? GetWidth() : 0, subtype == OR_VERT ? GetHeight() : 0);

	if (window->GetX() + popupPos.x + popup->GetWidth() >= LiSAGetDisplaySizeX()) popupPos.x = realPos.x - popup->GetWidth() + (subtype == OR_VERT ? GetWidth() : 0);
	if (window->GetY() + popupPos.y + popup->GetHeight() >= LiSAGetDisplaySizeY()) popupPos.y = realPos.y - popup->GetHeight() + (subtype == OR_HORZ ? GetHeight() : 0);

	popup->SetPosition(popupPos);
	popup->internalRequestClose.Connect(&MicroMenu::ClosePopupMenu, this);

	onOpenPopupMenu.Emit();

	RegisterObject(popup);
}

S::Void S::GUI::MicroMenu::ClosePopupMenu()
{
	if (popup->GetContainer() == this)
	{
		UnregisterObject(popup);

		onClosePopupMenu.Emit();

		popup->internalRequestClose.Disconnect(&MicroMenu::ClosePopupMenu, this);

		hotspot->Activate();
	}
}
