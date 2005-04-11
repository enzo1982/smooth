 /* The smooth Class Library
  * Copyright (C) 1998-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/text.h>
#include <smooth/definitions.h>
#include <smooth/loop.h>
#include <smooth/misc/math.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Text::classID = S::Object::RequestClassID();

S::GUI::Text::Text(String iText, Point iPos)
{
	type	= classID;

	possibleContainers.AddEntry(Layer::classID);

	pos	= iPos;
	text	= iText;

	GetTextSize();
}

S::GUI::Text::~Text()
{
}

S::Int S::GUI::Text::Paint(Int message)
{
	if (!registered)	return Failure;
	if (!visible)		return Success;

	Surface	*surface = container->GetDrawSurface();

	EnterProtectedRegion();

	Rect	 textRect;
	Point	 realPos	= GetRealPosition();
	Font	 nFont		= font;

	size.cx = textSize.cx;
	size.cy = textSize.cy + 1;

	switch (message)
	{
		default:
		case SP_PAINT:
			textRect.left	= realPos.x;
			textRect.top	= realPos.y;
			textRect.right	= textRect.left + textSize.cx;
			textRect.bottom	= textRect.top + Math::Round(textSize.cy * 1.2);

			if (!active) nFont.SetColor(Setup::GrayTextColor);

			surface->SetText(text, textRect, nFont);

			break;
	}

	LeaveProtectedRegion();

	return Success;
}
