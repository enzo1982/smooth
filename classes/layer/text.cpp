 /* The smooth Class Library
  * Copyright (C) 1998-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of the "Artistic License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/text.h>
#include <smooth/toolkit.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/metrics.h>
#include <smooth/math.h>
#include <smooth/stk.h>
#include <smooth/objectproperties.h>
#include <smooth/layer.h>
#include <smooth/surface.h>

#ifdef __WIN32__
__declspec (dllexport)
#endif

S::Int	 S::OBJ_TEXT = S::Object::RequestObjectID();

S::Text::Text(String text, Point pos)
{
	type = OBJ_TEXT;

	possibleContainers.AddEntry(OBJ_LAYER);

	objectProperties->pos.x = Math::Round(pos.x * SMOOTH::Setup::FontSize);
	objectProperties->pos.y = Math::Round(pos.y * SMOOTH::Setup::FontSize);

	objectProperties->text = text;

	GetTextSize();
}

S::Text::~Text()
{
	if (registered && myContainer != NIL) myContainer->UnregisterObject(this);
}

S::Int S::Text::Paint(Int message)
{
	if (!registered)	return Error;
	if (!visible)		return Success;

	Surface	*surface = myContainer->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 textRect;
	Point	 realPos = GetRealPosition();

	objectProperties->size = objectProperties->textSize;

	switch (message)
	{
		default:
		case SP_PAINT:
			textRect.left	= realPos.x;
			textRect.top	= realPos.y;
			textRect.right	= textRect.left + objectProperties->textSize.cx;
			textRect.bottom	= textRect.top + Math::Round(objectProperties->textSize.cy * 1.2);

			if (active)	surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, objectProperties->fontColor, objectProperties->fontWeight);
			else		surface->SetText(objectProperties->text, textRect, objectProperties->font, objectProperties->fontSize, SMOOTH::Setup::GrayTextColor, objectProperties->fontWeight);

			break;
	}

	LeaveProtectedRegion();

	return Success;
}
