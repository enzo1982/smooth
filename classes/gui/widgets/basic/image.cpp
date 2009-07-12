 /* The smooth Class Library
  * Copyright (C) 1998-2009 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/image.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Image::classID = S::Object::RequestClassID();

S::GUI::Image::Image(const Bitmap &iBitmap, const Point &iPos, const Size &iSize) : Widget(iPos, iSize)
{
	type	= classID;

	SetBitmap(iBitmap);
}

S::GUI::Image::~Image()
{
}

S::Int S::GUI::Image::Paint(Int message)
{
	if (!IsRegistered())	return Error();
	if (!IsVisible())	return Success();

	switch (message)
	{
		case SP_SHOW:
		case SP_PAINT:
			{
				/* Scale image if necessary.
				 */
				Size	 bmpSize = bitmap.GetSize();
				Size	 newSize = bmpSize;

				if	(float(bmpSize.cx) / GetWidth()  >= float(bmpSize.cy) / GetHeight() && bmpSize.cx > GetWidth())	 newSize = newSize * (float(GetWidth()) / bmpSize.cx);
				else if (float(bmpSize.cy) / GetHeight() >= float(bmpSize.cx) / GetWidth()  && bmpSize.cy > GetHeight()) newSize = newSize * (float(GetHeight()) / bmpSize.cy);

				/* Draw image centered.
				 */
				Surface	*surface = GetDrawSurface();
				Rect	 bmpRect = Rect(GetRealPosition() + Point(GetWidth() - newSize.cx, GetHeight() - newSize.cy) / 2, newSize);

				surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), bitmap.GetSize()), bmpRect);
			}

			break;
	}

	return Success();
}

S::Int S::GUI::Image::SetBitmap(const Bitmap &newBmp)
{
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	bitmap = newBmp;

	if (GetSize() == Size(0, 0)) SetSize(bitmap.GetSize());

	if (prevVisible) Show();

	return Success();
}

const S::GUI::Bitmap &S::GUI::Image::GetBitmap()
{
	return bitmap;
}
