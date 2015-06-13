 /* The smooth Class Library
  * Copyright (C) 1998-2015 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/image.h>
#include <smooth/graphics/surface.h>

const S::Short	 S::GUI::Image::classID = S::Object::RequestClassID();

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
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	switch (message)
	{
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();

				/* Scale image if necessary.
				 */
				Size	 size	  = GetSize();
				Size	 realSize = GetRealSize();
				Size	 bmpSize  = bitmap.GetSize();
				Size	 newSize  = bmpSize;

				if	(float(bmpSize.cx) / size.cx >= float(bmpSize.cy) / size.cy && bmpSize.cx > size.cx) newSize = newSize * (float(size.cx) / bmpSize.cx);
				else if (float(bmpSize.cy) / size.cy >= float(bmpSize.cx) / size.cx && bmpSize.cy > size.cy) newSize = newSize * (float(size.cy) / bmpSize.cy);

				newSize = newSize * surface->GetSurfaceDPI() / 96.0;

				/* Draw image centered.
				 */
				Rect	 bmpRect = Rect(GetRealPosition() + Point(realSize.cx - newSize.cx, realSize.cy - newSize.cy) / 2, newSize);

				surface->StartPaint(GetVisibleArea());

				if (bmpSize != newSize && bitmapScaled.GetSize() != newSize) bitmapScaled = bitmap.Scale(newSize);

				if (bmpSize == newSize) surface->BlitFromBitmap(bitmap,	      Rect(Point(0, 0), bmpSize), bmpRect);
				else			surface->BlitFromBitmap(bitmapScaled, Rect(Point(0, 0), newSize), bmpRect);

				surface->EndPaint();
			}

			break;
	}

	return Success();
}

S::Int S::GUI::Image::SetBitmap(const Bitmap &newBmp)
{
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	bitmap	     = newBmp;
	bitmapScaled = NIL;

	if (GetSize() == Size(0, 0)) SetSize(bitmap.GetSize());

	if (prevVisible) Show();

	return Success();
}
