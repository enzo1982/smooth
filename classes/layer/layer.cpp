 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/window.h>
#include <smooth/layer.h>
#include <smooth/definitions.h>
#include <smooth/toolkit.h>
#include <smooth/i18n.h>
#include <smooth/stk.h>
#include <smooth/tabwidget.h>
#include <smooth/objectproperties.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_LAYER = S::Object::RequestObjectID();

S::GUI::Layer::Layer(String name)
{
	self = this;

	type = OBJ_LAYER;
	containerType = OBJ_LAYER;

	objectProperties->text		= name;
	objectProperties->orientation	= OR_CENTER;

	visible = False;

	layerColor = -1;

	possibleContainers.AddEntry(OBJ_WINDOW);
	possibleContainers.AddEntry(OBJ_TABREGISTER);
}

S::GUI::Layer::~Layer()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Layer::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;
	if (nOfObjects == 0)		return Success;

	Object	*object;

	for (Int i = nOfObjects - 1; i >= 0; i--)
	{
		object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->Process(message, wParam, lParam) == Break) return Break;
	}

	return Success;
}

S::Int S::GUI::Layer::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Window	*wnd = (Window *) myContainer->GetContainerObject();

	if (wnd == NIL) return Success;
	if (wnd->hwnd == NIL) return Success;

	Rect	 updateRect = wnd->GetUpdateRect();
	Object	*object;

	if (layerColor != -1)
	{
		Rect	 frame;
		HDC	 dc = GetContext(wnd);

		frame.left	= objectProperties->pos.x;
		frame.top	= objectProperties->pos.y;
		frame.right	= objectProperties->pos.x + objectProperties->size.cx;
		frame.bottom	= objectProperties->pos.y + objectProperties->size.cy;

		updateRect = frame;

		Box(dc, frame, layerColor, FILLED);

		FreeContext(wnd, dc);
	}

	for (Int i = 0; i < nOfObjects; i++)
	{
		object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->IsVisible() && Affected(object, updateRect)) object->Paint(SP_PAINT);
	}

	return Success;
}

S::Int S::GUI::Layer::Show()
{
	if (visible) return Success;

	visible = True;

	if (!registered) return Success;

	Object	*object;

	if (layerColor != -1)
	{
		Window	*wnd = (Window *) myContainer->GetContainerObject();

		if (!(wnd == NIL || wnd->hwnd == NIL))
		{
			Rect	 frame;
			HDC	 dc = GetContext(wnd);

			frame.left	= objectProperties->pos.x;
			frame.top	= objectProperties->pos.y;
			frame.right	= objectProperties->pos.x + objectProperties->size.cx;
			frame.bottom	= objectProperties->pos.y + objectProperties->size.cy;

			Box(dc, frame, layerColor, FILLED);

			FreeContext(wnd, dc);
		}
	}

	for (Int i = 0; i < nOfObjects; i++)
	{
		object = assocObjects.GetNthEntry(i);

		if (object != NIL) object->Show();
	}

	return Success;
}

S::Int S::GUI::Layer::Hide()
{
	if (!visible) return Success;

	visible = False;

	if (!registered) return Success;

	Object	*object;

	for (Int i = 0; i < nOfObjects; i++)
	{
		object = assocObjects.GetNthEntry(i);

		if (object != NIL) object->Hide();
	}

	if (layerColor != -1)
	{
		Window	*wnd = (Window *) myContainer->GetContainerObject();

		if (!(wnd == NIL || wnd->hwnd == NIL))
		{
			Rect	 frame;
			HDC	 dc = GetContext(wnd);

			frame.left	= objectProperties->pos.x;
			frame.top	= objectProperties->pos.y;
			frame.right	= objectProperties->pos.x + objectProperties->size.cx;
			frame.bottom	= objectProperties->pos.y + objectProperties->size.cy;

			Box(dc, frame, Setup::BackgroundColor, FILLED);

			FreeContext(wnd, dc);
		}
	}

	return Success;
}

S::Int S::GUI::Layer::SetColor(Int newColor)
{
	layerColor = newColor;

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::Layer::SetMetrics(Point pos, Size size)
{
	objectProperties->orientation = OR_FREE;

	return Object::SetMetrics(pos, size);
}

S::GUI::Surface *S::GUI::Layer::GetDrawSurface()
{
	if (!registered) return nullSurface;

	return myContainer->GetDrawSurface();
}

S::Int S::GUI::Layer::RegisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->handle);
			nOfObjects++;

			object->SetContainer(this);
			object->SetRegisteredFlag();

			if (visible) object->Show();

			return Success;
		}
	}

	return Error;
}

S::Int S::GUI::Layer::UnregisterObject(Object *object)
{
	if (object == NIL) return Error;

	if (containerType == &object->possibleContainers)
	{
		if (nOfObjects > 0 && object->IsRegistered())
		{
			if (assocObjects.DeleteEntry(object->handle) == True)
			{
				nOfObjects--;

				if (visible) object->Hide();

				object->UnsetRegisteredFlag();
				object->SetContainer(NIL);

				return Success;
			}
		}
	}

	return Error;
}
