 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
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

		SetSize(scaledTextSize * 96.0 / Surface().GetSurfaceDPI() + Size(0, 1));
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
			{
				Rect	 rect	 = Rect(GetRealPosition(), GetRealSize());
				Surface	*surface = GetDrawSurface();

				if (linkBitmap == NIL)
				{
					surface->Box(rect, GetBackgroundColor(), Rect::Filled);
					surface->SetText(text, Rect::OverlapRect(Rect(GetRealPosition(), Size(scaledTextSize.cx, Math::Round(scaledTextSize.cy * 1.2))), GetVisibleArea()), font);
				}
				else
				{
					surface->BlitFromBitmap(linkBitmap, Rect(Point(0, 0), linkBitmap.GetSize()), rect);
				}
			}

			break;
	}

	return Success();
}

S::Void S::GUI::Hyperlink::OnMouseOver()
{
	Input::Pointer::SetCursor(container->GetContainerWindow(), Input::Pointer::CursorHand);

	if (linkBitmap == NIL)
	{
		Surface	*surface  = GetDrawSurface();
		Rect	 textRect = Rect(GetRealPosition(), scaledTextSize + Size(0, 1));
		Font	 nFont	  = font;

		nFont.SetColor(Color(0, 128, 255));

		surface->Box(textRect, GetBackgroundColor(), Rect::Filled);
		surface->SetText(text, textRect, nFont);
	}
}

S::Void S::GUI::Hyperlink::OnMouseOut()
{
	Input::Pointer::SetCursor(container->GetContainerWindow(), Input::Pointer::CursorArrow);

	if (linkBitmap == NIL)
	{
		Surface	*surface  = GetDrawSurface();
		Rect	 textRect = Rect(GetRealPosition(), scaledTextSize + Size(0, 1));

		surface->Box(textRect, GetBackgroundColor(), Rect::Filled);
		surface->SetText(text, textRect, font);
	}
}

S::Void S::GUI::Hyperlink::OnClickLink()
{
	S::System::System::OpenURL(linkURL);

	onAction.Emit();
}

S::Int S::GUI::Hyperlink::SetText(const String &newText)
{
	if (linkBitmap != NIL) linkBitmap = NIL;

	Widget::SetText(newText);

	SetSize(scaledTextSize * 96.0 / Surface().GetSurfaceDPI() + Size(0, 1));

	hotspot->SetSize(GetSize());

	Paint(SP_PAINT);

	return Success();
}

S::Int S::GUI::Hyperlink::SetBitmap(const Bitmap &newBitmap)
{
	linkBitmap = newBitmap;

	linkBitmap.ReplaceColor(Color(192, 192, 192), Setup::BackgroundColor);

	Paint(SP_PAINT);

	return Success();
}
