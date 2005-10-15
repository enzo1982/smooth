 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/window/window.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/definitions.h>
#include <smooth/misc/i18n.h>
#include <smooth/gui/widgets/basic/tabwidget.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Layer::classID = S::Object::RequestClassID();

S::GUI::Layer::Layer(String name)
{
	type		= classID;
	containerType	= classID;

	text		= name;
	orientation	= OR_CENTER;

	visible = False;

	possibleContainers.AddEntry(Window::classID);
	possibleContainers.AddEntry(classID);
	possibleContainers.AddEntry(TabWidget::classID);
}

S::GUI::Layer::~Layer()
{
}

S::Int S::GUI::Layer::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())		return Failure;
	if (!active || !IsVisible())	return Success;
	if (GetNOfObjects() == 0)	return Success;

	for (Int i = GetNOfObjects() - 1; i >= 0; i--)
	{
		Widget	*object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->Process(message, wParam, lParam) == Break) return Break;
	}

	return Success;
}

S::Int S::GUI::Layer::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success;

	Rect	 updateRect	= wnd->GetUpdateRect();
	Surface	*surface	= container->GetDrawSurface();
	Point	 realPos	= GetRealPosition();

	if (backgroundColor != -1)
	{
		Rect	 frame;

		frame.left	= realPos.x;
		frame.top	= realPos.y;
		frame.right	= realPos.x + size.cx;
		frame.bottom	= realPos.y + size.cy;

		updateRect = frame;

		surface->Box(frame, backgroundColor, FILLED);
	}

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*object = assocObjects.GetNthEntry(i);

		if (object == NIL) continue;

		if (object->IsVisible() && object->IsAffected(updateRect)) object->Paint(SP_PAINT);
	}

	return Success;
}

S::Int S::GUI::Layer::Show()
{
	if (IsVisible()) return Success;

	visible = True;

	if (!IsRegistered()) return Success;

	if (backgroundColor != -1 && IsVisible())
	{
		Surface	*surface	= container->GetDrawSurface();
		Rect	 frame		= Rect(GetRealPosition(), size);

		surface->Box(frame, backgroundColor, FILLED);
	}

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*object = assocObjects.GetNthEntry(i);

		if (object != NIL)
		{
			if (object->IsVisible())
			{
				visible = False;
				object->Hide();

				visible = True;
				object->Show();
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
		Widget	*object = assocObjects.GetNthEntry(i);

		if (object != NIL)
		{
			if (object->IsVisible())
			{
				object->Hide();
				visible = False;

				object->Show();
				visible = True;
			}
		}
	}

	Bool	 wasVisible = IsVisible();

	visible = False;

	if (!IsRegistered()) return Success;

	if (backgroundColor != -1 && wasVisible)
	{
		Surface	*surface	= container->GetDrawSurface();
		Rect	 frame		= Rect(GetRealPosition(), size);

		surface->Box(frame, Setup::BackgroundColor, FILLED);
	}

	onHide.Emit();

	return Success;
}

S::Int S::GUI::Layer::SetMetrics(Point iPos, Size iSize)
{
	if (orientation == OR_CENTER) orientation = OR_FREE;

	return Widget::SetMetrics(iPos, iSize);
}

S::Int S::GUI::Layer::RegisterObject(Widget *object)
{
	if (object == NIL) return Failure;

	if (containerType == &object->possibleContainers)
	{
		if (!object->IsRegistered())
		{
			assocObjects.AddEntry(object, object->GetHandle());

			object->SetContainer(this);
			object->SetRegisteredFlag(True);

			object->onRegister.Emit(this);
			object->Show();

			return Success;
		}
	}

	return Failure;
}

S::Int S::GUI::Layer::UnregisterObject(Widget *object)
{
	if (object == NIL) return Failure;

	if (containerType == &object->possibleContainers)
	{
		if (GetNOfObjects() > 0 && object->IsRegistered())
		{
			if (assocObjects.RemoveEntry(object->GetHandle()) == True)
			{
				object->onUnregister.Emit(this);
				object->Hide();

				object->SetRegisteredFlag(False);
				object->SetContainer(NIL);

				return Success;
			}
		}
	}

	return Failure;
}
