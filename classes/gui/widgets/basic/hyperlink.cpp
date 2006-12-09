 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
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

const S::Int	 S::GUI::Hyperlink::classID = S::Object::RequestClassID();

S::GUI::Hyperlink::Hyperlink(const String &iText, const Bitmap &bitmap, const String &link, const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	type		= classID;
	text		= iText;
	linkURL		= link;
	linkBitmap	= bitmap;

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

		SetSize(textSize);
	}

	hotspot	= new Hotspot(Point(0, 0), GetSize());

	hotspot->onMouseOver.Connect(&Hyperlink::OnMouseOver, this);
	hotspot->onMouseOut.Connect(&Hyperlink::OnMouseOut, this);
	hotspot->onLeftButtonClick.Connect(&Hyperlink::OnClickLink, this);

	RegisterObject(hotspot);
}

S::GUI::Hyperlink::~Hyperlink()
{
	DeleteObject(hotspot);
}

S::Int S::GUI::Hyperlink::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				Surface	*surface = container->GetDrawSurface();

				if (linkBitmap == NIL)	surface->SetText(text, Rect(GetRealPosition(), textSize + Size(0, 1)), font);
				else			surface->BlitFromBitmap(linkBitmap, Rect(Point(0, 0), linkBitmap.GetSize()), Rect(GetRealPosition(), GetSize()));
			}

			break;
	}

	return Success();
}

S::Void S::GUI::Hyperlink::OnMouseOver()
{
	LiSASetMouseCursor((HWND) container->GetContainerWindow()->GetSystemWindow(), LiSA_MOUSE_HAND);

	if (linkBitmap == NIL)
	{
		Surface	*surface  = container->GetDrawSurface();
		Rect	 textRect = Rect(GetRealPosition(), textSize + Size(0, 1));
		Font	 nFont	  = font;

		nFont.SetColor(Color(0, 128, 255));

		surface->SetText(text, textRect, nFont);
	}
}

S::Void S::GUI::Hyperlink::OnMouseOut()
{
	LiSASetMouseCursor((HWND) container->GetContainerWindow()->GetSystemWindow(), LiSA_MOUSE_ARROW);

	if (linkBitmap == NIL)
	{
		Surface	*surface  = container->GetDrawSurface();
		Rect	 textRect = Rect(GetRealPosition(), textSize + Size(0, 1));

		surface->SetText(text, textRect, font);
	}
}

S::Void S::GUI::Hyperlink::OnClickLink()
{
	if (Setup::enableUnicode)	LiSAOpenURLW(linkURL);
	else				LiSAOpenURLA(linkURL);

	onAction.Emit();
}

S::Int S::GUI::Hyperlink::SetText(const String &newText)
{
	if (linkBitmap != NIL) linkBitmap = NIL;

	Widget::SetText(newText);

	SetSize(textSize);

	hotspot->SetSize(GetSize());

	return Success();
}

S::Int S::GUI::Hyperlink::SetBitmap(const Bitmap &newBmp)
{
	return Error();
}

const S::GUI::Bitmap &S::GUI::Hyperlink::GetBitmap()
{
	return linkBitmap;
}

S::Int S::GUI::Hyperlink::SetURL(const String &newUrl)
{
	linkURL = newUrl;

	return Success();
}

const S::String &S::GUI::Hyperlink::GetURL()
{
	return linkURL;
}
