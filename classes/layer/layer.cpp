 /* The SMOOTH Windowing Toolkit
  * Copyright (C) 1998-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef __OBJSMOOTH_LAYER_
#define __OBJSMOOTH_LAYER_

#include <smooth/window.h>
#include <smooth/layer.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/tabregister.h>
#include <smooth/objectproperties.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

SMOOTHInt	 OBJ_LAYER = SMOOTH::RequestObjectID();

SMOOTHLayer::SMOOTHLayer(SMOOTHString name)
{
	self = this;

	type = OBJ_LAYER;
	containerType = OBJ_LAYER;

	objectProperties->text		= name;
	objectProperties->orientation	= OR_CENTER;

	visible = SMOOTH::False;

	layerColor = -1;

	possibleContainers.AddEntry(OBJ_WINDOW);
	possibleContainers.AddEntry(OBJ_TABREGISTER);
}

SMOOTHLayer::~SMOOTHLayer()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

SMOOTHInt SMOOTHLayer::Process(SMOOTHInt message, SMOOTHInt wParam, SMOOTHInt lParam)
{
	if (!registered)		return SMOOTH::Error;
	if (!active || !visible)	return SMOOTH::Success;
	if (nOfObjects == 0)		return SMOOTH::Success;

	SMOOTHObject	*object;

	for (SMOOTHInt i = nOfObjects - 1; i >= 0; i--)
	{
		object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->Process(message, wParam, lParam) == SMOOTH::Break) return SMOOTH::Break;
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHLayer::Paint(SMOOTHInt message)
{
	if (!registered)	return SMOOTH::Error;
	if (!visible)		return SMOOTH::Success;

	SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

	if (wnd == NIL) return SMOOTH::Success;
	if (wnd->hwnd == NIL) return SMOOTH::Success;

	SMOOTHRect	 updateRect = wnd->GetUpdateRect();
	SMOOTHObject	*object;

	if (layerColor != -1)
	{
		SMOOTHRect	 frame;
		HDC		 dc = GetContext(wnd);

		frame.left	= objectProperties->pos.x;
		frame.top	= objectProperties->pos.y;
		frame.right	= objectProperties->pos.x + objectProperties->size.cx;
		frame.bottom	= objectProperties->pos.y + objectProperties->size.cy;

		updateRect = frame;

		Box(dc, frame, layerColor, FILLED);

		FreeContext(wnd, dc);
	}

	for (SMOOTHInt i = 0; i < nOfObjects; i++)
	{
		object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->IsVisible() && Affected(object, updateRect)) object->Paint(SP_PAINT);
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHLayer::Show()
{
	if (visible) return SMOOTH::Success;

	visible = SMOOTH::True;

	if (!registered) return SMOOTH::Success;

	SMOOTHObject	*object;

	if (layerColor != -1)
	{
		SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

		if (!(wnd == NIL || wnd->hwnd == NIL))
		{
			SMOOTHRect	 frame;
			HDC		 dc = GetContext(wnd);

			frame.left	= objectProperties->pos.x;
			frame.top	= objectProperties->pos.y;
			frame.right	= objectProperties->pos.x + objectProperties->size.cx;
			frame.bottom	= objectProperties->pos.y + objectProperties->size.cy;

			Box(dc, frame, layerColor, FILLED);

			FreeContext(wnd, dc);
		}
	}

	for (SMOOTHInt i = 0; i < nOfObjects; i++)
	{
		object = assocObjects.GetNthEntry(i);

		if (object != NIL) object->Show();
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHLayer::Hide()
{
	if (!visible) return SMOOTH::Success;

	visible = SMOOTH::False;

	if (!registered) return SMOOTH::Success;

	SMOOTHObject	*object;

	for (SMOOTHInt i = 0; i < nOfObjects; i++)
	{
		object = assocObjects.GetNthEntry(i);

		if (object != NIL) object->Hide();
	}

	if (layerColor != -1)
	{
		SMOOTHWindow	*wnd = (SMOOTHWindow *) myContainer->GetContainerObject();

		if (!(wnd == NIL || wnd->hwnd == NIL))
		{
			SMOOTHRect	 frame;
			HDC		 dc = GetContext(wnd);

			frame.left	= objectProperties->pos.x;
			frame.top	= objectProperties->pos.y;
			frame.right	= objectProperties->pos.x + objectProperties->size.cx;
			frame.bottom	= objectProperties->pos.y + objectProperties->size.cy;

			Box(dc, frame, SMOOTH::Setup::BackgroundColor, FILLED);

			FreeContext(wnd, dc);
		}
	}

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHLayer::SetColor(SMOOTHInt newColor)
{
	layerColor = newColor;

	Paint(SP_PAINT);

	return SMOOTH::Success;
}

SMOOTHInt SMOOTHLayer::SetMetrics(SMOOTHPoint pos, SMOOTHSize size)
{
	objectProperties->orientation = OR_FREE;

	return SMOOTHObject::SetMetrics(pos, size);
}

SMOOTHSurface *SMOOTHLayer::GetDrawSurface()
{
	if (!registered) return nullSurface;

	return myContainer->GetDrawSurface();
}

SMOOTHInt SMOOTHLayer::RegisterObject(SMOOTHObject *object)
{
	if (object == NIL) return SMOOTH::Error;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->handle);
			nOfObjects++;

			object->SetContainer(this);
			object->SetRegisteredFlag();

			if (visible) object->Show();

			return SMOOTH::Success;
		}
	}

	return SMOOTH::Error;
}

SMOOTHInt SMOOTHLayer::UnregisterObject(SMOOTHObject *object)
{
	if (object == NIL) return SMOOTH::Error;

	if (containerType == &object->possibleContainers)
	{
		if (nOfObjects > 0 && object->IsRegistered())
		{
			if (assocObjects.DeleteEntry(object->handle) == SMOOTH::True)
			{
				nOfObjects--;

				if (visible) object->Hide();

				object->UnsetRegisteredFlag();
				object->SetContainer(NIL);

				return SMOOTH::Success;
			}
		}
	}

	return SMOOTH::Error;
}

#endif
