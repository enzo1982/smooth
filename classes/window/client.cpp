 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_CLIENT_
#define __OBJSMOOTH_CLIENT_

#include <smooth/client.h>
#include <smooth/window.h>
#include <smooth/toolkit.h>
#include <smooth/object.h>
#include <smooth/divisionbar.h>
#include <smooth/definitions.h>
#include <smooth/metrics.h>
#include <smooth/loop.h>
#include <smooth/objectmanager.h>
#include <smooth/binary.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_CLIENT = SMOOTH::RequestObjectID();

SMOOTHClient::SMOOTHClient(SMOOTHProcParam, SMOOTHVoid *procParam)
{
	type				= OBJ_CLIENT;
	objectProperties->proc		= (SMOOTHProcType) newProc;
	objectProperties->procParam	= procParam;
	objectProperties->orientation	= OR_CENTER;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

SMOOTHClient::~SMOOTHClient()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHClient::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHSurface		*surface = myContainer->GetDrawSurface();
	SMOOTHWindow		*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	EnterProtectedRegion();

	SMOOTHObject		*object;
	SMOOTHDivisionbar	*db;
	SMOOTHRect		 clientFrame;
	SMOOTHRect		 client;

	updateRect = wnd->GetUpdateRect();

	client.left	= objectProperties->pos.x + 2;
	client.top	= objectProperties->pos.y + 2;
	client.right	= objectProperties->size.cx + objectProperties->pos.x - 1;
	client.bottom	= objectProperties->size.cy + objectProperties->pos.y - 2;

	for (SMOOTHInt i = SMOOTHObject::objectCount - 1; i >= 0; i--)
	{
		object = mainObjectManager->RequestObject(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == OBJ_DIVISIONBAR && object->GetContainer() == myContainer)
			{
				db = (SMOOTHDivisionbar *) object;

				if (IsBitSet(db->orientation, OR_VERT))
				{
					if (IsBitSet(db->orientation, OR_LEFT) && db->GetObjectProperties()->pos.x >= client.left - 3)		client.left = db->GetObjectProperties()->pos.x + 5;
					else if (!IsBitSet(db->orientation, OR_LEFT) && db->GetObjectProperties()->pos.x <= client.right + 1)	client.right = wnd->GetObjectProperties()->size.cx - db->GetObjectProperties()->pos.x - 2;
				}
				else if (IsBitSet(db->orientation, OR_HORZ))
				{
					if (IsBitSet(db->orientation, OR_TOP) && db->GetObjectProperties()->pos.y >= client.top - 2)		client.top = db->GetObjectProperties()->pos.y + 5;
					else if (!IsBitSet(db->orientation, OR_TOP) && db->GetObjectProperties()->pos.y <= client.bottom + 1)	client.bottom = wnd->GetObjectProperties()->size.cy - db->GetObjectProperties()->pos.y - 2;
				}
			}
		}
	}

	if (DoRectsOverlap(updateRect, client))
	{
		SMOOTHRect	 intersectRect;

		updateRect.right += 5;
		updateRect.bottom += 5;

#ifdef __WIN32__
		RECT	 iRect = intersectRect;
		RECT	 uRect = updateRect;
		RECT	 cRect = client;

		IntersectRect(&iRect, &uRect, &cRect);

		intersectRect = iRect;
#endif

		surface->Box(intersectRect, SMOOTH::Setup::ClientColor, FILLED);

		clientFrame.left	= client.left;
		clientFrame.top		= client.top;
		clientFrame.right	= client.right - 1;
		clientFrame.bottom	= client.bottom - 1;

		surface->Box(clientFrame, SMOOTH::Setup::DividerDarkColor, OUTLINED);

		updateRect.left		-= (objectProperties->pos.x + 3);
		updateRect.top		-= (objectProperties->pos.y + 3);
		updateRect.right	-= (objectProperties->pos.x + 3);
		updateRect.bottom	-= (objectProperties->pos.y + 3);

		updateRect.left		= max(0, updateRect.left);
		updateRect.top		= max(0, updateRect.top);
		updateRect.right	= min(GetSize().cx - 1, updateRect.right) + 5;
		updateRect.bottom	= min(GetSize().cy - 1, updateRect.bottom) + 5;

		SMOOTHProcCall(objectProperties->proc, objectProperties->procParam);
	}
	else
	{
		updateRect = SMOOTHRect(SMOOTHPoint(0, 0), SMOOTHSize(0, 0));
	}

	LeaveProtectedRegion();

	return SMOOTH::Success;
}

SMOOTHRect SMOOTHClient::GetUpdateRect()
{
	return updateRect;
}

SMOOTHSize SMOOTHClient::GetSize()
{
	return SMOOTHSize(objectProperties->size.cx - 6, objectProperties->size.cy - 7);
}

SMOOTHInt SMOOTHClient::BlitFromBitmap(HBITMAP bitmap, SMOOTHRect srcRect, SMOOTHRect destRect)
{
	SMOOTHSurface	*surface = myContainer->GetDrawSurface();

	destRect.left	+= (objectProperties->pos.x + 3);
	destRect.top	+= (objectProperties->pos.y + 3);
	destRect.right	+= (objectProperties->pos.x + 3);
	destRect.bottom	+= (objectProperties->pos.y + 3);

	return surface->BlitFromBitmap(bitmap, srcRect, destRect);
}

SMOOTHInt SMOOTHClient::BlitToBitmap(SMOOTHRect srcRect, HBITMAP bitmap, SMOOTHRect destRect)
{
	SMOOTHSurface	*surface = myContainer->GetDrawSurface();

	srcRect.left	+= (objectProperties->pos.x + 3);
	srcRect.top	+= (objectProperties->pos.y + 3);
	srcRect.right	+= (objectProperties->pos.x + 3);
	srcRect.bottom	+= (objectProperties->pos.y + 3);

	return surface->BlitToBitmap(srcRect, bitmap, destRect);
}

#endif
