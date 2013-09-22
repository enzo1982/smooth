 /* The smooth Class Library
  * Copyright (C) 1998-2013 Robert Kausch <robert.kausch@gmx.net>
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

const S::Short	 S::GUI::Button::classID = S::Object::RequestClassID();

S::GUI::Button::Button(const String &iText, const Bitmap &iBitmap, const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	type	= classID;
	text	= iText;
	bitmap	= iBitmap;

	bitmap.ReplaceColor(Color(192, 192, 192), Setup::BackgroundColor);

	if (GetWidth()	== 0) SetWidth(80);
	if (GetHeight() == 0) SetHeight(22);

	ComputeTextSize();

	hotspot	= new HotspotSimpleButton(flags & BF_NOFRAME ? Point() : Point(4, 4), GetSize() - (flags & BF_NOFRAME ? Size() : Size(8, 8)));

	hotspot->onLeftButtonClick.Connect(&onAction);

	Add(hotspot);

	onChangeSize.Connect(&Button::OnChangeSize, this);
}

S::GUI::Button::~Button()
{
	DeleteObject(hotspot);
}

S::Int S::GUI::Button::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	switch (message)
	{
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

				if (!(flags & BF_NOFRAME))
				{
					surface->Frame(frame, FRAME_DOWN);
					surface->Frame(Rect(GetRealPosition() + Point(1, 1), GetRealSize() - Size(2, 2)), FRAME_UP);
				}

				if (text != NIL)
				{
					Rect	 textRect;

					textRect.left	= frame.left + ((frame.GetWidth() - scaledTextSize.cx) / 2);

					if (bitmap != NIL) textRect.left = frame.left + ((frame.GetWidth() - scaledTextSize.cx - bitmap.GetSize().cx - 7) / 2) + bitmap.GetSize().cx + 7;

					textRect.top	= frame.top + ((frame.GetHeight() - scaledTextSize.cy) / 2) - 1;
					textRect.right	= textRect.left + scaledTextSize.cx + 1;
					textRect.bottom	= textRect.top + Math::Round(scaledTextSize.cy * 1.2);

					Font	 nFont = font;

					if (!IsActive()) nFont.SetColor(Setup::GrayTextColor);

					surface->Box(textRect, GetBackgroundColor(), Rect::Filled);
					surface->SetText(text, textRect, nFont);
				}

				if (bitmap != NIL)
				{
					Rect	 bmpRect;
					Size	 bmpSize = bitmap.GetSize() * surface->GetSurfaceDPI() / 96.0;

					bmpRect.left	= frame.left + (frame.GetWidth() - bmpSize.cx) / 2;

					if (text != NIL) bmpRect.left = frame.left + (frame.GetWidth() - bmpSize.cx - scaledTextSize.cx - 7) / 2;

					bmpRect.top	= frame.top + (frame.GetHeight() - bmpSize.cy) / 2;
					bmpRect.right	= bmpRect.left + bmpSize.cx;
					bmpRect.bottom	= bmpRect.top + bmpSize.cy;

					surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
				}
			}

			break;
	}

	return Success();
}

S::Void S::GUI::Button::OnChangeSize(const Size &nSize)
{
	hotspot->SetSize(nSize - (flags & BF_NOFRAME ? Size() : Size(8, 8)));
}

S::Int S::GUI::Button::SetBitmap(const Bitmap &nBitmap)
{
	Bool	 prevVisible = IsVisible();

	if (IsRegistered() && prevVisible) Hide();

	bitmap = nBitmap;
	bitmap.ReplaceColor(Color(192, 192, 192), Setup::BackgroundColor);

	if (IsRegistered() && prevVisible) Show();

	return Success();
}
