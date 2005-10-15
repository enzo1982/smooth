 /* The smooth Class Library
  * Copyright (C) 1998-2005 Robert Kausch <robert.kausch@gmx.net>
  *
  * This library is free software; you can redistribute it and/or
  * modify it under the terms of "The Artistic License, Version 2.0".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/gui/widgets/basic/image.h>
#include <smooth/gui/widgets/layer.h>
#include <smooth/graphics/surface.h>

const S::Int	 S::GUI::Image::classID = S::Object::RequestClassID();

S::GUI::Image::Image(Bitmap &bmp, Point iPos, Size iSize)
{
	type	= classID;
	bitmap	= bmp;

	possibleContainers.AddEntry(Layer::classID);

	pos	= iPos;
	size	= iSize;

	if (size.cx == 0 && size.cy == 0) size = bitmap.GetSize();
}

S::GUI::Image::~Image()
{
}

S::Int S::GUI::Image::Paint(Int message)
{
	if (!IsRegistered())	return Failure;
	if (!IsVisible())	return Success;

	Surface	*surface	= container->GetDrawSurface();
	Rect	 bmpRect	= Rect(GetRealPosition(), size);

	switch (message)
	{
		case SP_PAINT:
		case SP_UPDATE:
		case SP_SHOW:
			surface->BlitFromBitmap(bitmap, Rect(Point(0, 0), size), bmpRect);

			break;
	}

	return Success;
}

S::Int S::GUI::Image::SetBitmap(const Bitmap &newBmp)
{
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	bitmap = newBmp;

	if (size.cx == 0 && size.cy == 0) size = bitmap.GetSize();

	if (prevVisible) Show();

	return Success;
}

S::GUI::Bitmap &S::GUI::Image::GetBitmap()
{
	return bitmap;
}
