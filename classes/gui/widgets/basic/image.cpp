 /* The smooth Class Library
  * Copyright (C) 1998-2007 Robert Kausch <robert.kausch@gmx.net>
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
				Surface	*surface = container->GetDrawSurface();
				Rect	 bmpRect = Rect(GetRealPosition(), GetSize());

				surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), GetSize()), bmpRect);
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
