 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/window/window.h>
#include <smooth/layer.h>
#include <smooth/definitions.h>
#include <smooth/misc/i18n.h>
#include <smooth/tabwidget.h>
#include <smooth/objectproperties.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Layer::classID = S::Object::RequestClassID();

S::GUI::Layer::Layer(String name)
{
	self = this;

	type		= classID;
	containerType	= classID;

	objectProperties->text		= name;
	objectProperties->orientation	= OR_CENTER;

	visible = False;

	layerColor = (UnsignedLong) -1;

	possibleContainers.AddEntry(Window::classID);
	possibleContainers.AddEntry(classID);
	possibleContainers.AddEntry(TabWidget::classID);
}

S::GUI::Layer::~Layer()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Layer::Process(Int message, Int wParam, Int lParam)
{
	if (!registered)		return Error;
	if (!active || !visible)	return Success;
	if (GetNOfObjects() == 0)	return Success;

	for (Int i = GetNOfObjects() - 1; i >= 0; i--)
	{
		Object	*object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == Widget::classID)
		{
			if (((Widget *) object)->Process(message, wParam, lParam) == Break) return Break;
		}
	}

	return Success;
}

S::Int S::GUI::Layer::Paint(Int message)
{
	if (!registered)	return Error;
	if (!IsVisible())	return Success;

	Window	*wnd = myContainer->GetContainerWindow();

	if (wnd == NIL) return Success;

	Rect	 updateRect = wnd->GetUpdateRect();
	Surface	*surface = myContainer->GetDrawSurface();
	Point	 realPos = GetRealPosition();

	if (layerColor != (UnsignedLong) -1)
	{
		Rect	 frame;

		frame.left	= realPos.x;
		frame.top	= realPos.y;
		frame.right	= realPos.x + objectProperties->size.cx;
		frame.bottom	= realPos.y + objectProperties->size.cy;

		updateRect = frame;

		surface->Box(frame, layerColor, FILLED);
	}

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Object	*object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->GetObjectType() == Widget::classID)
		{
			if (((Widget *) object)->IsVisible() && ((Widget *) object)->IsAffected(updateRect)) ((Widget *) object)->Paint(SP_PAINT);
		}
	}

	return Success;
}

S::Int S::GUI::Layer::Show()
{
	if (visible) return Success;

	visible = True;

	if (!registered) return Success;

	Point	 realPos = GetRealPosition();

	if (layerColor != (UnsignedLong) -1 && IsVisible())
	{
		Surface	*surface = myContainer->GetDrawSurface();
		Rect	 frame;

		frame.left	= realPos.x;
		frame.top	= realPos.y;
		frame.right	= realPos.x + objectProperties->size.cx;
		frame.bottom	= realPos.y + objectProperties->size.cy;

		surface->Box(frame, layerColor, FILLED);
	}

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Object	*object = assocObjects.GetNthEntry(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == Widget::classID)
			{
				if (((Widget *) object)->IsVisible())
				{
					visible = False;
					((Widget *) object)->Hide();
					visible = True;
					((Widget *) object)->Show();
				}
			}
		}
	}

	onShow.Emit();

	return Success;
}

S::Int S::GUI::Layer::Hide()
{
	if (!visible) return Success;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Object	*object = assocObjects.GetNthEntry(i);

		if (object != NIL)
		{
			if (object->GetObjectType() == Widget::classID)
			{
				if (((Widget *) object)->IsVisible())
				{
					((Widget *) object)->Hide();
					visible = False;
					((Widget *) object)->Show();
					visible = True;
				}
			}
		}
	}

	Bool	 wasVisible = IsVisible();

	visible = False;

	if (!registered) return Success;

	if (layerColor != (UnsignedLong) -1 && wasVisible)
	{
		Point	 realPos = GetRealPosition();
		Surface	*surface = myContainer->GetDrawSurface();
		Rect	 frame;

		frame.left	= realPos.x;
		frame.top	= realPos.y;
		frame.right	= realPos.x + objectProperties->size.cx;
		frame.bottom	= realPos.y + objectProperties->size.cy;

		surface->Box(frame, Setup::BackgroundColor, FILLED);
	}

	onHide.Emit();

	return Success;
}

S::UnsignedLong S::GUI::Layer::GetColor()
{
	if ((Long) layerColor == -1)	return Setup::BackgroundColor;
	else				return layerColor;

	return Success;
}

S::Int S::GUI::Layer::SetColor(UnsignedLong newColor)
{
	layerColor = newColor;

	Paint(SP_PAINT);

	return Success;
}

S::Int S::GUI::Layer::SetMetrics(Point pos, Size size)
{
	if (objectProperties->orientation == OR_CENTER) objectProperties->orientation = OR_FREE;

	return Widget::SetMetrics(pos, size);
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

			object->SetContainer(this);
			object->SetRegisteredFlag();

			if (object->GetObjectType() == Widget::classID)
			{
				((Widget *) object)->onRegister.Emit(this);
				((Widget *) object)->Show();
			}

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
		if (GetNOfObjects() > 0 && object->IsRegistered())
		{
			if (assocObjects.RemoveEntry(object->handle) == True)
			{
				if (object->GetObjectType() == Widget::classID)
				{
					((Widget *) object)->onUnregister.Emit(this);
					((Widget *) object)->Hide();
				}

				object->UnsetRegisteredFlag();
				object->SetContainer(NIL);

				return Success;
			}
		}
	}

	return Error;
}
