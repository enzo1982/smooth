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

S::GUI::Layer::Layer(String name) : Widget(Point(), Size())
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

	Window	*wnd = container->GetContainerWindow();

	if (wnd == NIL) return Success();

	Rect	 updateRect	= wnd->GetUpdateRect();
	Surface	*surface	= container->GetDrawSurface();

	if (GetBackgroundColor() != -1)
	{
		Rect	 frame = Rect(GetRealPosition(), GetSize());

		updateRect = frame;

		surface->Box(frame, GetBackgroundColor(), FILLED);
	}

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		Widget	*object = GetNthObject(i);

		if (object == NIL) continue;

		if (object->IsVisible() && object->IsAffected(updateRect)) object->Paint(SP_PAINT);
	}

	return Success();
}

S::Int S::GUI::Layer::Process(Int message, Int wParam, Int lParam)
{
	if (!IsRegistered())			return Error();
	if (!IsActive() || !IsVisible())	return Success();

	for (Int i = GetNOfObjects() - 1; i >= 0; i--)
	{
		Widget	*object = GetNthObject(i);

		if (object == NIL) continue;

		if (object->Process(message, wParam, lParam) == Break) return Break;
	}

	return Success();
}

S::Int S::GUI::Layer::SetMetrics(Point iPos, Size iSize)
{
	if (orientation == OR_CENTER) orientation = OR_FREE;

	return Widget::SetMetrics(iPos, iSize);
}
