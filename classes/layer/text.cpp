 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/text.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Text::classID = S::Object::RequestClassID();

S::GUI::Text::Text(String text, Point pos)
{
	type = classID;

	possibleContainers.AddEntry(Layer::classID);

	objectProperties->pos.x = Math::Round(pos.x * Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * Setup::FontSize);

	objectProperties->text = text;

	GetTextSize();
}

S::GUI::Text::~Text()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::GUI::Text::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 textRect;
	Point	 realPos = GetRealPosition();
	Font	 font = objectProperties->font;

	objectProperties->size.cx = objectProperties->textSize.cx;
	objectProperties->size.cy = objectProperties->textSize.cy + 1;

	switch (message)
	{
		default:
		case SP_PAINT:
			textRect.left	= realPos.x;
			textRect.top	= realPos.y;
			textRect.right	= textRect.left + objectProperties->textSize.cx;
			textRect.bottom	= textRect.top + Math::Round(objectProperties->textSize.cy * 1.2);

			if (!active) font.SetColor(Setup::GrayTextColor);

			surface->SetText(objectProperties->text, textRect, font);

			break;
	}

	LeaveProtectedRegion();

	return Success;
}
