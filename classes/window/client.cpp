 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/client.h>
#include <smooth/window.h>
#include <smooth/toolkit.h>
#include <smooth/object.h>
#include <smooth/divider.h>
#include <smooth/definitions.h>
#include <smooth/metrics.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/surface.h>
#include <smooth/binary.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_CLIENT = S::Object::RequestObjectID();

S::GUI::Client::Client()
{
	type				= OBJ_CLIENT;
	objectProperties->orientation	= OR_CENTER;

	possibleContainers.AddEntry(OBJ_WINDOW);
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
	if (wnd->hwnd == NIL) return Success;

	EnterProtectedRegion();

	Object	*object;
	Divider	*db;
	Rect	 clientFrame;
	Rect	 client;

	updateRect = wnd->GetUpdateRect();

	client.left	= objectProperties->pos.x + 2;
	client.top	= objectProperties->pos.y + 2;
	client.right	= objectProperties->size.cx + objectProperties->pos.x - 1;
	client.bottom	= objectProperties->size.cy + objectProperties->pos.y - 2;

	for (Int i = Object::objectCount - 1; i >= 0; i--)
	{
		object = mainObjectManager->RequestObject(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == OBJ_DIVIDER && object->GetContainer() == myContainer)
			{
				db = (Divider *) object;

				if (Binary::IsFlagSet(db->orientation, OR_VERT))
				{
					if (Binary::IsFlagSet(db->orientation, OR_LEFT) && db->GetObjectProperties()->pos.x >= client.left - 3)		client.left = db->GetObjectProperties()->pos.x + 5;
					else if (!Binary::IsFlagSet(db->orientation, OR_LEFT) && db->GetObjectProperties()->pos.x <= client.right + 1)	client.right = wnd->GetObjectProperties()->size.cx - db->GetObjectProperties()->pos.x - 2;
				}
				else if (Binary::IsFlagSet(db->orientation, OR_HORZ))
				{
					if (Binary::IsFlagSet(db->orientation, OR_TOP) && db->GetObjectProperties()->pos.y >= client.top - 2)		client.top = db->GetObjectProperties()->pos.y + 5;
					else if (!Binary::IsFlagSet(db->orientation, OR_TOP) && db->GetObjectProperties()->pos.y <= client.bottom + 1)	client.bottom = wnd->GetObjectProperties()->size.cy - db->GetObjectProperties()->pos.y - 2;
				}
			}
		}
	}

	if (DoRectsOverlap(updateRect, client))
	{
		Rect	 intersectRect;

		updateRect.right += 5;
		updateRect.bottom += 5;

#ifdef __WIN32__
		RECT	 iRect = intersectRect;
		RECT	 uRect = updateRect;
		RECT	 cRect = client;

		IntersectRect(&iRect, &uRect, &cRect);

		intersectRect = iRect;
#endif

		surface->Box(intersectRect, Setup::ClientColor, FILLED);

		clientFrame.left	= client.left;
		clientFrame.top		= client.top;
		clientFrame.right	= client.right - 1;
		clientFrame.bottom	= client.bottom - 1;

		surface->Box(clientFrame, Setup::DividerDarkColor, OUTLINED);

		updateRect.left		-= (objectProperties->pos.x + 3);
		updateRect.top		-= (objectProperties->pos.y + 3);
		updateRect.right	-= (objectProperties->pos.x + 3);
		updateRect.bottom	-= (objectProperties->pos.y + 3);

		updateRect.left		= max(0, updateRect.left);
		updateRect.top		= max(0, updateRect.top);
		updateRect.right	= min(GetSize().cx - 1, updateRect.right) + 5;
		updateRect.bottom	= min(GetSize().cy - 1, updateRect.bottom) + 5;

		onPaint.Emit();
	}
	else
	{
		updateRect = Rect(Point(0, 0), Size(0, 0));
	}

	LeaveProtectedRegion();

	return Success;
}

S::Rect S::GUI::Client::GetUpdateRect()
{
	return updateRect;
}

S::Size S::GUI::Client::GetSize()
{
	return Size(objectProperties->size.cx - 6, objectProperties->size.cy - 7);
}

S::Int S::GUI::Client::BlitFromBitmap(HBITMAP bitmap, Rect srcRect, Rect destRect)
{
	Surface	*surface = myContainer->GetDrawSurface();

	destRect.left	+= (objectProperties->pos.x + 3);
	destRect.top	+= (objectProperties->pos.y + 3);
	destRect.right	+= (objectProperties->pos.x + 3);
	destRect.bottom	+= (objectProperties->pos.y + 3);

	return surface->BlitFromBitmap(bitmap, srcRect, destRect);
}

S::Int S::GUI::Client::BlitToBitmap(Rect srcRect, HBITMAP bitmap, Rect destRect)
{
	Surface	*surface = myContainer->GetDrawSurface();

	srcRect.left	+= (objectProperties->pos.x + 3);
	srcRect.top	+= (objectProperties->pos.y + 3);
	srcRect.right	+= (objectProperties->pos.x + 3);
	srcRect.bottom	+= (objectProperties->pos.y + 3);

	return surface->BlitToBitmap(srcRect, bitmap, destRect);
}
