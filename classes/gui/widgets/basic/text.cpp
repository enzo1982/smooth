 /* The smooth Class Library
  * Copyright (C) 1998-2010 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/text.h>
#include <smooth/misc/math.h>
#include <smooth/graphics/surface.h>

const S::Short	 S::GUI::Text::classID = S::Object::RequestClassID();

S::GUI::Text::Text(const String &iText, const Point &iPos) : Widget(iPos, Size())
{
	type	= classID;
	text	= iText;

	ComputeTextSize();
}

S::GUI::Text::~Text()
{
}

S::Int S::GUI::Text::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	SetSize(textSize + Size(0, 1));

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Font	 nFont		= font;

				if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

				Rect	 rect		= Rect(GetRealPosition(), GetSize());
				Surface	*surface	= GetDrawSurface();

				surface->Box(rect, GetBackgroundColor(), Rect::Filled);
				surface->SetText(text, Rect::OverlapRect(Rect(GetRealPosition(), Size(textSize.cx, Math::Round(textSize.cy * 1.2))), GetVisibleArea()), nFont);
			}

			break;
	}

	return Success();
}
