 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/client.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/basic/divider.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/graphics/surface.h>
#include <smooth/misc/binary.h>

const S::Int	 S::GUI::Client::classID = S::Object::RequestClassID();

S::GUI::Client::Client()
{
	type		= classID;
	orientation	= OR_CENTER;

	possibleContainers.AddEntry(Window::classID);

	onPaint.SetParentObject(this);
}

S::GUI::Client::~Client()
{
	if (registered && container != NIL) container->UnregisterObject(this);
}

S::Int S::GUI::Client::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface	*surface	= container->GetDrawSurface();
	Window	*wnd		= container->GetContainerWindow();

	if (wnd == NIL) return Success;

	EnterProtectedRegion();

	Object	*object;
	Divider	*db;
	Rect	 clientFrame;
	Rect	 client;
	Rect	 updateRect = wnd->GetUpdateRect();

	client.left	= pos.x + 2;
	client.top	= pos.y + 2;
	client.right	= size.cx + pos.x - 1;
	client.bottom	= size.cy + pos.y - 2;

	for (Int i = Object::GetNOfObjects() - 1; i >= 0; i--)
	{
		object = Object::GetNthObject(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == Divider::classID)
			{
				if (((Widget *) object)->GetContainer() == container)
				{
					db = (Divider *) object;

					if (Binary::IsFlagSet(db->GetFlags(), OR_VERT))
					{
						if (Binary::IsFlagSet(db->GetFlags(), OR_LEFT) && db->pos.x >= client.left - 3)		client.left = db->pos.x + 5;
						else if (!Binary::IsFlagSet(db->GetFlags(), OR_LEFT) && db->pos.x <= client.right + 1)	client.right = wnd->size.cx - db->pos.x - 2;
					}
					else if (Binary::IsFlagSet(db->GetFlags(), OR_HORZ))
					{
						if (Binary::IsFlagSet(db->GetFlags(), OR_TOP) && db->pos.y >= client.top - 2)		client.top = db->pos.y + 5;
						else if (!Binary::IsFlagSet(db->GetFlags(), OR_TOP) && db->pos.y <= client.bottom + 1)	client.bottom = wnd->size.cy - db->pos.y - 2;
					}
				}
			}
		}
	}

	if (message == SP_SHOW)
	{
		surface->Box(client, Setup::ClientColor, FILLED);

		clientFrame.left	= client.left;
		clientFrame.top		= client.top;
		clientFrame.right	= client.right - 1;
		clientFrame.bottom	= client.bottom - 1;

		surface->Box(clientFrame, Setup::DividerDarkColor, OUTLINED);

		onPaint.Emit();
	}
	else
	{
		if (Rect::DoRectsOverlap(updateRect, client))
		{
			updateRect.right += 5;
			updateRect.bottom += 5;

			Rect	 intersectRect = Rect::OverlapRect(updateRect, client);

			surface->Box(intersectRect, Setup::ClientColor, FILLED);

			clientFrame.left	= client.left;
			clientFrame.top		= client.top;
			clientFrame.right	= client.right - 1;
			clientFrame.bottom	= client.bottom - 1;

			surface->Box(clientFrame, Setup::DividerDarkColor, OUTLINED);

			onPaint.Emit();
		}
	}

	LeaveProtectedRegion();

	return Success;
}

S::GUI::Size S::GUI::Client::GetSize()
{
	return Size(size.cx - 6, size.cy - 7);
}
