 /* The smooth Class Library
  * Copyright (C) 1998-2018 Robert Kausch <robert.kausch@gmx.net>
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
