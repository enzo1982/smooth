 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_MDICLIENT_
#define __OBJSMOOTH_MDICLIENT_

#include <smooth/mdiclient.h>
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

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_MDICLIENT = SMOOTH::RequestObjectID();

SMOOTHMDIClient::SMOOTHMDIClient()
{
	type				= OBJ_MDICLIENT;
	objectProperties->orientation	= OR_CENTER;

	possibleContainers.AddEntry(OBJ_WINDOW);
}

SMOOTHMDIClient::~SMOOTHMDIClient()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHMDIClient::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHObject		*object;
	SMOOTHDivisionbar	*db;
	SMOOTHRect		 client;
	SMOOTHRect		 updateRect = wnd->GetUpdateRect();
	HDC			 dc = GetContext(wnd);

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
					if (IsBitSet(db->orientation, OR_LEFT))
					{
						if (db->GetObjectProperties()->pos.x >= client.left - 3) client.left = db->GetObjectProperties()->pos.x + 5;
					}
					else
					{
						if (db->GetObjectProperties()->pos.x <= client.right + 1) client.right = wnd->GetObjectProperties()->size.cx - db->GetObjectProperties()->pos.x - 2;
					}
				}
				else if (IsBitSet(db->orientation, OR_HORZ))
				{
					if (IsBitSet(db->orientation, OR_TOP))
					{
						if (db->GetObjectProperties()->pos.y >= client.top - 2) client.top = db->GetObjectProperties()->pos.y + 5;
					}
					else
					{
						if (db->GetObjectProperties()->pos.y <= client.bottom + 1) client.bottom = wnd->GetObjectProperties()->size.cy - db->GetObjectProperties()->pos.y - 2;
					}
				}
			}
		}
	}

	if (DoRectsOverlap(updateRect, client))
	{
		SMOOTHRect	 intersectRect;

		client.bottom--;
		client.right--;

		updateRect.right += 5;
		updateRect.bottom += 5;

#ifdef __WIN32__
		RECT	 iRect = intersectRect;
		RECT	 uRect = updateRect;
		RECT	 cRect = client;

		IntersectRect(&iRect, &uRect, &cRect);

		intersectRect = iRect;
#endif

		Box(dc, intersectRect, SMOOTH::Setup::DividerDarkColor, FILLED);

		Frame(dc, client, FRAME_DOWN);
	}

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

#endif
