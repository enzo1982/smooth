 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/mdi/client.h>
#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/basic/divider.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/binary.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::MDI::Client::classID = S::Object::RequestClassID();

S::GUI::MDI::Client::Client()
{
	type		= classID;
	orientation	= OR_CENTER;

	possibleContainers.AddEntry(GUI::Window::classID);
}

S::GUI::MDI::Client::~Client()
{
}

S::Int S::GUI::MDI::Client::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface		*surface = container->GetDrawSurface();
	GUI::Window	*wnd = container->GetContainerWindow();

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
		surface->Box(client, Setup::DividerDarkColor, FILLED);

		clientFrame.left	= client.left;
		clientFrame.top		= client.top;
		clientFrame.right	= client.right - 1;
		clientFrame.bottom	= client.bottom - 1;

		surface->Frame(client, FRAME_DOWN);
	}
	else
	{
		if (Rect::DoRectsOverlap(updateRect, client) || (updateRect.left == 0 && updateRect.top == 0 && updateRect.right == 0 && updateRect.bottom == 0))
		{
			updateRect.right += 5;
			updateRect.bottom += 5;

			Rect	 intersectRect = Rect::OverlapRect(updateRect, client);

			surface->Box(intersectRect, Setup::DividerDarkColor, FILLED);

			clientFrame.left	= client.left;
			clientFrame.top		= client.top;
			clientFrame.right	= client.right - 1;
			clientFrame.bottom	= client.bottom - 1;

			surface->Frame(client, FRAME_DOWN);
		}
	}

	LeaveProtectedRegion();

	return Success;
}
