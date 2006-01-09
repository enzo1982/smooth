 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/i18n/i18n.h>
#include <smooth/gui/widgets/basic/tabwidget.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Layer::classID = S::Object::RequestClassID();

S::GUI::Layer::Layer(const String &name) : Widget(Point(), Size())
{
	type		= classID;

	text		= name;
	orientation	= OR_CENTER;
}

S::GUI::Layer::~Layer()
{
}

S::Int S::GUI::Layer::Show()
{
	if (visible) return Success();

	visible = True;

	if (!IsRegistered()) return Success();

	if (GetBackgroundColor() != -1 && IsVisible())
	{
		Surface	*surface	= container->GetDrawSurface();
		Rect	 frame		= Rect(GetRealPosition(), GetSize());

		surface->Box(frame, GetBackgroundColor(), FILLED);
	}

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*object = GetNthObject(i);

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

	return Success();
}

S::Int S::GUI::Layer::Hide()
{
	if (!visible) return Success();

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*object = GetNthObject(i);

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

	if (!IsRegistered()) return Success();

	if (GetBackgroundColor() != -1 && wasVisible)
	{
		Surface	*surface	= container->GetDrawSurface();
		Rect	 frame		= Rect(GetRealPosition(), GetSize());

		surface->Box(frame, Setup::BackgroundColor, FILLED);
	}

	onHide.Emit();

	return Success();
}

S::Int S::GUI::Layer::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	Window	*window = container->GetContainerWindow();

	if (window == NIL) return Success();

	Rect	 updateRect	= window->GetUpdateRect();
	Surface	*surface	= container->GetDrawSurface();

	if (GetBackgroundColor() != -1)
	{
		Rect	 frame = Rect(GetRealPosition(), GetSize());

		surface->Box(Rect::OverlapRect(frame, updateRect), GetBackgroundColor(), FILLED);
	}

	return Widget::Paint(message);
}

S::Int S::GUI::Layer::SetMetrics(const Point &iPos, const Size &iSize)
{
	if (orientation == OR_CENTER) orientation = OR_FREE;

	return Widget::SetMetrics(iPos, iSize);
}
