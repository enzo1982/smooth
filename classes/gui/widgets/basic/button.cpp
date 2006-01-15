 /* The smooth Class Library
  * Copyright (C) 1998-2006 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/button.h>
#include <smooth/gui/widgets/hotspot/simplebutton.h>
#include <smooth/graphics/surface.h>
#include <smooth/graphics/bitmap.h>
#include <smooth/misc/math.h>

const S::Int	 S::GUI::Button::classID = S::Object::RequestClassID();

S::GUI::Button::Button(const String &iText, const Bitmap &iBitmap, const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	type	= classID;
	text	= iText;
	bitmap	= iBitmap;

	bitmap.ReplaceColor(Color(192, 192, 192), Setup::BackgroundColor);

	if (GetWidth() == 0) SetWidth(80);
	if (GetHeight() == 0) SetHeight(22);

	GetTextSize();

	hotspot	= new HotspotSimpleButton(flags & BF_NOFRAME ? Point() : Point(4, 4), GetSize() - (flags & BF_NOFRAME ? Size() : Size(8, 8)));

	hotspot->onLeftButtonClick.Connect(&onAction);

	RegisterObject(hotspot);

	onChangeSize.Connect(&Button::OnChangeSize, this);
}

S::GUI::Button::~Button()
{
	DeleteObject(hotspot);
}

S::Int S::GUI::Button::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	EnterProtectedRegion();

	Surface	*surface = container->GetDrawSurface();
	Rect	 frame	 = Rect(GetRealPosition(), GetSize());

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			if (!(flags & BF_NOFRAME))
			{
				surface->Frame(frame, FRAME_DOWN);
				surface->Frame(Rect(GetRealPosition() + Point(1, 1), GetSize() - Size(2, 2)), FRAME_UP);
			}

			if (GetBackgroundColor() >= 0) surface->Box(frame, GetBackgroundColor(), FILLED);

			if (text != NIL)
			{
				Rect	 textRect;

				textRect.left	= frame.left + ((GetWidth() - textSize.cx) / 2);

				if (bitmap != NIL) textRect.left = frame.left + ((GetWidth() - textSize.cx - bitmap.GetSize().cx - 7) / 2) + bitmap.GetSize().cx + 7;

				textRect.top	= frame.top + ((GetHeight() - textSize.cy) / 2) - 1;
				textRect.right	= textRect.left + textSize.cx + 1;
				textRect.bottom	= textRect.top + Math::Round(textSize.cy * 1.2);

				Font	 nFont = font;

				if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

				surface->SetText(text, textRect, nFont);
			}

			if (bitmap != NIL)
			{
				Rect	 bmpRect;

				bmpRect.left	= frame.left + (frame.right - frame.left - bitmap.GetSize().cx) / 2;

				if (text != NIL) bmpRect.left = frame.left + (frame.right - frame.left - bitmap.GetSize().cx - textSize.cx - 7) / 2;

				bmpRect.top	= frame.top + (frame.bottom - frame.top - bitmap.GetSize().cy) / 2;
				bmpRect.right	= bmpRect.left + bitmap.GetSize().cx;
				bmpRect.bottom	= bmpRect.top + bitmap.GetSize().cy;

				surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
			}

			break;
	}

	LeaveProtectedRegion();

	return Success();
}

S::Void S::GUI::Button::OnChangeSize(const Size &nSize)
{
	hotspot->SetSize(nSize - (flags & BF_NOFRAME ? Size() : Size(8, 8)));
}
