 /* The smooth Class Library
  * Copyright (C) 1998-2014 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/hyperlink.h>
#include <smooth/gui/widgets/hotspot/hotspot.h>
#include <smooth/gui/window/window.h>
#include <smooth/graphics/color.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/input/pointer.h>
#include <smooth/system/system.h>

const S::Short	 S::GUI::Hyperlink::classID = S::Object::RequestClassID();

S::GUI::Hyperlink::Hyperlink(const String &iText, const Bitmap &bitmap, const String &link, const Point &iPos, const Size &iSize) : Text(iText, iPos, Size())
{
	type		= classID;
	linkURL		= link;
	linkBitmap	= bitmap;

	SetSize(iSize);

	linkBitmap.ReplaceColor(Color(192, 192, 192), Setup::BackgroundColor);

	font.SetStyle(Font::Underline);
	font.SetColor(Color(0, 0, 255));

	if (linkBitmap != NIL)
	{
		if (GetSize() == Size(0, 0)) SetSize(linkBitmap.GetSize());
	}
	else
	{
		ComputeTextSize();

		SetSize(scaledTextSize * 96.0 / Surface().GetSurfaceDPI() + Size(0, 2));
	}

	hotspot	= new Hotspot(Point(0, 0), GetSize());

	hotspot->onMouseOver.Connect(&Hyperlink::OnMouseOver, this);
	hotspot->onMouseOut.Connect(&Hyperlink::OnMouseOut, this);
	hotspot->onLeftButtonClick.Connect(&Hyperlink::OnClickLink, this);

	Add(hotspot);
}

S::GUI::Hyperlink::~Hyperlink()
{
	DeleteObject(hotspot);
}

S::Int S::GUI::Hyperlink::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	switch (message)
	{
		case SP_PAINT:
			if (linkBitmap != NIL)
			{
				Rect	 rect	 = Rect(GetRealPosition(), GetRealSize());
				Surface	*surface = GetDrawSurface();

				surface->BlitFromBitmap(linkBitmap, Rect(Point(0, 0), linkBitmap.GetSize()), rect);
			}

			break;
	}

	return Text::Paint(message);
}

S::Void S::GUI::Hyperlink::OnMouseOver()
{
	Input::Pointer::SetCursor(container->GetContainerWindow(), Input::Pointer::CursorHand);

	if (linkBitmap == NIL)
	{
		Font	 originalFont = font;

		font.SetColor(Color(0, 128, 255));

		Text::Paint(SP_PAINT);

		font.SetColor(originalFont.GetColor());
	}
}

S::Void S::GUI::Hyperlink::OnMouseOut()
{
	Input::Pointer::SetCursor(container->GetContainerWindow(), Input::Pointer::CursorArrow);

	if (linkBitmap == NIL)
	{
		Text::Paint(SP_PAINT);
	}
}

S::Void S::GUI::Hyperlink::OnClickLink()
{
	S::System::System::OpenURL(linkURL);

	onAction.Emit();
}

S::Int S::GUI::Hyperlink::SetText(const String &nText)
{
	if (linkBitmap != NIL) linkBitmap = NIL;

	Text::SetText(nText);

	hotspot->SetSize(GetSize());

	return Success();
}

S::Int S::GUI::Hyperlink::SetFont(const Font &nFont)
{
	Text::SetFont(nFont);

	hotspot->SetSize(GetSize());

	return Success();
}

S::Int S::GUI::Hyperlink::SetBitmap(const Bitmap &newBitmap)
{
	linkBitmap = newBitmap;

	linkBitmap.ReplaceColor(Color(192, 192, 192), Setup::BackgroundColor);

	Paint(SP_PAINT);

	return Success();
}
