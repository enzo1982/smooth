 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_DIVISIONBAR_
#define __OBJSMOOTH_DIVISIONBAR_

#include <smooth/divisionbar.h>
#include <smooth/objectmanager.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/metrics.h>
#include <smooth/mathtools.h>
#include <smooth/binary.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_DIVISIONBAR = SMOOTH::RequestObjectID();

SMOOTHDivisionbar::SMOOTHDivisionbar(SMOOTHInt pos, SMOOTHInt iOrientation)
{
	type				= OBJ_DIVISIONBAR;
	orientation			= iOrientation;
	objectProperties->orientation	= OR_FREE;

	if (pos == 0)	pos = roundtoint(120 * SMOOTH::Setup::FontSize);
	else		pos = roundtoint(pos * SMOOTH::Setup::FontSize);

	possibleContainers.AddEntry(OBJ_WINDOW);
	possibleContainers.AddEntry(OBJ_LAYER);

	if (IsBitSet(orientation, OR_HORZ))		{ objectProperties->pos.x = 0; objectProperties->pos.y = pos; }
	else if (IsBitSet(orientation, OR_VERT))	{ objectProperties->pos.x = pos; objectProperties->pos.y = 0; }
}

SMOOTHDivisionbar::~SMOOTHDivisionbar()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHDivisionbar::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHObject		*container = NIL;
	SMOOTHWindow		*wnd = NIL;
	SMOOTHLayer		*layer = NIL;

	if (myContainer->GetContainerObject()->GetObjectType() == OBJ_WINDOW)
	{
		wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

		container = wnd;
	}
	else if (myContainer->GetContainerObject()->GetObjectType() == OBJ_LAYER)
	{
		layer = (SMOOTHLayer *) myContainer->GetContainerObject();
		wnd = (SMOOTHWindow *) layer->GetContainer()->GetContainerObject();

		container = layer;
	}

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHObject		*object;
	SMOOTHDivisionbar	*operat;
	HDC			 dc = GetContext(wnd);
	SMOOTHPoint		 doubleBar1;
	SMOOTHPoint		 doubleBar2;
	bool			 afterMe = SMOOTH::False;

	if (IsBitSet(orientation, OR_HORZ))
	{
		if (container->GetObjectType() == OBJ_WINDOW)
		{
			doubleBar1.x = 4;

			if (IsBitSet(orientation, OR_TOP))		doubleBar1.y = objectProperties->pos.y;
			else if	(IsBitSet(orientation, OR_BOTTOM))	doubleBar1.y = wnd->GetObjectProperties()->size.cy - objectProperties->pos.y;

			doubleBar2.x = wnd->GetObjectProperties()->size.cx - 4;
			doubleBar2.y = doubleBar1.y;

			for (SMOOTHInt i = SMOOTHObject::objectCount - 1; i >= 0; i--)
			{
				object = mainObjectManager->RequestObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == OBJ_DIVISIONBAR && object->GetContainer() == myContainer)
					{
						operat = (SMOOTHDivisionbar *) object;

						if (afterMe && IsBitSet(operat->orientation, OR_VERT))
						{
							if (IsBitSet(operat->orientation, OR_LEFT))
							{
								if (operat->GetObjectProperties()->pos.x >= doubleBar1.x-2) doubleBar1.x = operat->GetObjectProperties()->pos.x + 3;
							}
							else
							{
								if (operat->GetObjectProperties()->pos.x <= doubleBar2.x+1) doubleBar2.x = wnd->GetObjectProperties()->size.cx - operat->GetObjectProperties()->pos.x - 2;
							}
						}

						if (operat == this) afterMe = SMOOTH::True;
					}
				}
			}
		}
		else if (container->GetObjectType() == OBJ_LAYER)
		{
			doubleBar1.x = layer->GetObjectProperties()->pos.x + 4;

			if (IsBitSet(orientation, OR_TOP))		doubleBar1.y = layer->GetObjectProperties()->pos.y + objectProperties->pos.y;
			else if	(IsBitSet(orientation, OR_BOTTOM))	doubleBar1.y = layer->GetObjectProperties()->pos.y + layer->GetObjectProperties()->size.cy - objectProperties->pos.y;

			doubleBar2.x = layer->GetObjectProperties()->pos.x + layer->GetObjectProperties()->size.cx - 4;
			doubleBar2.y = doubleBar1.y;

			for (SMOOTHInt i = SMOOTHObject::objectCount - 1; i >= 0; i--)
			{
				object = mainObjectManager->RequestObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == OBJ_DIVISIONBAR && object->GetContainer() == myContainer)
					{
						operat = (SMOOTHDivisionbar *) object;

						if (afterMe && IsBitSet(operat->orientation, OR_VERT))
						{
							if (IsBitSet(operat->orientation, OR_LEFT))
							{
								if (layer->GetObjectProperties()->pos.x + operat->GetObjectProperties()->pos.x >= doubleBar1.x-2) doubleBar1.x = layer->GetObjectProperties()->pos.x + operat->GetObjectProperties()->pos.x + 3;
							}
							else
							{
								if (layer->GetObjectProperties()->pos.x + layer->GetObjectProperties()->size.cx - operat->GetObjectProperties()->pos.x <= doubleBar2.x+1) doubleBar2.x = layer->GetObjectProperties()->pos.x + layer->GetObjectProperties()->size.cx - operat->GetObjectProperties()->pos.x - 2;
							}
						}

						if (operat == this) afterMe = SMOOTH::True;
					}
				}
			}
		}

		objectProperties->size.cx = doubleBar2.x - doubleBar1.x + 4;
		objectProperties->size.cy = 2;

		HBar(dc, doubleBar1, doubleBar2);
	}
	else if (IsBitSet(orientation, OR_VERT))
	{
		if (container->GetObjectType() == OBJ_WINDOW)
		{
			if (IsBitSet(orientation, OR_LEFT))		doubleBar1.x = objectProperties->pos.x;
			else if	(IsBitSet(orientation, OR_RIGHT))	doubleBar1.x = wnd->GetObjectProperties()->size.cx - objectProperties->pos.x;

			doubleBar1.y = wnd->offset.top;
			doubleBar2.x = doubleBar1.x;
			doubleBar2.y = wnd->GetObjectProperties()->size.cy - wnd->offset.bottom - 2;

			for (SMOOTHInt i = SMOOTHObject::objectCount - 1; i >= 0; i--)
			{
				object = mainObjectManager->RequestObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == OBJ_DIVISIONBAR && object->GetContainer() == myContainer)
					{
						operat = (SMOOTHDivisionbar *) object;

						if (afterMe && IsBitSet(operat->orientation, OR_HORZ))
						{
							if (IsBitSet(operat->orientation, OR_TOP))
							{
								if (operat->GetObjectProperties()->pos.y >= doubleBar1.y-2) doubleBar1.y = operat->GetObjectProperties()->pos.y + 3;
							}
							else
							{
								if (operat->GetObjectProperties()->pos.y <= doubleBar2.y+1) doubleBar2.y = wnd->GetObjectProperties()->size.cy - operat->GetObjectProperties()->pos.y - 2;
							}
						}

						if (operat == this) afterMe = SMOOTH::True;
					}
				}
			}
		}
		else if (container->GetObjectType() == OBJ_LAYER)
		{
			if (IsBitSet(orientation, OR_LEFT))		doubleBar1.x = layer->GetObjectProperties()->pos.x + objectProperties->pos.x;
			else if (IsBitSet(orientation, OR_RIGHT))	doubleBar1.x = layer->GetObjectProperties()->pos.x + layer->GetObjectProperties()->size.cx - objectProperties->pos.x;

			doubleBar1.y = layer->GetObjectProperties()->pos.y + 4;
			doubleBar2.x = doubleBar1.x;
			doubleBar2.y = layer->GetObjectProperties()->pos.y + layer->GetObjectProperties()->size.cy - 4;

			for (SMOOTHInt i = SMOOTHObject::objectCount - 1; i >= 0; i--)
			{
				object = mainObjectManager->RequestObject(i);

				if (object != NIL)
				{
					if (object->GetObjectType() == OBJ_DIVISIONBAR && object->GetContainer() == myContainer)
					{
						operat = (SMOOTHDivisionbar *) object;

						if (afterMe && IsBitSet(operat->orientation, OR_HORZ))
						{
							if (IsBitSet(operat->orientation, OR_TOP))
							{
								if (layer->GetObjectProperties()->pos.y + operat->GetObjectProperties()->pos.y >= doubleBar1.y-2) doubleBar1.y = layer->GetObjectProperties()->pos.y + operat->GetObjectProperties()->pos.y + 3;
							}
							else
							{
								if (layer->GetObjectProperties()->pos.y + layer->GetObjectProperties()->size.cy - operat->GetObjectProperties()->pos.y <= doubleBar2.y+1) doubleBar2.y = layer->GetObjectProperties()->pos.y + layer->GetObjectProperties()->size.cy - operat->GetObjectProperties()->pos.y - 2;
							}
						}

						if (operat == this) afterMe = SMOOTH::True;
					}
				}
			}
		}

		objectProperties->size.cy = doubleBar2.y - doubleBar1.y + 4;
		objectProperties->size.cx = 2;

		VBar(dc, doubleBar1, doubleBar2);
	}

	FreeContext(wnd, dc);

	return SMOOTH::Success;
}

#endif
