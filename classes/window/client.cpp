 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/client.h>
#include <smooth/graphics/window.h>
#include <smooth/toolkit.h>
#include <smooth/object.h>
#include <smooth/divider.h>
#include <smooth/definitions.h>
#include <smooth/metrics.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/graphics/surface.h>
#include <smooth/binary.h>

const S::Int	 S::GUI::Client::classID = S::Object::RequestClassID();

S::GUI::Client::Client()
{
	type				= classID;
	objectProperties->orientation	= OR_CENTER;

	possibleContainers.AddEntry(Window::classID);

	onPaint.SetParentObject(this);
}

S::GUI::Client::~Client()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Client::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();
	Window	*wnd = (Window *) myContainer->GetContainerObject();

	if (wnd == NIL) return Success;

	EnterProtectedRegion();

	Object	*object;
	Divider	*db;
	Rect	 clientFrame;
	Rect	 client;
	Rect	 updateRect = wnd->GetUpdateRect();

	client.left	= objectProperties->pos.x + 2;
	client.top	= objectProperties->pos.y + 2;
	client.right	= objectProperties->size.cx + objectProperties->pos.x - 1;
	client.bottom	= objectProperties->size.cy + objectProperties->pos.y - 2;

	for (Int i = mainObjectManager->GetNOfObjects() - 1; i >= 0; i--)
	{
		object = mainObjectManager->GetNthObject(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == Divider::classID && object->GetContainer() == myContainer)
			{
				db = (Divider *) object;

				if (Binary::IsFlagSet(db->GetFlags(), OR_VERT))
				{
					if (Binary::IsFlagSet(db->GetFlags(), OR_LEFT) && db->GetObjectProperties()->pos.x >= client.left - 3)		client.left = db->GetObjectProperties()->pos.x + 5;
					else if (!Binary::IsFlagSet(db->GetFlags(), OR_LEFT) && db->GetObjectProperties()->pos.x <= client.right + 1)	client.right = wnd->GetObjectProperties()->size.cx - db->GetObjectProperties()->pos.x - 2;
				}
				else if (Binary::IsFlagSet(db->GetFlags(), OR_HORZ))
				{
					if (Binary::IsFlagSet(db->GetFlags(), OR_TOP) && db->GetObjectProperties()->pos.y >= client.top - 2)		client.top = db->GetObjectProperties()->pos.y + 5;
					else if (!Binary::IsFlagSet(db->GetFlags(), OR_TOP) && db->GetObjectProperties()->pos.y <= client.bottom + 1)	client.bottom = wnd->GetObjectProperties()->size.cy - db->GetObjectProperties()->pos.y - 2;
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
		if (DoRectsOverlap(updateRect, client))
		{
			Rect	 intersectRect;

			updateRect.right += 5;
			updateRect.bottom += 5;

			RECT	 iRect = intersectRect;
			RECT	 uRect = updateRect;
			RECT	 cRect = client;

			IntersectRect(&iRect, &uRect, &cRect);

			intersectRect = iRect;

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

S::Size S::GUI::Client::GetSize()
{
	return Size(objectProperties->size.cx - 6, objectProperties->size.cy - 7);
}
