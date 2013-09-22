 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
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
#include <smooth/gui/widgets/basic/tabwidget.h>
#include <smooth/graphics/surface.h>

const S::Short	 S::GUI::Layer::classID = S::Object::RequestClassID();

S::GUI::Layer::Layer(const String &name) : Widget(Point(0, 0), Size(32768, 32768))
{
	type	    = classID;

	text	    = name;
	orientation = OR_CENTER;

	ComputeTextSize();
}

S::GUI::Layer::~Layer()
{
}

S::Int S::GUI::Layer::Show()
{
	if (visible) return Success();

	visible = True;

	if (!IsRegistered()) return Success();

	if (IsBackgroundColorSet() && IsVisible())
	{
		Surface	*surface	= GetDrawSurface();
		Rect	 frame		= Rect(GetRealPosition(), GetRealSize());

		surface->Box(frame, GetBackgroundColor(), Rect::Filled);
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

	if (IsBackgroundColorSet() && wasVisible)
	{
		Surface	*surface	= GetDrawSurface();
		Rect	 frame		= Rect(GetRealPosition(), GetRealSize());

		surface->Box(frame, container->GetBackgroundColor(), Rect::Filled);
	}

	onHide.Emit();

	return Success();
}

S::Int S::GUI::Layer::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	switch (message)
	{
		case SP_PAINT:
			if (IsBackgroundColorSet())
			{
				Surface	*surface = GetDrawSurface();

				surface->Box(Rect(GetRealPosition(), GetRealSize()), GetBackgroundColor(), Rect::Filled);
			}

			break;
	}

	return Widget::Paint(message);
}

S::Int S::GUI::Layer::SetMetrics(const Point &iPos, const Size &iSize)
{
	if (orientation == OR_CENTER) orientation = OR_FREE;

	return Widget::SetMetrics(iPos, iSize);
}
